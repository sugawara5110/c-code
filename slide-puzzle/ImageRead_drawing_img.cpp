//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(描画読み込み)                 **//
//**                                    (ImageDrawの基底クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
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
		for (j = 0; j < g2.rows; j++){
			for (i = 0; i < g2.cols; i++){
				
				if (dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2] > 0 ||   //データの有無
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1] > 0 ||
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0] > 0)
				{
					g2.data[j * g2.step + i * g2.channels() + 2] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2];
					g2.data[j * g2.step + i * g2.channels() + 1] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1];
					g2.data[j * g2.step + i * g2.channels() + 0] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0];
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

int ImageRead::drawing_img(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z, int f){ //move==NULL:無加工画像表示, x,yは描画時のオフセット量, f == 0:静止画更新無, 1:静止画更新有(動画は常に更新)

	int i, j;//for

	if ((d.gfr == 6 || d.gfr == 7 || d.gfr == 10) && face_detect_h == NULL){
		face_detect_h = (HANDLE)_beginthreadex(NULL, 0, face_detect, &d, 0, NULL);//スレッドスタート
	}
	if (d.gfr != 6 && d.gfr != 7 && d.gfr != 10){ d.th_f = 0; d.th_st = 0; }//検出処理未選択時フラグ初期化

	/********************************************************************************************************************************/
	/********************************************動画 カメラ初期処理開始*************************************************************/
	/********************************************************************************************************************************/

	if (d.mcf == 1 || d.mcf == 2){//動画,カメラ読み込み処理開始
		if (d.mcf == 1){//動画読み込み処理開始

			//ストリームの合計時間幅を基準とする、現在の位置を取得する。
			d.pMediaPosition->get_CurrentPosition(&d.time1);

			//現位置と終了位置が同じ場合スタート位置にセット
			if (d.time1 == d.time2)d.pMediaPosition->put_CurrentPosition(0);

			//最新画像データをpBufferに格納
			pSampleGrabber->GetCurrentBuffer(&nBufferSize, (long *)(pBuffer));

			d.th_f = 1;//検出スレッドスタート

		}//動画読み込み処理終了

		if (d.mcf == 2){//カメラ読み込み処理開始

			int cn = 0;//カウント
			do{
				d.cap >> c_img;           //画像データ更新
				if (cn == 3){
					d.mcf = 0; g2 = cv::imread("./dat/img/z_cam_ewc.dat", CV_LOAD_IMAGE_COLOR);//画像読み込み
					break;
				}
				cn++;
			} while (c_img.data == NULL);//中身がNULLの間は次の処理に行かない

			d.th_f = 1;//検出スレッド稼働　   

		}//カメラ読み込み処理終了

		//カメラ,動画選択時の検出後処理
		if (d.gfr == 6 || d.gfr == 7 || d.gfr == 10){
			if (d.th_st){            //検出スレッド1回目の処理完了してるか?
				face_detect_after();//検出処理後の描画処理関数呼び出し
			}
		}

		if (FAILED(pic_resize(dx))) return -1;//画像リサイズ処理

	}//動画,カメラ読み込み処理終了

	/********************************************************************************************************************************/
	/********************************************動画 カメラ初期処理終了*************************************************************/
	/********************************************************************************************************************************/

	/********************************************************************************************************************************/
	/*********************************************静止画初期処理開始*****************************************************************/
	/********************************************************************************************************************************/
	if (d.mcf == 0){
		if (f == 1){   //更新が必要な時のみ実行する
			g2 = cv::imread(d.g_name, CV_LOAD_IMAGE_COLOR);//画像読み込み

			if (d.gfr == 6 || d.gfr == 7 || d.gfr == 10){//顔面検出選択時
				d.th_f = 1;                 //検出スレッド検出部ループON
				while (d.th_st != 1);      //検出スレッド処理完待ち(volatile付けないと最適化で無限ループになる)
				face_detect_after();      //検出処理後の描画処理関数呼び出し
			}

			if (FAILED(pic_resize(dx))) return -1;    //画像リサイズ処理
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


	if (FAILED(draw(dx, filter, move, x, y, z))) return -1;//各種画像処理,描画処理
	return 0;
}//drawing_img()終了

HRESULT ImageRead::draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z){ return S_OK; }//オーバーライドで使う