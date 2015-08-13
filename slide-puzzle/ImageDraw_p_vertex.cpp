//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義(パズル画像頂点)               **//
//**                                    (ImageReadの派生クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <math.h>
#include "back_image.h"
#include "Move.h"
#include "ImageRead.h"
#include "ImageDraw.h" 

HRESULT ImageDraw::p_vertex(Dx9Init *dx, Move *move, int x, int y, int z){  //頂点処理

	if (pMyVB == NULL)dx->getVBarray(this, d3);

	int i, j, k, k1, k2;//for
	int r, g, b;       //色
	typedef struct{//ヒストグラム作成用
		int rp;   //赤の数
		int gp;  //緑の数
		int bp; //青の数
	}his;
	static his hist[128]; //ヒストグラム各色の各階調数(2階調分で1個)
	int xx, yy, zz;      //完成前,完成後のパズル画像オフセット
	static int ff = 0;  //完成状態  0:完成  1:未完成
	int ffr = 1;       //完成状態履歴 0:更新無し 1:更新直後
	DWORD cl;         //1ピクセル色一時保管
	float i2, j2, z2;//1ピクセル座標一時保管
	float pz;       //1ピクセルのZ座標
	int bf;        //残バッファ処理

	//スクリーンクリア 
	if (FAILED(dx->sclear()))return E_FAIL;

	//カメラ位置更新
	cpx = xrs * 0.5f + sin(theta_lr * 3.14f / 180.0f) * 750.0f;
	cpz = -cos(theta_lr * 3.14f / 180.0f) * 750.0f;

	// カメラの位置と方向を設定
	dx->cameraset(xrs, yrs, cpx, cpz);

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
	int df;
	if (d3 != 1)df = 8; else df = 4;
	//↓毎回実行する処理(ファイル関数以外)
	back_image(dx, 1);     //背景描画関数,引数1==画像描画
	k1 = 0;

	//パズル部以外の画像描画左側
	int yr = (int)((yrs - 400) * 0.5);
	int xr = (int)((xrs - 400) * 0.5);
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = 0; j < 400; j++){
			for (i = 0; i < xr; i++){
				//エッジ検出時
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z方向色情報による
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//ピクセル間隙間埋め
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = 0; j < 400; j++){
			for (i = 0; i < xr; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//パズル部以外の画像頂点右側
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = 0; j < 400; j++){
			for (i = xrs - xr; i < xrs; i++){
				//エッジ検出時
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z方向色情報による
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//ピクセル間隙間埋め
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = 0; j < 400; j++){
			for (i = xrs - xr; i < xrs; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//パズル部以外の画像頂点上側
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = 0; j < yr; j++){
			for (i = 0; i < 400; i++){
				//エッジ検出時
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z方向色情報による
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//ピクセル間隙間埋め
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = 0; j < yr; j++){
			for (i = 0; i < 400; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//パズル部以外の画像頂点下側
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = yrs - yr; j < yrs; j++){
			for (i = 0; i < 400; i++){
				//エッジ検出時
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z方向色情報による
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//ピクセル間隙間埋め
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = yrs - yr; j < yrs; j++){
			for (i = 0; i < 400; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//パズル間の画像頂点
	if (move->size != 5){//size5は処理しない
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				//エッジ検出時
				if (d.gfl == 3 && imgpi[j + yr][i + xr] == 0){
					img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					img_a[k1++].color = 0;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
					}
					continue;
				}

				if (block[j][i] == 150 || block[j][i] == 1){//フィルター値150,1のみ描画
					if (d3 == 1 || d3 == 2)pz = 20.0f;
					if (d3 == 3)pz = ((imgpi[j + yr][i + xr] >> 16 & 0xff) +
						(imgpi[j + yr][i + xr] >> 8 & 0xff) +
						(imgpi[j + yr][i + xr] & 0xff)) / 9.0f;//Z方向色情報による
					img_a[k1].p = D3DXVECTOR3((float)i + xr, (float)j + yr, pz);
					img_a[k1++].color = imgpi[j + yr][i + xr];
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i + xr, (float)j + yr, -pz - 15.0f);
						img_a[k1++].color = imgpi[j + yr][i + xr];
					}
				}
			}
		}
	}

	//パズル完成判定処理,完成前完成後のオフセット処理
	for (i = 0; i < prs->idx; i++){
		if (img[i].cx != img[i].fx || img[i].cy != img[i].fy || img[i].cz != 0)break;
	}
	if (i == prs->idx && ff == 0)ffr = 0;               //完成,更新無し
	if (i == prs->idx && ff == 1){ ffr = 1; ff = 0; }  //完成,更新有り
	if (i != prs->idx && ff == 1)ffr = 0;             //未完成,更新無し
	if (i != prs->idx && ff == 0){ ffr = 1; ff = 1; }//未完成,更新有り

	//ブロック画像処理
	if (ff == 1 && move->size < 5){
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (block[j][i] == 0)continue;   //色変換無しスキップ
				//↓色取得変換
				r = imgpi[j + yr][i + xr] >> 16 & 0xff;
				g = imgpi[j + yr][i + xr] >> 8 & 0xff;
				b = imgpi[j + yr][i + xr] & 0xff;
				r -= block[j][i];
				g -= block[j][i];
				b -= block[j][i];
				if (r <= 0)r = 1;
				if (g <= 0)g = 1;
				if (b <= 0)b = 1;
				imgpi[j + yr][i + xr] = (r << 16) + (g << 8) + b;//色代入
			}
		}
	}//ブロック画像処理終わり

	if (ff == 0){ xx = 0; yy = 0, zz = 0; }//完成状態でオフセット量更新
	else{ xx = 19; yy = 19, zz = -57; }

	//パズル部画像頂点処理開始
	for (k = 0; k < prs->idx; k++){
		for (j = 0; j < prs->bsize; j++){     //1ブロック内y方向
			for (i = 0; i < prs->bsize; i++){//1ブロック内x方向

				//エッジ検出処理
				if (d.gfl == 3 && imgpi[j + img[k].fy + yr]
					[i + img[k].fx + xr] == 0){
					img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					img_a[k1++].color = 0;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
					}

					//ピクセル間隙間埋め
					if (i + 1 == prs->bsize && j + 1 != prs->bsize){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}

					if (i + 1 != prs->bsize && j + 1 != prs->bsize){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}

					if (i + 1 != prs->bsize && j + 1 == prs->bsize){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				//ブロック間は描画しない(フィルター値150,1 size==5)
				if ((block[j + img[k].fy][i + img[k].fx] == 150 ||
					block[j + img[k].fy][i + img[k].fx] == 1) &&
					move->size != 5)continue;

				//通常表示,表示座標位置＋オフセット, 表示色情報(完成座標位置－オフセット)
				cl = imgpi[j + img[k].fy + yr][i + img[k].fx + xr];

				j2 = (float)j + img[k].cy + yr + yy + y;
				i2 = (float)i + img[k].cx + xr + xx + x;
				z2 = (float)img[k].cz + zz + z;

				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j + img[k].fy + yr][i + img[k].fx + xr] >> 16 & 0xff) +
					(imgpi[j + img[k].fy + yr][i + img[k].fx + xr] >> 8 & 0xff) +
					(imgpi[j + img[k].fy + yr][i + img[k].fx + xr] & 0xff)) / 9.0f;//Z方向色情報による

				img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2, pz + z2);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2, -pz - 15.0f + z2);
					img_a[k1++].color = cl;
				}

				//ピクセル間隙間埋め
				if (i + 1 == prs->bsize && j + 1 != prs->bsize){
					img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2 + 0.5f, pz + z2);
					img_a[k1++].color = cl;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2 + 0.5f, -pz - 15.0f + z2);
						img_a[k1++].color = cl;
					}
				}

				if (i + 1 != prs->bsize && j + 1 != prs->bsize){
					img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2 + 0.5f, pz + z2);
					img_a[k1++].color = cl;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2 + 0.5f, -pz - 15.0f + z2);
						img_a[k1++].color = cl;
					}
				}

				if (i + 1 != prs->bsize && j + 1 == prs->bsize){
					img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2, pz + z2);
					img_a[k1++].color = cl;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2, -pz - 15.0f + z2);
						img_a[k1++].color = cl;
					}
				}
			}
		}
	}//パズル部画像頂点処理終わり

	//残頂点データ処理(個数変更の際使用頂点データ数に差異が出るので残りが表示される為(´-ω-`))
	if (d3 == 1)bf = yrs * xrs * 4;//頂点数算出
	else bf = yrs * xrs * 8;
	do{
		img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//残頂点データ0埋め
		img_a[k1++].color = 0;
	} while (k1 < bf);

	dx->d3primitive(this, d3);//3D描画

	//完成,未完成直後処理(再帰は1度だけ処理される)
	if (ffr == 1){
		if (ff == 0){//完成直後処理
			for (i = 0; i < 19; i++){
				draw_img(dx, move, 19 - i, 19 - i, -57 + i * 3);//再帰
				dx->drawscreen();//描画
			}
		}
		if (ff == 1){//未完成直後処理
			for (i = 0; i < 19; i++){
				draw_img(dx, move, i - 19, i - 19, 57 - i * 3);//再帰(19はxx,yyの分の補正)
				dx->drawscreen();//描画
			}
		}
	}//完成,未完成直後処理終わり

	//ヒストグラム頂点
	pLine->Begin();
	for (i = 0; i < 128; i++){//ヒストグラム頂点

		hi[0] = D3DXVECTOR2(640.0f + i, 200.0f - hist[i].rp / 80.0f);
		hi[1] = D3DXVECTOR2(640.0f + i, 200.0f);
		pLine->Draw(hi, 2, D3DCOLOR_ARGB(255, i * 2, 0, 0));

		hi[0] = D3DXVECTOR2(640.0f + i, 300.0f - hist[i].gp / 80.0f);
		hi[1] = D3DXVECTOR2(640.0f + i, 300.0f);
		pLine->Draw(hi, 2, D3DCOLOR_ARGB(255, 0, i * 2, 0));

		hi[0] = D3DXVECTOR2(640.0f + i, 400.0f - hist[i].bp / 80.0f);
		hi[1] = D3DXVECTOR2(640.0f + i, 400.0f);
		pLine->Draw(hi, 2, D3DCOLOR_ARGB(255, 0, 0, i * 2));
	}
	pLine->End();
	//ヒストグラム頂点終了

	//完成例画像頂点
	if (finish == true){
		k1 = 0;
		fimg = (Dx9Init::MY_VERTEX2*)malloc(sizeof(Dx9Init::MY_VERTEX2) * 200 * 200);
		for (j = 0; j < 200; j++){
			for (i = 0; i < 200; i++){
				if (d.gfl == 3 && imgpi[yr + j * 2][xr + i * 2] == 0)continue;
				fimg[k1].x = 590.0f + i;
				fimg[k1].y = 360.0f + (199.0f - j);
				fimg[k1].z = 0.0f;
				fimg[k1].rhw = 0.0f;
				fimg[k1].color = imgpi[yr + j * 2][xr + i * 2];
				fimg[k1].tu = 0.0f;
				fimg[k1++].tv = 0.0f;
			}
		}
		dx->d2primitive(fimg);//2D描画
		free(fimg);
	}
	ver = 1;//初期描画完了
	/********************************************************************************************************************************/
	/********************************************最終画像描画処理終了****************************************************************/
	/********************************************************************************************************************************/
	return S_OK;
}

