//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(リサイズ)                     **//
//**                                  (ImageDrawの基底クラス)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

// 頂点データのＦＶＦフォーマットの設定
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )

int ImageRead::pic_resize(Dx9Init *dx){

	int j, i;            //for
	static int xrsc, yrsc; //メモリ確保チェック

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
			pMyVB->Release(); pMyVB = NULL;//頂点バッファ解放
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
		
		if (d3 == 1){ if (FAILED(dx->verbufcr(&pMyVB, xrs, yrs)))return -1; }//頂点バッファー確保(通常)
		else  { if (FAILED(dx->verbufcr(&pMyVB, xrs, yrs * 2)))return -1; } //頂点バッファー確保(Z方向分で*2)

		mc = 1;//確保フラグON
	}//メモリ確保解放処理終わり

	switch (d.mcf){

	case 0://静止画
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				imcpy1[yrs - j - 1][i] = imcpy[yrs - j - 1][i] = imgpi[yrs - j - 1][i] =
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

				imcpy[j][i] = imgpi[j][i] =
					(pBuffer[offset + 2] << 16) +
					(pBuffer[offset + 1] << 8) +
					pBuffer[offset + 0];
			}
		}
		break;

	case 2://カメラ
		for (j = 0; j < yrs; j++){//元サイズc_img.rows
			for (i = 0; i < xrs; i++){//元サイズc_img.cols

				imcpy[yrs - j - 1][i] = imgpi[yrs - j - 1][i] =
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 2] << 16) +
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 1] << 8) +
					c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 0];
			}
		}
		break;

	}//switch終わり
	return 0;
}

