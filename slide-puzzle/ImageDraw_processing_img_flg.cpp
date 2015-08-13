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

int ImageDraw::processing_img_flg(Dx9Init *dx, Move *move, int flg){

	if (flg != 0 && d.gfl == 3)ver = 0;
	if (flg >= 8 && flg <= 12 || flg >= 14 && flg <= 19 || flg == 21){
		obj_delete();//オブジェクト破棄
	}

	if (flg == 8){//個数変更
		if (d.gfl == 9)d.gfl = 0;
		return 1;//メインループ抜け
	}

	if (flg == 10)d.gfl = 0;//通常画像画像処理
	if (flg == 11)d.gfl = 1;//モノクロ画像処理
	if (flg == 12)d.gfl = 2;//モザイク画像処理
	if (flg == 14)d.gfl = 3;//エッジ検出処理
	if (flg == 15)d.gfl = 4;//エンボス処理
	if (flg == 16)d.gfl = 5;//絵画風処理
	if (flg == 25)d.gfl = 11;//スリットスキャン処理
	if (flg == 26)d.gfl = 12;//ノイズ除去処理
	if (flg == 17) {//顔面検出処理
		d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
		d.th_st = 0;//スレッド進行状況初期化
		d.gfl = 6;
	}

	if (flg == 18) {//顔面モザイク処理
		d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
		d.th_st = 0;//スレッド進行状況初期化
		d.gfl = 7; 
	}

	if (flg == 19) {//ネガポジ処理
		d.gfl = 8; 
	}

	if (flg == 21) {//顔すげ替え処理
		d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
		d.th_st = 0;//スレッド進行状況初期化
		d.gfl = 10; 
	}

	if (flg == 20 || d.gfl == 9) {//画像エンボス処理
		static Menu menu(0);
		static File file;
		int fr = 0;
		fr = menu.mouse(dx, this, 6, 0);
		if (fr != 0 || d.gfl != 9){//メニュー操作有又はgfl==9後最初
			obj_delete();   //画像エンボス用オブジェクト破棄
			obj_create(dx, file.e_file(this, fr));//画像エンボス用オブジェクト生成
		}
		d.gfl = 9;
	}

	if (flg == 22){//3D変換
		d3 = d3 % 3 + 1;
		releaseVB();//頂点バッファ,配列解放
		ver = 0;
	}

	if (flg == 23){//キャプチャ
		capture(move);
	}

	if (flg == 24)putrate(1.0f);//再生速度標準戻し

	return 0;
}