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
#include "struct_set.h"
#include "drawing_img.h"
#include "file.h"
#include "mov.h"
#include "auto_set.h"
#include "cha_shu.h"
#include "back_image.h"
#include "mouse.h"
#include "sound.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){    //メイン関数

	//ウィンドウモード変更, DxLib初期化, ウィンドウサイズ変更, 裏画面設定
	ChangeWindowMode(TRUE), DxLib_Init(), SetGraphMode(800, 600, 32); SetDrawScreen(DX_SCREEN_BACK); 
	//非アクティブでも処理続行
	SetAlwaysRunFlag(TRUE);

	alldata p;             //ほぼ全関数で使うデータ群
	int i, j, k;          //for文
	int frg = 0;         //キー入力
	int Color;          //色
	time_t sec1,sec2;  //待ち時間用
	int stl;          //画像ファイル名文字列長さ
	int sm[2];       //手数計算処理前スペース座標退避用
    
	p.cnt_results = 0;//手数計算結果確認用
	p.gfr = 0;       //画像モード初期化(モノクロ等)

	p.paras[0] = { 16, 4, 100, 500, 400, 400, 15, 31, 5, 10 };    //ブロック個数毎のパラメーター
	p.paras[1] = { 64, 8, 50, 550, 450, 800, 63, 127, 5, 10 };
	p.paras[2] = { 256, 16, 25, 575, 475, 1000, 255, 511, 5, 5 };
	p.paras[3] = { 1600, 40, 10, 590, 490, 2000, 1599, 3199, 2, 5 };
	p.paras[4] = { 10000, 100, 4, 596, 496, 9000, 9999, 19999, 2, 4 };
	p.paras[5] = { 160000, 400, 1, 599, 499, 300000, 159999, 319999, 1, 1 };
	
	srand((unsigned)time(NULL));
	InitSoftImage();                 //ソフトイメージ全開放
	Color = GetColor(255, 255, 255);//字の色
    
	//↓初期設定
	back_image(0);     //背景描画関数,引数0==画像をハンドルに格納
	mouse(&p,0,0);    //マウス関数初期設定 
	sound(0);        //サウンド関数初期処理
	p.size = 0;     //エラー防止 
	file(&p);      //画像ファイル選択関数呼び出しで,p.g_nameに画像ファイル名格納
	p.tkf = 0;    //手数計算実行フラグ初期化
	p.tkc = 0;   //手数計算カウント初期化
	p.mc = 0;   //pic_resize内メモリ確保チェック初期化 0:未確保, 1:確保済み
		
	while (1){ //1番外側のwhile文

		back_image(1);         //背景描画関数,引数1==画像描画

		while (frg == 0){    //個数決定
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(p.img); return -1; }//強制終了
            frg = mouse(&p,2,0);//マウス関数
			ScreenFlip();      //表画面に描写
            p.size = frg - 1; //size決定
        }                    //while終わり
		sound(1);           //サウンド関数ボタン音

		//メモリ確保
		p.img = (imxy*)malloc(sizeof(imxy)*p.paras[p.size].idx);//画像座標
		p.block = (int**)malloc(sizeof(int*) * 400);//ブロック画像(drawing_imgで処理だと描画遅くなるので)
		for (i = 0; i < 400; i++)p.block[i] = (int*)malloc(sizeof(int) * 400);

		//ブロック画像フィルター作成
		for (j = 0; j < 400; j++){   
			for (i = 0; i < 400; i++){
				//外周
				if (j%p.paras[p.size].bsize == 0 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 1 ||
					i%p.paras[p.size].bsize == 0 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 1){
					p.block[i][j] = 150; continue;
				}
				//中
				if (p.size < 4 &&   //size3以下だけ
					(j%p.paras[p.size].bsize == 1 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 2 ||
					i%p.paras[p.size].bsize == 1 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 2) &&
					j%p.paras[p.size].bsize != 0 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 1 &&
					i%p.paras[p.size].bsize != 0 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 1){
					p.block[i][j] = 90; continue;
				}
				//中
				if (p.size < 4 &&   //size3以下だけ
					(j%p.paras[p.size].bsize == 2 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 3 ||
					i%p.paras[p.size].bsize == 2 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 3) &&
					j%p.paras[p.size].bsize != 1 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					i%p.paras[p.size].bsize != 1 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					j%p.paras[p.size].bsize != 0 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 1 &&
					i%p.paras[p.size].bsize != 0 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 1){
					p.block[i][j] = 50; continue;
				}
				//内側
				if (p.size < 4 &&   //size3以下だけ
					(j%p.paras[p.size].bsize == 3 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 4 ||
					i%p.paras[p.size].bsize == 3 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 4)&&
					j%p.paras[p.size].bsize != 2 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 3 &&
					i%p.paras[p.size].bsize != 2 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 3 &&
					j%p.paras[p.size].bsize != 1 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					i%p.paras[p.size].bsize != 1 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					j%p.paras[p.size].bsize != 0 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 1 &&
					i%p.paras[p.size].bsize != 0 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 1){
					p.block[i][j] = 20; continue;
				}
				//該当無し
				p.block[i][j] = 0;
			}
		}//ブロック画像フィルター作成終了
		
		k = 0;
		for (j = 100; j <= p.paras[p.size].lastposy; j += p.paras[p.size].bsize){    //座標データ初期化
			for (i = 200; i <= p.paras[p.size].lastposx; i += p.paras[p.size].bsize){
				p.img[k].cx = p.img[k].fx = p.img[k].chx = i;
				p.img[k].cy = p.img[k].fy = p.img[k++].chy = j;
			}
		}

		drawing_img(&p, 0, 0, 1);//初期画像描画
		ScreenFlip();           //表画面描写

		sec2 = 0; for (time(&sec1); sec2 - sec1 <= 0.7; time(&sec2));//待ち時間

		shuffle(&p);               //シャッフル関数
		change(&p);               //ブロック交換関数
		frg = 0;
		while (1){

				if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(p.img); return -1; }

				frg = mouse(&p,3,0);  //マウス関数

				ScreenFlip();       //表画面描写
				if (frg >= 5) {    //移動フラグ以外の時は飛ばす
					if (frg == 5){//全ブロック位置元通り処理↓
						sound(1);//サウンド関数ボタン音  
						for (i = 0; i < p.paras[p.size].idx; i++){ p.img[i].chx = p.img[i].fx; p.img[i].chy = p.img[i].fy; } //完成画像データからコピー
						change(&p);//ブロック交換関数
					}             //if終わり

					if (frg == 6){     //更に並び替え
						sound(1);     //サウンド関数ボタン音
						shuffle(&p); //シャッフル関数
						change(&p); //ブロック交換関数
					}              //if終わり

					if (frg == 7){ //automatic関数実行
						sound(1); //サウンド関数ボタン音
						auto_matic(&p, 0, p.paras[p.size].pcs, p.paras[p.size].pcs, 0);
						drawing_img(&p, 0, 0, 0);
					}

					if (frg == 8){//個数変更
						InitSoftImage(); free(p.img); frg = 0; break;
					} //読み込み済みグラフィックデータ削除,メモリ解放,break

					if (frg == 9){//画像変更
						frg = 0; file(&p); back_image(1); drawing_img(&p, 0, 0, 1);
					} //ファイル関数呼び出し,背景,画像描画

					if (frg == 10) {//通常画像画像処理
						p.gfr = 0; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 11) {//モノクロ画像処理
						p.gfr = 1; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 12) {//モザイク画像処理
						p.gfr = 2; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 13){//手数計算
						p.tkf = 1; p.tkc = 0;//フラグ,カウント初期化
						for (i = 0; i < p.paras[p.size].idx; i++){
							p.img[i].chx = p.img[i].cx;//手数計算前座標値退避
							p.img[i].chy = p.img[i].cy;//手数計算前座標値退避
						}
						sm[0] = p.space[0];//手数計算前座標値退避
						sm[1] = p.space[1];//手数計算前座標値退避
						DrawFormatString(305, 502, Color, "画像内クリックで中止");
						ScreenFlip();
						auto_matic(&p, 0, p.paras[p.size].pcs, p.paras[p.size].pcs, 0);//auto_matic関数処理

						for (i = 0; i < p.paras[p.size].idx; i++){
							p.img[i].cx = p.img[i].chx;//手数計算後座標値戻し
							p.img[i].cy = p.img[i].chy;//手数計算後座標値戻し
						}
						p.space[0] = sm[0];//手数計算後座標値戻し
						p.space[1] = sm[1];//手数計算後座標値戻し
						if (p.cnt_results != -1){//-1だと中止してる事になる
							DrawBox(300, 500, 500, 540, 1, TRUE);
							DrawFormatString(305, 502, Color, "%d手で完了します", p.tkc);
							DrawFormatString(305, 522, Color, "画像内クリックで終了");
							ScreenFlip();
							while (mouse(&p, 5, 0) != 1);
						}//if終わり
						drawing_img(&p, 0, 0, 1);
						p.tkf = 0; p.tkc = 0;//フラグ,カウント初期化
					}//手数計算終了

					if (frg == 14) {//エッジ検出処理
						p.gfr = 3; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 15) {//エンボス処理
						p.gfr = 4; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 16) {//水彩画風処理
						p.gfr = 5; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}
				}//移動フラグ以外の時は飛ばす
			
				mov(&p, 0, frg, 0); //移動処理
			
			stl = strlen(p.g_name) - 4;
			if (!strcmp(p.g_name + stl, ".mpg") || !strcmp(p.g_name + stl, ".avi") ||
				!strcmp(p.g_name, "z_cam_ewc.bmp")){//動画,カメラの時は常に呼び出す
				drawing_img(&p, 0, 0, 1);
			}

			frg = 0; //移動フラグ初期化

		}  //while終わり

	}//一番外のwhile終わり
	InitSoftImage();        //ソフトイメージ全開放
	DxLib_End();           // DXライブラリ終了処理
	free(p.img); return 0;//メモリ解放,終了
} // winmain終わり

//********************************************************************************************************//
//**** libjpeg　Copyright (C) 1991-2013, Thomas G. Lane, Guido Vollbeding.  ******************************//
//**** DX Library Copyright (C) 2001-2014 Takumi Yamada.   ***********************************************//
//********************************************************************************************************//