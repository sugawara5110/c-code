//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義(画像処理フラグ)               **//
//**                                    (ImageReadの派生クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h" 
#include "Menu.h"
#include "File.h"

int ImageDraw::image_processing_flg(Dx9Init *dx, Filter *filter, Move *move, int flg){

	if (flg >= 8 && flg <= 12 || flg >= 14 && flg <= 19 || flg == 21){
		obj_delete();//オブジェクト破棄
	}

	if (flg == 8){//個数変更
		if (d.gfr == 9)d.gfr = 0;
		return 1;//メインループ抜け
	}

	if (flg == 10) {//通常画像画像処理
		d.gfr = 0; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 11) {//モノクロ画像処理
		d.gfr = 1; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 12) {//モザイク画像処理
		d.gfr = 2; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 14) {//エッジ検出処理
		d.gfr = 3; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 15) {//エンボス処理
		d.gfr = 4; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 16) {//絵画風処理
		d.gfr = 5; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 17) {//顔面検出処理
		d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
		d.th_st = 0;//スレッド進行状況初期化
		d.gfr = 6; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 18) {//顔面モザイク処理
		d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
		d.th_st = 0;//スレッド進行状況初期化
		d.gfr = 7; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 19) {//ネガポジ処理
		d.gfr = 8; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 21) {//顔すげ替え処理
		d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
		d.th_st = 0;//スレッド進行状況初期化
		d.gfr = 10; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 20 || d.gfr == 9) {//画像エンボス処理
		static Menu menu(0);
		static File file;
		int fr = 0;
		fr = menu.mouse(dx, this, 6, 0);
		if (fr != 0 || d.gfr != 9){//メニュー操作有又はgfr==9後最初
			obj_delete();   //画像エンボス用オブジェクト破棄
			obj_create(dx, file.e_file(this, fr));//画像エンボス用オブジェクト生成
			read->drawing_img(dx, NULL, NULL, 0, 0, 0, 1);//静止画用更新
		}
		d.gfr = 9;
	}

	if (flg == 22){//3D変換
		d3 = d3 % 3 + 1;
		pMyVB->Release(); pMyVB = NULL;//頂点バッファ解放(基本的に確保後しか実行されないのでNULL確認必要無し)
		if (d3 == 1)dx->verbufcr(&pMyVB, xrs, yrs);//頂点バッファー確保(通常)
		else  dx->verbufcr(&pMyVB, xrs, yrs * 2); //頂点バッファー確保(Z方向分で*2)
	}

	return 0;
}

int ImageDraw::theta(int f, int t){//カメラ角度操作
	if (f == 1)theta_lr += t;
	if (f == 2)theta_lr -= t;
	if (theta_lr < 0)theta_lr = 360;
	if (theta_lr > 360)theta_lr = 0;
	return theta_lr;
}