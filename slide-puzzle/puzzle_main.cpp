//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　スライドパズルmain                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sound.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Menu.h"
#include "File.h"
#include "Filter.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){    //メイン関数

	//ウィンドウモード変更, DxLib初期化, ウィンドウサイズ変更, 裏画面設定
	ChangeWindowMode(TRUE), DxLib_Init(), SetGraphMode(800, 600, 32); SetDrawScreen(DX_SCREEN_BACK);
	//非アクティブでも処理続行
	SetAlwaysRunFlag(TRUE);

	int i, j, k;          //for文
	int frg, frgr;       //キー入力,入力履歴
	int fr;
	int Color;         //色
	int sm[2];        //手数計算処理前スペース座標退避用

	Move move;          //移動クラスオブジェクト生成
	File file;         //ファイルクラスオブジェクト生成
	Filter filter;    //フィルタークラスオブジェクト生成
	Menu menu;       //メニュークラスオブジェクト生成
	ImageDraw *draw;//画像描画クラスオブジェクト定義

	srand((unsigned)time(NULL));
	InitSoftImage();                 //ソフトイメージ全開放
	Color = GetColor(255, 255, 255);//字の色

	menu.mouse(NULL, 0, 0); //マウス関数初期設定 
	sound(0);              //サウンド関数初期処理

	draw = new ImageDraw(file.file());//ファイル関数返り値でdrawオブジェクト生成

	while (1){ //1番外側のwhile文

		frg = 0;       //移動フラグ初期化

		while (frg == 0){    //個数決定
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(move.img); return -1; }//強制終了
			draw->drawing_img(&filter, NULL, 0, 0, 1); //描画前処理
			draw->drawing_img(&filter, NULL, 0, 0, 0);//描画
			frg = menu.mouse(draw, 2, 0);  //マウス関数
			ScreenFlip();          //表画面に描写
		}                         //while終わり
		move.size = frg - 1;     //size決定
		sound(1);               //サウンド関数ボタン音

		//メモリ確保
		move.img = (Move::imxy*)malloc(sizeof(Move::imxy)*move.paras[move.size].idx);//画像座標

		//ブロック画像フィルター作成
		filter.filter(&move, draw);
		//ブロック画像フィルター作成終了

		k = 0;
		for (j = 100; j <= move.paras[move.size].lastposy; j += move.paras[move.size].bsize){    //座標データ初期化
			for (i = 200; i <= move.paras[move.size].lastposx; i += move.paras[move.size].bsize){
				move.img[k].cx = move.img[k].fx = move.img[k].chx = i;
				move.img[k].cy = move.img[k].fy = move.img[k++].chy = j;
			}
		}

		move.shuffle(); //シャッフル関数
		move.change(&filter, draw);//ブロック交換関数

		while (1){//パズル実行中ループ

			frg = 0; //移動フラグ初期化

			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(move.img); return -1; }

			frg = menu.mouse(draw, 3, 0); //マウス関数
		 
			if (frg == 20 || draw->d.gfr == 9) {//画像エンボス処理
				fr = 0;
				fr = menu.mouse(draw, 6, 0);
				if (fr != 0 || draw->d.gfr != 9){//メニュー操作有又はgfr==9後最初
					draw->obj_delete();   //画像エンボス用オブジェクト破棄
					draw->obj_create(file.e_file(draw, fr));//画像エンボス用オブジェクト生成
					draw->read->drawing_img(NULL, NULL, 0, 0, 1);//静止画用更新
				}
				draw->d.gfr = 9;
			}

			if (draw->d.d3f == 1 || draw->d.d3f == 2){//3D画像
				fr = 0;
				fr = menu.mouse(draw, 7, 0);
				if (fr == 1)
				{
					draw->d.theta_lr -= 1;
					if (draw->d.theta_lr < 0)draw->d.theta_lr = 360;
				}
				if (fr == 2)
				{
					draw->d.theta_lr += 1;
					if (draw->d.theta_lr > 360)draw->d.theta_lr = 0;
				}
			}

			ScreenFlip();       //表画面描写
			if (frg >= 5) {    //移動フラグの時は飛ばす
				if (frg == 5){//全ブロック位置元通り処理↓
					sound(1);//サウンド関数ボタン音  
					for (i = 0; i < move.paras[move.size].idx; i++){ move.img[i].chx = move.img[i].fx; move.img[i].chy = move.img[i].fy; } //完成画像データからコピー
					move.change(&filter, draw);//ブロック交換関数
				}             //if終わり

				if (frg == 6){        //並び替え
					sound(1);        //サウンド関数ボタン音
					move.shuffle(); //シャッフル関数
					move.change(&filter, draw);//ブロック交換関数
				}                     //if終わり

				if (frg == 7){ //automatic関数実行
					sound(1); //サウンド関数ボタン音
					move.auto_matic(&filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0);
					draw->drawing_img(&filter, &move, 0, 0, 0);
				}

				if (frg >= 8 && frg <= 12 || frg >= 14 && frg <= 19 || frg == 21){ //画像処理前準備
					draw->obj_delete();//オブジェクト破棄
				}

				if (frg == 8){//個数変更
					InitSoftImage(); free(move.img);
					draw->d.th_st = 0;           //スレッド進行状況初期化
					draw->d.gfr = 0;            //画像モードリセット
					break;
				} //読み込み済みグラフィックデータ削除,メモリ解放,break

				if (frg == 9){             //画像変更
					draw->d.th_f = 0;     //検出スレッドフラグ初期化
					draw->d.gfr = 0;     //画像モードリセット
					delete draw;        //オブジェクト破棄  
					draw = new ImageDraw(file.file()); //ファイル関数呼び出し,画像描画オブジェクト生成
					draw->d.th_st = 0; //検出スレッド進行状況初期化
					draw->drawing_img(&filter, &move, 0, 0, 1);//背景,画像描画
				}

				if (frg == 10) {//通常画像画像処理
					draw->d.gfr = 0; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 11) {//モノクロ画像処理
					draw->d.gfr = 1; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 12) {//モザイク画像処理
					draw->d.gfr = 2; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 13){//手数計算
					move.tkf = 1; move.tkc = 0;//フラグ,カウント初期化
					for (i = 0; i < move.paras[move.size].idx; i++){
						move.img[i].chx = move.img[i].cx;//手数計算前座標値退避
						move.img[i].chy = move.img[i].cy;//手数計算前座標値退避
					}
					sm[0] = move.space[0];//手数計算前座標値退避
					sm[1] = move.space[1];//手数計算前座標値退避
					DrawFormatString(305, 502, Color, "画像内クリックで中止");
					ScreenFlip();
					move.auto_matic(&filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0);//auto_matic関数処理

					for (i = 0; i < move.paras[move.size].idx; i++){
						move.img[i].cx = move.img[i].chx;//手数計算後座標値戻し
						move.img[i].cy = move.img[i].chy;//手数計算後座標値戻し
					}
					move.space[0] = sm[0];//手数計算後座標値戻し
					move.space[1] = sm[1];//手数計算後座標値戻し
					if (move.cnt_results != -1){//-1だと中止してる事になる
						DrawBox(300, 500, 500, 540, 1, TRUE);
						DrawFormatString(305, 502, Color, "%d手で完了します", move.tkc);
						DrawFormatString(305, 522, Color, "画像内クリックで終了");
						ScreenFlip();
						while (menu.mouse(draw, 5, 0) != 1);
					}//if終わり
					draw->drawing_img(&filter, &move, 0, 0, 1);
					move.tkf = 0; move.tkc = 0;//フラグ,カウント初期化
				}//手数計算終了

				if (frg == 14) {//エッジ検出処理
					draw->d.gfr = 3; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 15) {//エンボス処理
					draw->d.gfr = 4; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 16) {//絵画風処理
					draw->d.gfr = 5; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 17) {//顔面検出処理
					draw->d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
					draw->d.th_st = 0;//スレッド進行状況初期化
					draw->d.gfr = 6; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 18) {//顔面モザイク処理
					draw->d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
					draw->d.th_st = 0;//スレッド進行状況初期化
					draw->d.gfr = 7; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 19) {//ネガポジ処理
					draw->d.gfr = 8; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 21) {//顔すげ替え処理
					draw->d.th_f = 0; //検出スレッドスタートフラグ0:ストップ 1:スタート
					draw->d.th_st = 0;//スレッド進行状況初期化
					draw->d.gfr = 10; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frgr != 22 && frg == 22){//3D画像
					if (draw->d.d3f == 0)draw->d.d3f = 1;
					else if (draw->d.d3f == 1)draw->d.d3f = 2;
					else if (draw->d.d3f == 2)draw->d.d3f = 0;
					draw->drawing_img(&filter, &move, 0, 0, 1);//背景,画像描画
				}

			}//移動フラグの時は飛ばす

			move.mov(&filter, draw, 0, frg, 0); //移動処理

			draw->drawing_img(&filter, &move, 0, 0, 0);//移動しない場合の為の処理

			frgr = frg;//キー入力履歴
		}//パズル実行中ループ

	}//一番外のwhile終わり
	InitSoftImage();        //ソフトイメージ全開放
	DxLib_End();           // DXライブラリ終了処理
	free(move.img); return 0;//メモリ解放,終了
} // winmain終わり

//********************************************************************************************************//
//**** libjpeg　Copyright (C) 1991-2013, Thomas G. Lane, Guido Vollbeding.  ******************************//
//**** DX Library Copyright (C) 2001-2014 Takumi Yamada.   ***********************************************//
//********************************************************************************************************//