//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　画像描画関数                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include "struct_set.h"
#include "back_image.h"
#include "pic_resize.h"
#include "ewclib.h"//カメラ用ライブラリ

void drawing_img(alldata *p, int x, int y, int f){ //x,yは描画時のオフセット量, f 0:静止画更新無, 1:静止画更新有, 2,3,4:ファイル関数用(動画は常に更新)
	
	int i, j, k, i1, j1;    //for
	int rd, rd1, rd2, rd3; //ランダム
	int r, g, b;          //色
	int g2;              //動画像ハンドル
	int stl;            //画像ファイル名文字列長さ
	int **pi16;        //エッジ,エンボス処理一時保管用
	int mf = 0;       //静止画,動画判別  0:静止画,1:動画
	int camf = 0;          //カメラファイル選択 0:選択してない 1:選択している
	static int xs, ys;    //動画描画前処理用の画像サイズ
	static int buffer[320 * 240];//カメラ画像用バッファ(1要素に色情報が入ってる)
	int mono;         //モノクロ用
	int pi;          //エッジ検出用
	double ghs, gvs;//エッジ検出用
	typedef struct{//ヒストグラム作成用
		int rp;   //赤の数
		int gp;  //緑の数
		int bp; //青の数
	}his;
	static his hist[128]; //ヒストグラム各色の各階調数(2階調分で1個)
	int xx, yy;          //完成前,完成後のパズル画像オフセット
	static int ff = 0;  //完成状態  0:完成  1:未完成
	int ffr = 1;       //完成状態履歴 0:更新無し 1:更新直後
	int temp;         //値交換用
	
	para *prs = &p->paras[p->size];
	imxy *img = p->img;

	if (f == 4){ EWC_Close(0); return; }//カメラ終了処理

	stl = strlen(p->g_name) - 4;
	if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi"))mf = 1;//静止画,動画判別更新
	if (!strcmp(p->g_name, "z_cam_ewc.bmp")){ mf = 1; camf = 1;}//カメラファイル選択, 動画判別ON, カメラファイル選択ON
		
	if (mf == 1) {        //動画ファイルオープン,描画,リサイズ処理
		if (camf == 0){  //カメラファイル選択していない場合は動画ファイル処理↓
			if (GetMovieStateToGraph(p->mof) != 1){//動画再生,停止判別用ハンドルを再生用に使う(file関数での判別にも使用してる)
				p->mof = LoadGraph(p->g_name);    //画像ファイル名から画像読み込み
				GetGraphSize(p->mof, &xs, &ys);  //サイズ読み込み
				PlayMovieToGraph(p->mof);       //動画ファイルopen
			}
			DrawGraph(0, 0, p->mof, TRUE);    //動画ファイルopen後の描画の前処理用の描画                
			SaveDrawScreen(0, 0, xs, ys, "./save/save.bmp");//描画した画像の保存
			g2 = LoadSoftImage("./save/save.bmp");         //保存した画像のソフトイメージ読み込み(g2==元画像ソフトイメージ)
		}
		else {//カメラファイル選択してる場合
			g2 = MakeXRGB8ColorSoftImage(320, 240);//ハンドルにカラ画像設定

			if (EWC_GetCamera() == 0){      //カメラが接続,又はOPENされてない場合カメラOPEN処理
				EWC_Open(0, 320, 240, 30.);//カメラopen カメラ識別番号,x軸,y軸,FPS(最初だけ実行必要)
			}

			EWC_GetImage(0, buffer);//EWCLIB内部にあるフレームバッファから buffer へ画像データをコピー
			k = 0;
			for (j = 0; j < 240; j++){
				for (i = 0; i < 320; i++){
					r = buffer[k] >> 16;//各色情報(各8bit計24bit)をシフト演算で取り出す
					g = (buffer[k] - (r << 16)) >> 8;
					b = buffer[k++] - (r << 16) - (g << 8);
					DrawPixelSoftImage(g2, i, j, r, g, b, 0);//カライメージハンドルに取得した色情報を描画(g2==元画像ソフトイメージ)
				}
			}
			if (EWC_GetCamera() == 0){ DeleteGraph(g2); g2 = LoadSoftImage("z_cam_ewc.bmp"); }//カメラ未接続の場合"cam_ewc.bmp"をそのままcamに格納
		}//else終わり(カメラ処理)

		pic_resize(p, g2);//pic_resize関数実行(p.imgpiに色情報格納)
		DeleteSoftImage(g2);//g2解放

		if (f == 3 || f == 2) {//ファイル関数実行時のみ処理(ファイル関数では1枚絵で表示の為分割せずにそのまま表示)
			back_image(1);
			for (j = 0; j < p->yrs; j++){
				for (i = 0; i < p->xrs; i++){
					DrawPixel(i + (800 - p->xrs) / 2 + x, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
				}
			}
			return;
		}//ファイル関数実行時のみ処理終了

	}//動画ファイルオープン,描画,リサイズ処理終了


	if (f == 1 && mf != 1){  //静止画で更新が必要な時のみ実行する(引数fで操作,mf !=1(動画ではない時の処理))
		g2 = LoadSoftImage(p->g_name); //画像読み込み
		pic_resize(p, g2);     //pic_resize関数実行(p.imgpiに色情報格納)
		DeleteSoftImage(g2);  //ハンドルg2解放
	}

	if (f == 2 && mf != 1){             //静止画でファイル関数用の初期処理(毎回LoadSoftImageを呼ばないようにする為)
		g2 = LoadSoftImage(p->g_name); //画像読み込み
		pic_resize(p, g2);            //pic_resize関数実行(p.imgpiに色情報格納)
		DeleteSoftImage(g2);         //ハンドルg2解放
		return;
	}

	if (f == 3 && mf != 1){//静止画でファイル関数用の主処理
		back_image(1);
		for (j = 0; j < p->yrs; j++){
			for (i = 0; i < p->xrs; i++){
				DrawPixel(i + (800 - p->xrs) / 2 + x, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
			}
		}
		return;
	}
		
    //モノクロ画像処理,エッジ検出,エンボス処理(静止画,動画,どちらでも使用する)p->gfrの値はpuzzle_mainから操作している
	//mf==1動画ONでgfrが一致で実行, f==1静止画更新有りでgfr一致で実行
	if ((p->gfr == 1 || p->gfr == 3 || p->gfr == 4) && (mf == 1 || f == 1)) {
		pi16 = (int**)malloc(sizeof(int*) * p->xrs);//エッジ,エンボス処理一時保管用メモリ確保
		for (i = 0; i < p->xrs; i++){ pi16[i] = (int*)malloc(sizeof(int) * p->yrs); }

		for (j = 0; j < p->yrs; j++){
			for (i = 0; i < p->xrs; i++){
				if (p->gfr == 1){//モノクロ画像処理
					r = p->imgpi[i][j] >> 16;
					g = (p->imgpi[i][j] - (r << 16)) >> 8;
					b = p->imgpi[i][j] - (r << 16) - (g << 8);
					mono = (r + g + b) / 3;//(赤＋緑＋青)÷3
					p->imcpy[i][j] = p->imgpi[i][j] = (mono << 16) + (mono << 8) + mono;//色情報を平均化するとモノクロになる
				}//モノクロ処理終わり

				if (p->gfr == 3 || p->gfr == 4){//エッジ検出,エンボス前処理
					r = p->imgpi[i][j] >> 16;
					g = (p->imgpi[i][j] - (r << 16)) >> 8;
					b = p->imgpi[i][j] - (r << 16) - (g << 8);
					pi16[i][j] = (r + g + b) / 3;
				}//エッジ検出,エンボス前処理終わり
			}   //for終わり
		}      //for終わり
		if ((p->gfr == 3 || p->gfr == 4) && (mf == 1 || f == 1)) {//エッジ検出,エンボス後処理

			for (j = 0; j < p->yrs; j++){
				for (i = 0; i < p->xrs; i++){
					if (i == 0 || i == p->xrs - 1 || j == 0 || j == p->yrs - 1){//配列範囲外まで読んでしまうので外側は計算に含まないようにする
						p->imgpi[i][j] = (1 << 16) + (1 << 8) + 1; continue;
					}
					//水平方向ghs 
					ghs = pi16[i - 1][j - 1] * -1;     //左上,×-1が係数,計算してないピクセルは係数0
					ghs += pi16[i - 1][j] * -2;       //左
					ghs += pi16[i - 1][j + 1] * -1;  //左下
					ghs += pi16[i + 1][j - 1] * 1;  //右上
					ghs += pi16[i + 1][j] * 2;     //右
					ghs += pi16[i + 1][j + 1] * 1;//右下
					//垂直方向gvs
					gvs = pi16[i - 1][j - 1] * -1;     //左上
					gvs += pi16[i][j - 1] * -2;       //上
					gvs += pi16[i + 1][j - 1] * -1;  //右上
					gvs += pi16[i - 1][j + 1] * 1;  //左下
					gvs += pi16[i][j + 1] * 2;     //下
					gvs += pi16[i + 1][j + 1] * 1;//右下

					if (p->gfr == 3){                 //エッジ検出計算
						pi = sqrt(ghs*ghs + gvs*gvs);//sqrt平方根(pow関数でやるつもりだったがうまくいかない)
					}
					if (p->gfr == 4){         //エンボス計算
						pi = (ghs + gvs) / 2;//とりあえずこの計算方法にするとエンボスになるみたい
						pi += 128;          //灰色っぽくした方がそれらしく見える
					}
					if (pi < 1)pi = 1;
					if (pi > 255)pi = 255;
					p->imcpy[i][j] = p->imgpi[i][j] = (pi << 16) + (pi << 8) + pi;
				}//for終わり
			}   //for終わり
		}      //if エッジ,エンボス後処理終わり
		for (i = 0; i < p->yrs; i++)free(pi16[i]);//エッジ,エンボス処理一時保管用メモリ解放
		free(pi16);
	}//モノクロ,エッジ検出,エンボス処理終わり

    //モザイク処理,水彩画風処理(動画,静止画どちらでも使う)
	//gfr一致でmf==1動画ONで実行  gfr一致でf==1静止画更新有りで実行
	if ((p->gfr == 2 || p->gfr == 5) && (mf == 1 || f == 1)) {
		for (j = 0; j < p->yrs - 10; j += 10){
			for (i = 0; i < p->xrs - 10; i += 10){
				//モザイク処理
				if (p->gfr == 2){
					for (j1 = j; j1 < j + 10; j1++){
						for (i1 = i; i1 < i + 10; i1++){
							p->imcpy[i1][j1] = p->imgpi[i1][j1] = p->imgpi[i][j];//10pixel連続で同色描写でモザイクになる
						}
					}
				}
				//水彩画風処理
				if (p->gfr == 5){
					for (i1 = 0; i1 < 20; i1++){//20回ランダムで入れ替え
						rd = rand() % 10;
						rd1 = rand() % 10;
						rd2 = rand() % 10;
						rd3 = rand() % 10;
						temp = p->imgpi[i + rd][j + rd1];
						p->imcpy[i + rd][j + rd1] = p->imgpi[i + rd][j + rd1] = p->imgpi[i + rd2][j + rd3];
						p->imcpy[i + rd2][j + rd3] = p->imgpi[i + rd2][j + rd3] = temp;
					}
				}
			}
		}
	}

	//ヒストグラムデータ処理(更新必要時,実行)
	if (mf == 1 || f == 1){
		for (i = 0; i < 128; i++){  //初期化
			hist[i].rp = 0;
			hist[i].gp = 0;
			hist[i].bp = 0;
		}
		for (j = 0; j < p->yrs; j++){//各色,各階調毎に個数増加
			for (i = 0; i < p->xrs; i++){
				r = p->imgpi[i][j] >> 16;
				g = (p->imgpi[i][j] - (r << 16)) >> 8;
				b = p->imgpi[i][j] - (r << 16) - (g << 8);
				hist[r / 2].rp++;
				hist[g / 2].gp++;
				hist[b / 2].bp++;
			}
		}
	}//ヒストグラムデータ処理終了

			
    //↓毎回実行する処理(ファイル関数以外)
	ClearDrawScreen();  //画面消去
	back_image(1);     //背景描画関数,引数1==画像描画
	//パズル部以外の画像描画左側
	for (i = 0; i < (p->xrs - 400) / 2; i++){
		for (j = 0; j < 400; j++){
			DrawPixel(i + (800 - p->xrs) / 2 + 1, j + 100, p->imgpi[i][j]);//+1は補正
		}
	}  
	//パズル部以外の画像描画右側
	for (i = p->xrs - (p->xrs - 400) / 2; i < p->xrs; i++){
		for (j = 0; j < 400; j++){
			DrawPixel(i + (800 - p->xrs) / 2, j + 100, p->imgpi[i][j]);
		}
	}
    //パズル部以外の画像描画上側
	for (j = 0; j < (p->yrs - 400) / 2; j++){
		for (i = 0; i < 400; i++){
			DrawPixel(i + 200, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
		}
	}
	//パズル部以外の画像描画下側
	for (j = p->yrs - (p->yrs - 400) / 2; j < p->yrs; j++){
		for (i = 0; i < 400; i++){
			DrawPixel(i + 200, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
		}
	}
	//パズル完成判定処理,完成前完成後のオフセット処理
	for (i = 0; i < prs->idx; i++){
		if (img[i].cx != img[i].fx || img[i].cy != img[i].fy)break;
	}
	if (i == prs->idx && ff == 0)ffr = 0;               //完成,更新無し
	if (i == prs->idx && ff == 1){ ffr = 1; ff = 0; }  //完成,更新有り
	if (i != prs->idx && ff == 1)ffr = 0;             //未完成,更新無し
	if (i != prs->idx && ff == 0){ ffr = 1; ff = 1; }//未完成,更新有り

	//ブロック画像処理
	if (ff == 1 && p->size < 5){
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (p->block[i][j] == 0)continue;   //色変換無しスキップ
				//↓そのまま変換すると色が変わっていってしまうので一旦元に戻す
				p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] = p->imcpy[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2];
				//↓色取得変換
				r = (p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] >> 16);
                g = ((p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] - (r << 16)) >> 8);
			    b = (p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] - (r << 16) - (g << 8));
				r -= p->block[i][j];
				g -= p->block[i][j];
				b -= p->block[i][j];
				if (r <= 0)r = 1;
				if (g <= 0)g = 1;
				if (b <= 0)b = 1;
				p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] = GetColor(r, g, b);//色代入
			}
		}
	}//ブロック画像処理終わり

	//完成ブロック処理
	if (ff == 0 && mf == 0){    //完成済み静止画処理
		for (j = 0; j < p->yrs; j++){
			for (i = 0; i < p->xrs; i++){
				p->imgpi[i][j] = p->imcpy[i][j];//ブロック画像処理前データをコピー
			}
		}
	}

	//完成,未完成直後処理(再帰は1度だけ処理される)
	if (ffr == 1){
		if (ff == 0){//完成直後処理
			for (i = 0; i < 20; i++){
				drawing_img(p, 20 - i, -20 + i, 0);//再帰
				ScreenFlip();
			}
		}
		if (ff == 1){//未完成直後処理
			for (i = 0; i < 20; i++){
				drawing_img(p, i - 20, -i + 20, 0);//再帰(20はxx,yyの分の補正)
				ScreenFlip();
			}
		}
	}//完成,未完成直後処理終わり

	if (ff == 0){ xx = 0; yy = 0; }//完成状態でオフセット量更新
	else{ xx = 20; yy = -20; }
	
	for (i = 0; i < prs->idx; i++){ //パズル部画像描画処理
		for (j = 0; j < prs->bsize; j++){     //1ブロック内y方向
			for (k = 0; k < prs->bsize; k++){//1ブロック内x方向
							
				//表示座標位置＋オフセット, 表示色情報(完成座標位置－オフセット)
				DrawPixel(k + img[i].cx + xx + x, j + img[i].cy + yy + y,     
					p->imgpi[k + img[i].fx - 200 + (p->xrs - 400) / 2]
					[j + img[i].fy - 100 + (p->yrs - 400) / 2]);
			}
		}
	}//パズル部画像描画処理終わり

	for (i = 0; i < 128; i++){//ヒストグラム描画
		DrawLine(640 + i, 200, 640 + i, 200 - hist[i].rp / 80, GetColor(i * 2, 0, 0));
		DrawLine(640 + i, 300, 640 + i, 300 - hist[i].gp / 80, GetColor(0, i * 2, 0));
		DrawLine(640 + i, 400, 640 + i, 400 - hist[i].bp / 80, GetColor(0, 0, i * 2));
	}//ヒストグラム描画終了

	InitSoftImage();//ソフトイメージ全開放
}                 //drawing_img()終了