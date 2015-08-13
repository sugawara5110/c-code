//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(リサイズ)                     **//
//**                                  (ImageDrawの基底クラス)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

int ImageRead::resize(){

	int j, i;//for
	
	//ピクセル配列確保
	if (imgpi == NULL){
		//リサイズ前画像サイズ取得
		switch (d.mcf){
		case 0://静止画
			xs = g2.cols;
			ys = g2.rows;
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
			xrs = yrs = 400;
		}

		if (xs > ys){//元画像のxの方がサイズ大きい(小さい方を400固定)
			xrs = (int)((double)xs / ys * 400);
			yrs = 400;
		}

		if (xs < ys){ //元画像のyの方がサイズ大きい(小さい方を400固定)
			xrs = 400;
			yrs = (int)((double)ys / xs * 400);
		}

		imgpi = (int**)malloc(sizeof(int*) * yrs);
		for (i = 0; i < yrs; i++){
			imgpi[i] = (int*)malloc(sizeof(int) * xrs);
		}
	}//ピクセル配列確保完了

	//画像リサイズ
	switch (d.mcf){

	case 0://静止画
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				imgpi[yrs - j - 1][i] =
					(g2.data[(j * ys / yrs) * g2.step + (i * xs / xrs) * g2.channels() + 2] << 16) +
					(g2.data[(j * ys / yrs) * g2.step + (i * xs / xrs) * g2.channels() + 1] << 8) +
					g2.data[(j * ys / yrs) * g2.step + (i * xs / xrs) * g2.channels() + 0];
			}
		}
		break;

	case 1://動画
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				int offset = (j * ys / yrs) * linesize + (i * xs / xrs) * 3;

				imgpi[j][i] =
					(pBuffer[offset + 2] << 16) +
					(pBuffer[offset + 1] << 8) +
					pBuffer[offset + 0];
			}
		}
		break;

	case 2://カメラ
		for (j = 0; j < yrs; j++){//元サイズc_img.rows
			for (i = 0; i < xrs; i++){//元サイズc_img.cols

				imgpi[yrs - j - 1][i] =
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 2] << 16) +
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 1] << 8) +
					c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 0];
			}
		}
		break;

	}//画像リサイズ完了

	return 0;
}

