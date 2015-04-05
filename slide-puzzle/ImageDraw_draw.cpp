//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義(画像描画)                 　　**//
//**                                    (ImageReadの派生クラス)                          **//
//*****************************************************************************************//

#include "DxLib.h"
#include <math.h>
#include "back_image.h"
#include "Move.h"
#include "ImageRead.h"
#include "ImageDraw.h" 
#include "Filter.h"

void ImageDraw::draw(Filter *filter, Move *move, int x, int y){//画像変換,描画処理  drawing_img内から呼び出される(オーバーライド)

	int i, j, k, i1, j1; //for
	int r, g, b;        //色
	int **pi16;        //エッジ,エンボス処理一時保管用
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
	int d3_im, d3_z;  //3D用一時格納用

	if (move == NULL) {//NULLの場合画像処理無し描画
		back_image(1);
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				DrawPixel(i + (800 - xrs) / 2 + x, j + (600 - yrs) / 2, imgpi[j][i]);
			}
		}
		return;
	}//ファイル関数実行時のみ処理終了

	/********************************************************************************************************************************/
	/********************************************各種画像処理開始********************************************************************/
	/********************************************************************************************************************************/

	//モノクロ画像処理,エッジ検出,エンボス,画像エンボス処理,絵画風処理(静止画,動画,どちらでも使用する)gfrの値はpuzzle_mainから操作している
	if (d.gfr == 1 || d.gfr == 3 || d.gfr == 4 || d.gfr == 5 || d.gfr == 8 || d.gfr == 9) {
		pi16 = (int**)malloc(sizeof(int*) * yrs);//エッジ,エンボス,画像エンボス処理一時保管用メモリ確保
		for (i = 0; i < yrs; i++){ pi16[i] = (int*)malloc(sizeof(int) * xrs); }

		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				if (d.gfr == 1){//モノクロ画像処理
					r = imgpi[j][i] >> 16 & 0xff;//0xは16進数 0xffで下8ビット &論理積
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					mono = (r + g + b) / 3;//(赤＋緑＋青)÷3
					imcpy[j][i] = imgpi[j][i] = (mono << 16) + (mono << 8) + mono;//色情報を平均化するとモノクロになる
				}//モノクロ処理終わり

				if (d.gfr == 5){//絵画風処理
					r = (imgpi[j][i] >> 16 & 0xff) / 60 * 60;
					g = (imgpi[j][i] >> 8 & 0xff) / 60 * 60;
					b = (imgpi[j][i] & 0xff) / 60 * 60;
					imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfr == 8){//ネガポジ処理
					r = 255 - (imgpi[j][i] >> 16 & 0xff);
					g = 255 - (imgpi[j][i] >> 8 & 0xff);
					b = 255 - (imgpi[j][i] & 0xff);
					imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfr == 3 || d.gfr == 4 || d.gfr == 9){//エッジ検出,エンボス,画像エンボス前処理
					r = imgpi[j][i] >> 16 & 0xff;
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					pi16[j][i] = (r + g + b) / 3;
				}//エッジ検出,エンボス前処理終わり

			}   //for終わり
		}      //for終わり

		if (d.gfr == 3 || d.gfr == 4 || d.gfr == 9) {//エッジ検出,エンボス,画像エンボス後処理

			if (d.gfr == 9){    //画像エンボス用画像更新,音量調整
                read->drawing_img(NULL, NULL, 0, 0, 0); //画像更新
				if (read->d.mcf == 1)read->pBasicAudio->put_Volume(-2000);//音量調整,動画以外だとまだグラフ未生成
			}

			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					if (i == 0 || i == xrs - 1 || j == 0 || j == yrs - 1){//配列範囲外まで読んでしまうので外側は計算に含まないようにする
						imgpi[j][i] = (1 << 16) + (1 << 8) + 1; continue;
					}
					//水平方向ghs 
					ghs = pi16[j - 1][i - 1] * -1;     //左上,×-1が係数,計算してないピクセルは係数0
					ghs += pi16[j][i - 1] * -2;       //左
					ghs += pi16[j + 1][i - 1] * -1;  //左下
					ghs += pi16[j - 1][i + 1] * 1;  //右上
					ghs += pi16[j][i + 1] * 2;     //右
					ghs += pi16[j + 1][i + 1] * 1;//右下
					//垂直方向gvs
					gvs = pi16[j - 1][i - 1] * -1;     //左上
					gvs += pi16[j - 1][i] * -2;       //上
					gvs += pi16[j - 1][i + 1] * -1;  //右上
					gvs += pi16[j + 1][i - 1] * 1;  //左下
					gvs += pi16[j + 1][i] * 2;     //下
					gvs += pi16[j + 1][i + 1] * 1;//右下

					if (d.gfr == 3){                  //エッジ検出計算
						pi = (int)(sqrt(ghs*ghs + gvs*gvs));//sqrt平方根(pow関数でやるつもりだったがうまくいかない)
						if (pi < 50)pi = 0;         //背景色(黒)を0にする
						if (pi > 255)pi = 255;
						imcpy[j][i] = imgpi[j][i] = (pi << 16) + (pi << 8) + pi;
					}

					if (d.gfr == 4 || d.gfr == 9){  //エンボス,画像エンボス計算
						pi = (int)((ghs + gvs) * 0.5);//とりあえずこの計算方法にするとエンボスになるみたい
						pi += 128;          //灰色っぽくした方がそれらしく見える
						if (pi < 1)pi = 1;
						if (pi > 255)pi = 255;
						imcpy[j][i] = imgpi[j][i] = (pi << 16) + (pi << 8) + pi;

						if (d.gfr == 9){//画像エンボス
							pi -= 128;
							if (xrs == read->xrs && yrs == read->yrs){//サイズ同じ
								r = (read->imgpi[j][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i] & 0xff) - pi;
							}

							if (xrs > read->xrs && yrs == read->yrs){//元画像の方がx大きい,y同じ
								if (i <= (xrs - read->xrs) * 0.5 || i >= (xrs - read->xrs) * 0.5 + read->xrs)continue;
								r = (read->imgpi[j][i - (xrs - read->xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i - (xrs - read->xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i - (xrs - read->xrs) / 2] & 0xff) - pi;
							}

							if (xrs < read->xrs && yrs == read->yrs){//元画像の方がx小さい,y同じ
								r = (read->imgpi[j][i + (read->xrs - xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i + (read->xrs - xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i + (read->xrs - xrs) / 2] & 0xff) - pi;
							}

							if (yrs > read->yrs && xrs == read->xrs){//元画像の方がy大きい,x同じ
								if (j <= (yrs - read->yrs) * 0.5 || j >= (yrs - read->yrs) * 0.5 + read->yrs)continue;
								r = (read->imgpi[j - (yrs - read->yrs) / 2][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j - (yrs - read->yrs) / 2][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j - (yrs - read->yrs) / 2][i] & 0xff) - pi;
							}

							if (yrs < read->yrs && xrs == read->xrs){//元画像の方がy小さい,x同じ
								r = (read->imgpi[j + (read->yrs - yrs) / 2][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j + (read->yrs - yrs) / 2][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j + (read->yrs - yrs) / 2][i] & 0xff) - pi;
							}

							if (yrs < read->yrs && xrs > read->xrs){//元画像の方がy小さい,元画像の方がx大きい
								if (i <= (xrs - read->xrs) * 0.5 || i >= (xrs - read->xrs) * 0.5 + read->xrs)continue;
								r = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] & 0xff) - pi;
							}

							if (yrs > read->yrs && xrs < read->xrs){//元画像の方がy大きい,元画像の方がx小さい
								if (j <= (yrs - read->yrs) * 0.5 || j >= (yrs - read->yrs) * 0.5 + read->yrs)continue;
								r = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] & 0xff) - pi;
							}

							if (r < 1)r = 1;
							if (r > 255)r = 255;
							if (g < 1)g = 1;
							if (g > 255)g = 255;
							if (b < 1)b = 1;
							if (b > 255)b = 255;
							imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;//画像エンボス計算終了
						}
					}

				}//for終わり
			}   //for終わり
		}      //if エッジ,エンボス後処理終わり
		for (i = 0; i < yrs; i++)free(pi16[i]);//エッジ,エンボス処理一時保管用メモリ解放
		free(pi16);
	}//モノクロ,エッジ検出,エンボス処理終わり

	//モザイク処理(動画,静止画どちらでも使う)
	//gfr一致でmf==1動画ONで実行  gfr一致でf==1静止画更新有りで実行
	if (d.gfr == 2) {
		for (j = 0; j < yrs - 10; j += 10){
			for (i = 0; i < xrs - 10; i += 10){
				for (j1 = j; j1 < j + 10; j1++){
					for (i1 = i; i1 < i + 10; i1++){
						imcpy[j1][i1] = imgpi[j1][i1] = imgpi[j][i];//10pixel連続で同色描写でモザイクになる
					}
				}
			}
		}
	}//モザイク処理終わり

	/********************************************************************************************************************************/
	/********************************************各種画像処理終了********************************************************************/
	/********************************************************************************************************************************/

	//ヒストグラムデータ処理
	for (i = 0; i < 128; i++){  //初期化
		hist[i].rp = 0;
		hist[i].gp = 0;
		hist[i].bp = 0;
	}
	for (j = 0; j < yrs; j++){//各色,各階調毎に個数増加
		for (i = 0; i < xrs; i++){
			r = imgpi[j][i] >> 16 & 0xff;
			g = imgpi[j][i] >> 8 & 0xff;
			b = imgpi[j][i] & 0xff;
			hist[r / 2].rp++;
			hist[g / 2].gp++;
			hist[b / 2].bp++;
		}
	}
	//ヒストグラムデータ処理終了

	/********************************************************************************************************************************/
	/********************************************最終画像描画処理開始****************************************************************/
	/********************************************************************************************************************************/

	Move::para *prs = &move->paras[move->size];
	Move::imxy *img = move->img;

	if (d.d3f == 1 || d.d3f == 2){
		//カメラ位置更新
		CameraPos.x = 400.0f + sin(d.theta_lr * 3.14 / 180) * 550;
		CameraPos.z = -550.0f + 550 - cos(d.theta_lr * 3.14 / 180) * 550;

		// カメラの位置と注視点をセット
		SetCameraPositionAndTarget_UpVecY(CameraPos, VGet(400.0f, 250.0f, 0.0f));
	}

	//↓毎回実行する処理(ファイル関数以外)
	ClearDrawScreen();  //画面消去
	back_image(1);     //背景描画関数,引数1==画像描画
	//パズル部以外の画像描画左側
	for (j = 0; j < 400; j++){
		for (i = 0; i < (xrs - 400) * 0.5; i++){
			//ifエッジ検出時の背景色描画スキップ処理以下同じ処理有り
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + (800 - xrs) * 0.5 + 1, j + 100, imgpi[j][i]);//+1は補正
		}
	}
	//パズル部以外の画像描画右側
	for (j = 0; j < 400; j++){
		for (i = xrs - (xrs - 400) * 0.5; i < xrs; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + (800 - xrs) * 0.5 - 1, j + 100, imgpi[j][i]);
		}
	}
	//パズル部以外の画像描画上側
	for (j = 0; j < (yrs - 400) * 0.5; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + 200, j + (600 - yrs) * 0.5, imgpi[j][i]);
		}
	}
	//パズル部以外の画像描画下側
	for (j = yrs - (yrs - 400) * 0.5; j < yrs; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + 200, j + (600 - yrs) * 0.5, imgpi[j][i]);
		}
	}
	//パズル間の画像描画
	if (move->size != 5){//size5は処理しない
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (d.gfr == 3 && imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] == 0)continue;
				if (filter->block[j][i] == 150 || filter->block[j][i] == 1){//フィルター値150,1のみ描画
					DrawPixel(i + 200, j + 100, imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2]);
				}
			}
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
	if (ff == 1 && move->size < 5){
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (filter->block[j][i] == 0)continue;   //色変換無しスキップ
				//↓そのまま変換すると色が変わっていってしまうので一旦元に戻す
				imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] = imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2];
				//↓色取得変換
				r = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff;
				g = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff;
				b = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff;
				r -= filter->block[j][i];
				g -= filter->block[j][i];
				b -= filter->block[j][i];
				if (r <= 0)r = 1;
				if (g <= 0)g = 1;
				if (b <= 0)b = 1;
				imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] = GetColor(r, g, b);//色代入
			}
		}
	}//ブロック画像処理終わり

	//完成ブロック処理
	if (ff == 0 && d.mcf == 0){    //完成済み静止画処理
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				if (d.gfr == 3 && imgpi[j][i] == 0)continue;
				imgpi[j][i] = imcpy[j][i];//ブロック画像処理前データをコピー
			}
		}
	}

	//完成,未完成直後処理(再帰は1度だけ処理される)
	if (ffr == 1){
		if (ff == 0){//完成直後処理
			for (i = 0; i < 19; i++){
				drawing_img(filter, move, 19 - i, -19 + i, 0);//再帰
				ScreenFlip();
			}
		}
		if (ff == 1){//未完成直後処理
			for (i = 0; i < 19; i++){
				drawing_img(filter, move, i - 19, -i + 19, 0);//再帰(19はxx,yyの分の補正)
				ScreenFlip();
			}
		}
	}//完成,未完成直後処理終わり

	if (ff == 0){ xx = 0; yy = 0; }//完成状態でオフセット量更新
	else{ xx = 19; yy = -19; }

	//パズル部画像描画処理開始
	for (i = 0; i < prs->idx; i++){
		for (j = 0; j < prs->bsize; j++){     //1ブロック内y方向
			for (k = 0; k < prs->bsize; k++){//1ブロック内x方向

				//エッジ検出処理
				if (d.gfr == 3 && imgpi[j + img[i].fy - 100 + (yrs - 400) / 2]
					[k + img[i].fx - 200 + (xrs - 400) / 2] == 0)continue;

				//ブロック間は描画しない(フィルター値150,1 size==5)
				if ((filter->block[j + img[i].fy - 100][k + img[i].fx - 200] == 150 ||
					filter->block[j + img[i].fy - 100][k + img[i].fx - 200] == 1) &&
					move->size != 5)continue;

				if (d.d3f == 1 || d.d3f == 2){
					//3D表示
					d3_im = imgpi[j + img[i].fy - 100 + (yrs - 400) / 2]
						[k + img[i].fx - 200 + (xrs - 400) / 2];
					if (d.d3f == 2)d3_z = ((d3_im >> 16) & 0xff + (d3_im >> 8) & 0xff + d3_im & 0xff) / 6;
					else d3_z = 0;
					DrawLine3D(VGet(k + img[i].cx + xx + x, 550 - (j + img[i].cy + yy + y),
						       d3_z + 20),
						       VGet(k + img[i].cx + xx + x, 550 - (j + img[i].cy + yy + y),
						       -(d3_z + 20)),
						       d3_im);
				}
				else{
					//通常表示,表示座標位置＋オフセット, 表示色情報(完成座標位置－オフセット)
					DrawPixel(k + img[i].cx + xx + x, j + img[i].cy + yy + y,
						imgpi[j + img[i].fy - 100 + (yrs - 400) / 2]
						[k + img[i].fx - 200 + (xrs - 400) / 2]);
				}

			}
		}
	}//パズル部画像描画処理終わり

	for (i = 0; i < 128; i++){//ヒストグラム描画
		DrawLine(640 + i, 200, 640 + i, 200 - hist[i].rp / 80, GetColor(i * 2, 0, 0));
		DrawLine(640 + i, 300, 640 + i, 300 - hist[i].gp / 80, GetColor(0, i * 2, 0));
		DrawLine(640 + i, 400, 640 + i, 400 - hist[i].bp / 80, GetColor(0, 0, i * 2));
	}//ヒストグラム描画終了

	//完成例画像描画
	if (finish == 1){
		for (j = 0; j < 200; j++){
			for (i = 0; i < 200; i++){
				if (d.gfr == 3 && imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2] == 0)continue;
				DrawPixel(i + 600, j + 400, imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2]);
			}
		}
	}
	/********************************************************************************************************************************/
	/********************************************最終画像描画処理終了****************************************************************/
	/********************************************************************************************************************************/

}

void ImageDraw::obj_create(char *name){//オブジェクト生成関数

	if (read == NULL){//オブジェクト生成されていない時のみ実行
		read = new ImageRead(name);//画像エンボス用オブジェクト生成
	}
}

void ImageDraw::obj_delete(){//オブジェクト破棄関数

	if (read != NULL){//オブジェクト生成されている時のみ実行
		read->d.cap.release();         //カメラ終了処理
		delete read;                  //オブジェクト破棄
		read = NULL;
	}
}

ImageDraw::ImageDraw(){}//規定コンストラクタ

ImageDraw::ImageDraw(char *name):ImageRead(name){

	read = NULL;
	back_image(0);//背景描画関数,引数0==画像をハンドルに格納
}

ImageDraw::~ImageDraw(){//デストラクタ

	obj_delete();//オブジェクト破棄関数

}