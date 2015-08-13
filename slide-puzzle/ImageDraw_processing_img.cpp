//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(画像変換)                     **//
//**                                    (ImageDrawの基底クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"

HRESULT ImageDraw::processing_img(){ //画像変換

	int i, j, i1, j1, k; //for
	int temp;
	int r, g, b;        //色
	int mono;          //モノクロ用
	int pi;           //エッジ検出用
	double ghs, gvs; //エッジ検出用
	static int kr = 1;//スリットスキャン出力ページ
	int krs;         //スリットスキャン更新ページスタート位置
	int noise[9];   //ノイズ除去一時格納

	/********************************************************************************************************************************/
	/********************************************各種画像処理開始********************************************************************/
	/********************************************************************************************************************************/

	//エッジ,エンボス,画像エンボス,ノイズ除去処理一時保管用メモリ確保
	if (pi16 == NULL && (d.gfl == 3 || d.gfl == 4 || d.gfl == 9 || d.gfl == 12)){
		pi16 = (int**)malloc(sizeof(int*) * yrs);
		for (i = 0; i < yrs; i++){ pi16[i] = (int*)malloc(sizeof(int) * xrs); }
	}
	//エッジ,エンボス,画像エンボス,ノイズ除去処理一時保管用メモリ解放
	if (pi16 != NULL && (d.gfl != 3 && d.gfl != 4 && d.gfl != 9 && d.gfl != 12)){
		for (i = 0; i < yrs; i++)free(pi16[i]);
		free(pi16);
		pi16 = NULL;
	}

	//スリットスキャン用メモリ確保
	if (slit == NULL && d.gfl == 11){
		int kyrs = (int)(yrs * 0.5);
		slit = (int*)malloc(sizeof(int) * kyrs * yrs * xrs);//メモリ確保
		for (k = 0; k < kyrs; k++){
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					slit[k * yrs * xrs + j * xrs + i] = imgpi[j][i];//初期化
				}
			}
		}
	}
	//スリットスキャン用メモリ解放
	if (slit != NULL && d.gfl != 11){ free(slit); slit = NULL; }

	//モノクロ画像処理,エッジ検出,エンボス,画像エンボス処理,絵画風処理(静止画,動画,どちらでも使用する)
	if (d.gfl == 1 || d.gfl == 3 || d.gfl == 4 || d.gfl == 5 || d.gfl == 8 || d.gfl == 9) {

		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				if (d.gfl == 1){//モノクロ画像処理
					r = imgpi[j][i] >> 16 & 0xff;//0xは16進数 0xffで下8ビット &論理積
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					mono = (r + g + b) / 3;//(赤＋緑＋青)÷3
					imgpi[j][i] = (mono << 16) + (mono << 8) + mono;//色情報を平均化するとモノクロになる
				}//モノクロ処理終わり

				if (d.gfl == 5){//絵画風処理
					r = (imgpi[j][i] >> 16 & 0xff) / 60 * 60;
					g = (imgpi[j][i] >> 8 & 0xff) / 60 * 60;
					b = (imgpi[j][i] & 0xff) / 60 * 60;
					imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfl == 8){//ネガポジ処理
					r = 255 - (imgpi[j][i] >> 16 & 0xff);
					g = 255 - (imgpi[j][i] >> 8 & 0xff);
					b = 255 - (imgpi[j][i] & 0xff);
					imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfl == 3 || d.gfl == 4 || d.gfl == 9){//エッジ検出,エンボス,画像エンボス前処理
					r = imgpi[j][i] >> 16 & 0xff;
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					pi16[j][i] = (r + g + b) / 3;
				}//エッジ検出,エンボス前処理終わり

			}   //for終わり
		}      //for終わり

		if (d.gfl == 3 || d.gfl == 4 || d.gfl == 9) {//エッジ検出,エンボス,画像エンボス後処理

			if (d.gfl == 9){       //画像エンボス用画像更新,音量調整
				read->read_img(); //画像更新
				if (read->getmcf() == 1)read->putVol(-2000);//音量調整,動画以外だとまだグラフ未生成
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

					if (d.gfl == 3){                  //エッジ検出計算
						pi = (int)(sqrt(ghs*ghs + gvs*gvs));//sqrt平方根(pow関数でやるつもりだったがうまくいかない)
						if (pi < 50)pi = 0;         //背景色(黒)を0にする
						if (pi > 255)pi = 255;
						imgpi[j][i] = (pi << 16) + (pi << 8) + pi;
					}

					if (d.gfl == 4 || d.gfl == 9){  //エンボス,画像エンボス計算
						pi = (int)((ghs + gvs) * 0.5);//とりあえずこの計算方法にするとエンボスになるみたい
						pi += 128;          //灰色っぽくした方がそれらしく見える
						if (pi < 1)pi = 1;
						if (pi > 255)pi = 255;
						imgpi[j][i] = (pi << 16) + (pi << 8) + pi;

						if (d.gfl == 9){//画像エンボス
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
							imgpi[j][i] = (r << 16) + (g << 8) + b;//画像エンボス計算終了
						}
					}

				}//for終わり
			}   //for終わり
		}      //if エッジ,エンボス後処理終わり

	}//モノクロ,エッジ検出,エンボス処理,絵画風処理終わり

	//モザイク処理(動画,静止画どちらでも使う)
	if (d.gfl == 2) {
		for (j = 0; j < yrs - 10; j += 10){
			for (i = 0; i < xrs - 10; i += 10){
				for (j1 = j; j1 < j + 10; j1++){
					for (i1 = i; i1 < i + 10; i1++){
						imgpi[j1][i1] = imgpi[j][i];//10pixel連続で同色描写でモザイクになる
					}
				}
			}
		}
	}//モザイク処理終わり

	//スリットスキャン処理(動画,カメラのみ処理,静止画の場合はメニューに出ない)
	if (d.gfl == 11){
		int kyrs = (int)(yrs * 0.5);
		krs = kr;//更新ページ
		for (j = 0; j < yrs; j += 2){
			krs = krs%kyrs + 1;
			for (i = 0; i < xrs; i++){
				slit[(krs - 1) * yrs * xrs + j * xrs + i] = imgpi[j][i];//スリットスキャン一時保管更新
				if (j < yrs - 1){
					slit[(krs - 1) * yrs * xrs + (j + 1) * xrs + i] = imgpi[j][i];//スリットスキャン一時保管更
				}
			}
		}

		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				imgpi[j][i] = slit[(kr - 1) * yrs * xrs + j * xrs + i];//出力画像更新
			}
		}
		kr = kr%kyrs + 1;
	}

	//ノイズ除去
	if (d.gfl == 12){
		for (j = 1; j < yrs - 1; j++){
			for (i = 1; i < xrs - 1; i++){
				noise[0] = imgpi[j - 1][i - 1];    //左上
				noise[1] = imgpi[j - 1][i];       //上
				noise[2] = imgpi[j - 1][i + 1];  //右上
				noise[3] = imgpi[j][i - 1];     //左
				noise[4] = imgpi[j][i];        //真ん中
				noise[5] = imgpi[j][i + 1];       //右
				noise[6] = imgpi[j + 1][i - 1];  //左下
				noise[7] = imgpi[j + 1][i];     //下
				noise[8] = imgpi[j + 1][i + 1];//右下

				//3*3領域並び替えで中間の要素を確定,格納
				for (j1 = 8; j1 >= 6; j1--){
					for (i1 = 0; i1 < j1; i1++){
						if (noise[i1] > noise[i1 + 1]){
							temp = noise[i1];
							noise[i1] = noise[i1 + 1];
							noise[i1 + 1] = temp;
						}
					}
				}
				pi16[j][i] = noise[5];//並び替え後の中央値を一時格納
			}
		}
		//コピー
		for (j = 1; j < yrs - 1; j++){
			for (i = 1; i < xrs; i++){
				imgpi[j][i] = pi16[j][i];
			}
		}
	}

	return S_OK;
}