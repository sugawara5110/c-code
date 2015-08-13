//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義(画像保存)                 　　**//
//**                                    (ImageReadの派生クラス)                          **//
//*****************************************************************************************//

#include "ImageRead.h"
#include "ImageDraw.h" 
#include "Move.h"

void ImageDraw::capture(Move *move){

	int i, j, k;
	cv::Mat cap(cv::Size(xrs, yrs), CV_8UC3, cv::Scalar(0, 0, 0));//カラ画像生成
	cv::Mat capflip;
	Move::para *prs = &move->paras[move->size];
	Move::imxy *img = move->img;

	//現ピクセルデータをMatに書き込み
	//パズル部以外の画像描画左側
	for (j = 0; j < 400; j++){
		for (i = 0; i < (xrs - 400) * 0.5; i++){
			//ifエッジ検出時の背景色描画スキップ処理以下同じ処理有り
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
			
		}
	}
	//パズル部以外の画像頂点右側
	for (j = 0; j < 400; j++){
		for (i = xrs - (int)((xrs - 400) * 0.5); i < xrs; i++){
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
		}
	}
	//パズル部以外の画像頂点上側
	for (j = 0; j < (yrs - 400) * 0.5; j++){
		for (i = 0; i < 400; i++){
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
		}
	}
	//パズル部以外の画像頂点下側
	for (j = yrs - (int)((yrs - 400) * 0.5); j < yrs; j++){
		for (i = 0; i < 400; i++){
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
		}
	}
	//パズル間の画像頂点
	if (move->size != 5){//size5は処理しない
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (d.gfl == 3 && imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] == 0)continue;
				if (block[j][i] == 150 || block[j][i] == 1){//フィルター値150,1のみ描画
					cap.data[(j + (yrs - 400) / 2) * cap.step + (i + (xrs - 400) / 2) * cap.channels() + 2] = 
						imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff;
					cap.data[(j + (yrs - 400) / 2) * cap.step + (i + (xrs - 400) / 2) * cap.channels() + 1] = 
						imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff;
					cap.data[(j + (yrs - 400) / 2) * cap.step + (i + (xrs - 400) / 2) * cap.channels() + 0] = 
						imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff;
				}
			}
		}
	}
	//パズル部画像頂点処理開始
	for (k = 0; k < prs->idx; k++){
		for (j = 0; j < prs->bsize; j++){     //1ブロック内y方向
			for (i = 0; i < prs->bsize; i++){//1ブロック内x方向

				//エッジ検出処理
				if (d.gfl == 3 && imgpi[j + img[k].fy + (yrs - 400) / 2]
					[i + img[k].fx + (xrs - 400) / 2] == 0)continue;

				//ブロック間は描画しない(フィルター値150,1 size==5)
				if ((block[j + img[k].fy][i + img[k].fx] == 150 ||
					block[j + img[k].fy][i + img[k].fx] == 1) &&
					move->size != 5)continue;
				cap.data[(j + img[k].cy + (yrs - 400) / 2) * cap.step + (i + img[k].cx + (xrs - 400) / 2) * cap.channels() + 2] = 
					imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 16 & 0xff;
				cap.data[(j + img[k].cy + (yrs - 400) / 2) * cap.step + (i + img[k].cx + (xrs - 400) / 2) * cap.channels() + 1] = 
					imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 8 & 0xff;
				cap.data[(j + img[k].cy + (yrs - 400) / 2) * cap.step + (i + img[k].cx + (xrs - 400) / 2) * cap.channels() + 0] = 
					imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] & 0xff;

			}
		}
	}

	//画像垂直反転
	cv::flip(cap, capflip, 0);

	//画像保存
	cv::imwrite("capture.jpg", capflip);

}