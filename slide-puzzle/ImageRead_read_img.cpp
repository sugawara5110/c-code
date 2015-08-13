//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(描画読み込み)                 **//
//**                                    (ImageDrawの基底クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "Move.h"

void ImageRead::face_detect_after(){    //検出処理後の描画処理関数

	cv::Mat dte;
	d.lock = 1;                                //検索スレッドからのデータ操作ロック
	if (d.lock_t == 1){ d.lock = 0; return; }//検索スレッドロックの場合, ロック解除後return
	dte = d.mt_temp.clone();                //本スレデータ引き渡し.clone()完全コピー
	d.lock = 0;                             //ロック解除

	int i, j;

	//dte.data[]の要素毎にデータが入ってる要素のみ拡大描画
	for (j = 0; j < yrs; j++){
		for (i = 0; i < xrs; i++){

			if (dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 2] > 0 ||  //データの有無
				dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 1] > 0 ||
				dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 0] > 0)
			{
				imgpi[yrs - j - 1][i] =
					(dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 2] << 16) +
					(dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 1] << 8) +
					dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 0];
			}
		}
	}
}

int ImageRead::read_img(){ //画像読み込み

	if ((d.gfl == 6 || d.gfl == 7 || d.gfl == 10) && face_detect_h == NULL){
		face_detect_h = (HANDLE)_beginthreadex(NULL, 0, face_detect, &d, 0, NULL);//スレッドスタート
	}
	if (d.gfl != 6 && d.gfl != 7 && d.gfl != 10){ d.th_f = 0; d.th_st = 0; }//検出処理未選択時フラグ初期化

	if (d.mcf == 0)g2 = cv::imread(d.g_name, CV_LOAD_IMAGE_COLOR);//画像読み込み

	if (d.mcf == 1){//動画読み込み処理開始

		//再生速度1.0標準 有効範囲0.1～4.0 
		pMediaPosition->put_Rate(rate);

		//ストリームの合計時間幅を基準とする、現在の位置を取得する。
		pMediaPosition->get_CurrentPosition(&d.time1);

		//現位置と終了位置が同じ場合スタート位置にセット
		if (d.time1 == time2)pMediaPosition->put_CurrentPosition(0);

		//最新画像データをpBufferに格納
		pSampleGrabber->GetCurrentBuffer(&nBufferSize, (long *)(pBuffer));

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

	}//カメラ読み込み処理終了

	if (FAILED(resize())) return -1;//画像リサイズ処理

	if (d.gfl == 6 || d.gfl == 7 || d.gfl == 10){
		d.th_f = 1;                 //検出スレッド検出部ループON
		if (d.th_st){            //検出スレッド1回目の処理完了してるか?
			face_detect_after();//検出処理後の描画処理関数呼び出し
		}
	}
	return 0;
}