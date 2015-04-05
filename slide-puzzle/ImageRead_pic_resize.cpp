//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(リサイズ)                     **//
//**                                  (ImageDrawの基底クラス)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

void ImageRead::pic_resize(){

	int j, i;            //for
	int r, g, b, a;     //色情報
	static int xrsc, yrsc; //メモリ確保チェック

	switch (d.mcf){
	case 0://静止画
		GetSoftImageSize(g2, &xs, &ys);//変換元画像サイズ取得
		break;

	case 1://動画
		xs = pVideoInfoHeader->bmiHeader.biWidth;
		ys = pVideoInfoHeader->bmiHeader.biHeight;
		break;

	case 2://カメラ
		xs = c_img.cols;
		ys = c_img.rows;
		break;

	}//switch
	
		if (xs == ys){//元画像のxyが同じサイズ
			xrsc = yrsc = 400;
		}

		if (xs > ys){//元画像のxの方がサイズ大きい(小さい方を400固定)
			xrsc = (int)((double)xs / ys * 400);
			yrsc = 400;
		}

		if (xs < ys){ //元画像のyの方がサイズ大きい(小さい方を400固定)
			xrsc = 400;
			yrsc = (int)((double)ys / xs * 400);
		}

		if (xrs != xrsc || yrs != yrsc){  //どちらかが真の場合画像変更されたと判断,メモリ解放,メモリ再確保
			if (mc == 1){                //mc==0の場合はメモリ確保してない為解放処理しない(アクセス違反防止)
				//↓更新前画像サイズ分の解放なのでxrsc,yrscでxrs,yrs更新前に処理
				for (i = 0; i < yrs; i++){ free(imgpi[i]); free(imcpy[i]); free(imcpy1[i]); }
				free(imgpi); free(imcpy); free(imcpy1);
			}
			xrs = xrsc; //コピー
			yrs = yrsc;//コピー
			imgpi = (int**)malloc(sizeof(int*) * yrs);//更新後画像サイズ分の確保なのでxrsc,yrscでxrs,yrs更新後に処理
			imcpy = (int**)malloc(sizeof(int*) * yrs);
			imcpy1 = (int**)malloc(sizeof(int*) * yrs);
			for (i = 0; i < yrs; i++){
				imgpi[i] = (int*)malloc(sizeof(int) * xrs);
				imcpy[i] = (int*)malloc(sizeof(int) * xrs);
				imcpy1[i] = (int*)malloc(sizeof(int) * xrs);
			}
			mc = 1;//確保フラグON
		}//メモリ確保解放処理終わり

		switch (d.mcf){

		case 0://静止画
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					GetPixelSoftImage(g2, i * xs / xrs, j * ys / yrs, &r, &g, &b, &a);       //元画像から色情報取得
					imcpy1[j][i] = imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;//画像色情報格納変数に色情報格納
				}
			}
			break;

		case 1://動画
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					int offset = (j * ys / yrs) * linesize + (i * xs / xrs) * 3;

					imcpy[yrs - j - 1][i] = imgpi[yrs - j - 1][i] =
						(pBuffer[offset + 2] << 16) +
						(pBuffer[offset + 1] << 8) +
						pBuffer[offset + 0];
				}
			}
			break;

		case 2://カメラ
			for (j = 0; j < yrs; j++){//元サイズc_img.rows
				for (i = 0; i < xrs; i++){//元サイズc_img.cols

					imcpy[j][i] = imgpi[j][i] =
						(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 2] << 16) +
						(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 1] << 8) +
						 c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 0];
				}
			}
			break;

		}//switch終わり
			
	InitSoftImage();//ソフトイメージ全開放
}

