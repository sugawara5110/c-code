//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　画像リサイズ処理                                    　**//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "struct_set.h"

void pic_resize(alldata *p, int g2){//g2:元画像ソフトイメージ

	int j, i;            //for
	int r, g, b, a;     //色情報
	int xs, ys;             //元画像サイズ
	static int xrsc, yrsc; //メモリ確保チェック
		
	GetSoftImageSize(g2, &xs, &ys);          //変換元画像サイズ取得
	
		if (xs == ys){//元画像のxyが同じサイズ
			xrsc = yrsc = 400;
		}

		if (xs > ys){//元画像のxの方がサイズ大きい(小さい方を400固定)
			xrsc = (double)xs / ys * 400;
			yrsc = 400;
		}

		if (xs < ys){ //元画像のyの方がサイズ大きい(小さい方を400固定)
			xrsc = 400;
			yrsc = (double)ys / xs * 400;
		}
		if (p->xrs != xrsc || p->yrs != yrsc){  //どちらかが偽の場合画像変更されたと判断,メモリ解放,メモリ再確保
			if (p->mc == 1){                   //mc==0の場合はメモリ確保してない為解放処理しない(アクセス違反防止)
				                              //↓更新前画像サイズ分の解放なのでxrsc,yrscでp->xrs,yrs更新前に処理
				for (i = 0; i < p->yrs; i++){ free(p->imgpi[i]); free(p->imcpy[i]); }
				free(p->imgpi); free(p->imcpy);
			}
			p->xrs = xrsc; //構造体にコピー
			p->yrs = yrsc;//構造体にコピー
			p->imgpi = (int**)malloc(sizeof(int*) * p->xrs);//更新後画像サイズ分の確保なのでxrsc,yrscでp->xrs,yrs更新後に処理
			p->imcpy = (int**)malloc(sizeof(int*) * p->xrs);
			for (i = 0; i < p->xrs; i++){ p->imgpi[i] = (int*)malloc(sizeof(int) * p->yrs); p->imcpy[i] = (int*)malloc(sizeof(int) * p->yrs); }
			p->mc = 1;//確保フラグON
		}//メモリ確保解放処理終わり

	for (j = 0; j < p->yrs; j++){
		for (i = 0; i < p->xrs; i++){
			GetPixelSoftImage(g2, i*xs / p->xrs, j*ys / p->yrs, &r, &g, &b, &a);//元画像から色情報取得
			p->imcpy[i][j] = p->imgpi[i][j] = (r << 16) + (g << 8) + b;//画像色情報格納変数に色情報格納
		}
	}
	InitSoftImage();//ソフトイメージ全開放
}
	
