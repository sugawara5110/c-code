//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(描画読み込み)                 **//
//**                                    (ImageDrawの基底クラス)                          **//
//*****************************************************************************************//

#include "DxLib.h"
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "Move.h"
#include "Filter.h"

void ImageRead::face_detect_after(){    //検出処理後の描画処理関数

	cv::Mat dte;
	d.lock = 1;                               //検索スレッドからのデータ操作ロック
	if (d.lock_t == 1){ d.lock = 0; return; }//検索スレッドロックの場合, ロック解除後return
	dte = d.mt_temp.clone();                //本スレデータ引き渡し.clone()完全コピー
	d.lock = 0;                            //ロック解除

	int i, j;

	//dte.data[]の要素毎にデータが入ってる要素のみ拡大描画
	switch (d.mcf){

	case 0://静止画
		for (j = 0; j < ys; j++){
			for (i = 0; i < xs; i++){
				if (dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2] > 0 ||   //データの有無
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1] > 0 ||
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0] > 0)
				{
					DrawPixelSoftImage(g2, i, j, dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2],
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1],
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0], 0);
				}
			}
		}
		break;

	case 1://動画
		for (j = 0; j < pVideoInfoHeader->bmiHeader.biHeight; j++){
			for (i = 0; i < pVideoInfoHeader->bmiHeader.biWidth; i++){
				int offset = j * linesize + i * 3;
				int hei = pVideoInfoHeader->bmiHeader.biHeight - j - 1;//directshowは逆からなので反転 

				if (dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 2] > 0 ||   //データの有無
					dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 1] > 0 ||
					dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 0] > 0)
				{
					pBuffer[offset + 2] = dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 2];
					pBuffer[offset + 1] = dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 1];
					pBuffer[offset + 0] = dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 0];
				}
			}
		}
		break;

	case 2://カメラ
		for (j = 0; j < c_img.rows; j++){
			for (i = 0; i < c_img.cols; i++){
				if (dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2] > 0 ||   //データの有無
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1] > 0 ||
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0] > 0)
				{
					c_img.data[j * c_img.step + i * c_img.channels() + 2] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2];
					c_img.data[j * c_img.step + i * c_img.channels() + 1] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1];
					c_img.data[j * c_img.step + i * c_img.channels() + 0] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0];
				}
			}
		}
		break;

	}//switch

}

void ImageRead::drawing_img(Filter *filter, Move *move, int x, int y, int f){ //move==NULL:無加工画像表示, x,yは描画時のオフセット量, f == 0:静止画更新無, 1:静止画更新有(動画は常に更新)
	
	int i, j;//for
	REFTIME time1, time2;//再生時間取得用
	
	if ((d.gfr == 6 || d.gfr == 7 || d.gfr == 10) && face_detect_h == NULL){
		face_detect_h = (HANDLE)_beginthreadex(NULL, 0, face_detect, &d, 0, NULL);//スレッドスタート
	}
	if (d.gfr != 6 && d.gfr != 7 && d.gfr != 10){d.th_f = 0; d.th_st = 0;}//検出処理未選択時フラグ初期化

/********************************************************************************************************************************/
/********************************************動画 カメラ初期処理開始*************************************************************/
/********************************************************************************************************************************/
	
	if (d.mcf == 1 || d.mcf == 2){//動画,カメラ読み込み処理開始
		if (d.mcf == 1){//動画読み込み処理開始

			//ストリームの合計時間幅を基準とする、現在の位置を取得する。
			d.pMediaPosition->get_CurrentPosition(&time1);

			//ストリームの時間幅を取得する。
			d.pMediaPosition->get_Duration(&time2);

			//現位置と終了位置が同じ場合スタート位置にセット
			if (time1 == time2)d.pMediaPosition->put_CurrentPosition(0);

			//最新画像データをpBufferに格納
			pSampleGrabber->GetCurrentBuffer(&nBufferSize, (long *)(pBuffer));
			
			d.th_f = 1;//検出スレッドスタート

		}//動画読み込み処理終了

		if (d.mcf == 2){//カメラ読み込み処理開始
		
			do{
				d.cap >> c_img;           //画像データ更新
			} while (c_img.data == NULL);//中身がNULLの間は次の処理に行かない

			d.th_f = 1;//検出スレッド稼働　   

			if (d.cap.isOpened() == false){//カメラ未接続の場合"z_cam_ewc.dat"をそのままg2に格納
				DeleteGraph(g2); d.mcf = 0; g2 = LoadSoftImage("./dat/img/z_cam_ewc.dat");
			}

		}//カメラ読み込み処理終了

		//カメラ,動画選択時の検出後処理
		if (d.gfr == 6 || d.gfr == 7 || d.gfr == 10){
			if (d.th_st){            //検出スレッド1回目の処理完了してるか?
				face_detect_after();//検出処理後の描画処理関数呼び出し
			}
		}

		pic_resize();//画像リサイズ処理
		DeleteSoftImage(g2);//g2解放

	}//動画,カメラ読み込み処理終了
	
/********************************************************************************************************************************/
/********************************************動画 カメラ初期処理終了*************************************************************/
/********************************************************************************************************************************/

/********************************************************************************************************************************/
/*********************************************静止画初期処理開始*****************************************************************/
/********************************************************************************************************************************/
	if (d.mcf == 0){  
		if (f == 1){   //更新が必要な時のみ実行する
			g2 = LoadSoftImage(d.g_name);      //画像読み込み
			GetSoftImageSize(g2, &xs, &ys);   //変換元画像サイズ取得

			if (d.gfr == 6 || d.gfr == 7 || d.gfr == 10){//顔面検出選択時
				d.th_f = 1;                 //検出スレッド検出部ループON
				while (d.th_st != 1);      //検出スレッド処理完待ち(volatile付けないと最適化で無限ループになる)
				face_detect_after();      //検出処理後の描画処理関数呼び出し
			}

			pic_resize();        //画像リサイズ処理
			DeleteSoftImage(g2);//ハンドルg2解放
		}

		if (f == 0){  //更新無しの場合実行する
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					imgpi[j][i] = imcpy1[j][i];
				}
			}
		}
	}

/********************************************************************************************************************************/
/*********************************************静止画初期処理終了*****************************************************************/
/********************************************************************************************************************************/


	draw(filter, move, x, y);//各種画像処理,描画処理

	InitSoftImage();//ソフトイメージ全開放
}//drawing_img()終了

void ImageRead::draw(Filter *filter, Move *move, int x, int y){}//オーバーライドで使う