//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義コンストラクタ等               **//
//**                                    (ImageReadの派生クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h" 
#include "back_image.h"

ImageDraw::ImageDraw(){}//規定コンストラクタ

ImageDraw::ImageDraw(Dx9Init *dx, char *name) :ImageRead(name){//コンストラクタ

	//フィルター用配列確保
	block = (int**)malloc(sizeof(int*) * 400);
	for (int i = 0; i < 400; i++)block[i] = (int*)malloc(sizeof(int) * 400);

	read = NULL;
	back_image(dx, 0); //背景描画関数,引数0==画像をハンドルに格納
	dx->line(&pLine); //ライン描画
	theta_lr = 0;   //3Dモードカメラ角度
	ver = 0;        //0:描画実行
	pi16 = NULL;   //エッジ,エンボス,ノイズ除去一時保管
	slit = NULL;  //スリットスキャン一時保管
	pMyVB = NULL;//頂点バッファ
	d3 = 1;         //3Dモード切替
	finish = false;//完成画像ON OFF
}

void ImageDraw::releaseVB(){//頂点バッファ,配列解放

	if (pMyVB != NULL){
		pMyVB->Release(); pMyVB = NULL;
		free(img_a);
	}
}

void ImageDraw::obj_create(Dx9Init *dx, char *name){//オブジェクト生成関数

	if (read == NULL){//オブジェクト生成されていない時のみ実行
		read = new ImageRead(name);//画像エンボス用オブジェクト生成
	}
}

void ImageDraw::obj_delete(){//オブジェクト破棄関数

	if (read != NULL){//オブジェクト生成されている時のみ実行
		delete read;                  //オブジェクト破棄
		read = NULL;
	}
}

void ImageDraw::putfin(bool fin){ //完成画像ON OFF入力
	finish = fin;
}

int ImageDraw::theta(int f, int t){//カメラ角度操作
	if (f == 1)theta_lr += t;
	if (f == 2)theta_lr -= t;
	if (theta_lr < 0)theta_lr = 360;
	if (theta_lr > 360)theta_lr = 0;
	return theta_lr;
}

ImageDraw::~ImageDraw(){//デストラクタ

	//頂点バッファ,配列解放
	releaseVB();

	//フィルター用配列解放
	for (int i = 0; i < 400; i++)free(block[i]);
	free(block);

	pLine->Release();//線引くやつ解放
	pLine = NULL;
	obj_delete();//オブジェクト破棄関数

	if (pi16 != NULL){
		for (int i = 0; i < yrs; i++)free(pi16[i]);//エッジエンボス一時保管メモリ解放
		free(pi16);
		pi16 = NULL;
	}

	if (slit != NULL)free(slit);//スリットスキャン一時保管メモリ解放
}
