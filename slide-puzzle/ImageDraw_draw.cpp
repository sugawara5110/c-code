//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義(画像描画)                 　　**//
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
#include "Filter.h"

// 頂点データのＦＶＦフォーマットの設定
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

HRESULT ImageDraw::draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z){//画像変換,描画処理  drawing_img内から呼び出される(オーバーライド)

	int i, j, k, k1, i1, j1;//for
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
	int xx, yy, zz;      //完成前,完成後のパズル画像オフセット
	static int ff = 0;  //完成状態  0:完成  1:未完成
	int ffr = 1;       //完成状態履歴 0:更新無し 1:更新直後
	DWORD cl;         //1ピクセル色一時保管
	float i2, j2, z2;//1ピクセル座標一時保管
	float pz;       //1ピクセルのZ座標

	// ビューポートと深度バッファのクリアとステンシルバッファの削除
	if (FAILED(dx->pD3DDevice->Clear(0, NULL,   // クリアする領域は全面
		D3DCLEAR_TARGET |			// バックバッファを指定
		D3DCLEAR_ZBUFFER, 		   // 深度バッファ（Zバッファ）を指定
		D3DCOLOR_XRGB(0, 0, 255), // クリアする色
		1.0f, 					 // クリアする深度バッファ（Zバッファ）の値
		0)))				    // クリアするステンシルバッファの値
	{
		return E_FAIL;
	}

	//カメラ位置更新
	cpx = xrs * 0.5f + sin(theta_lr * 3.14f / 180.0f) * 750.0f;
	cpz = -cos(theta_lr * 3.14f / 180.0f) * 750.0f;

	// カメラの位置と方向を設定
	D3DXMatrixLookAtLH(&m_view,
		&D3DXVECTOR3(cpx, yrs * 0.5f, cpz),          //カメラの位置
		&D3DXVECTOR3(xrs * 0.5f, yrs * 0.5f, 0.0f),//カメラの方向を向ける点
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));          //カメラの上の方向

	if (d3 == 1)img_a = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * yrs * xrs * 4);//パズル画像頂点配列確保(通常)
	else img_a = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * yrs * xrs * 8);      //パズル画像頂点配列確保(Z方向分で*2)
	if (move == NULL) {//NULLの場合画像処理無し描画
		back_image(dx, 1);
		k1 = 0;
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z方向色情報による
				img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
				//ピクセル間隙間埋め
				img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j + 0.5f, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j + 0.5f, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
			}
		}
		pMyVB->Lock(0, 0, (void**)&img_p, 0);
		if (d3 == 1)memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 4);
		else memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 8);
		pMyVB->Unlock();
		dx->pD3DDevice->BeginScene();// 描画開始宣言
		// 頂点バッファーをレンダリングパイプラインに設定
		dx->pD3DDevice->SetStreamSource(0, pMyVB, 0, sizeof(MY_VERTEX));

		// マトリックスをレンダリングパイプラインに設定
		dx->pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF);

		// 描画
		if (d3 == 1)dx->pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, yrs * xrs * 4);
		else dx->pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, yrs * xrs * 8);
		dx->pD3DDevice->EndScene();// 描画終了宣言
		free(img_a);     //頂点配列解放
		return S_OK;
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
				read->drawing_img(dx, NULL, NULL, 0, 0, 0, 0); //画像更新
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

	//↓毎回実行する処理(ファイル関数以外)
	back_image(dx, 1);     //背景描画関数,引数1==画像描画
	k1 = 0;
	//パズル部以外の画像描画左側
	for (j = 0; j < 400; j++){
		for (i = 0; i < (xrs - 400) * 0.5; i++){
			//ifエッジ検出時の背景色描画スキップ処理以下同じ処理有り
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//パズル部以外の画像頂点右側
	for (j = 0; j < 400; j++){
		for (i = xrs - (int)((xrs - 400) * 0.5); i < xrs; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//パズル部以外の画像頂点上側
	for (j = 0; j < (yrs - 400) * 0.5; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//パズル部以外の画像頂点下側
	for (j = yrs - (int)((yrs - 400) * 0.5); j < yrs; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//パズル間の画像頂点
	if (move->size != 5){//size5は処理しない
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (d.gfr == 3 && imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] == 0)continue;
				if (filter->block[j][i] == 150 || filter->block[j][i] == 1){//フィルター値150,1のみ描画
					if (d3 == 1 || d3 == 2)pz = 20.0f;
					if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z方向色情報による
					img_a[k1].p = D3DXVECTOR3((float)i + (xrs - 400) * 0.5f, (float)j + (yrs - 400) * 0.5f, pz);
					img_a[k1++].color = D3DXCOLOR((float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff) / 255,
						(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff) / 255,
						(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff) / 255, 1.0f);
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i + (xrs - 400) * 0.5f, (float)j + (yrs - 400) * 0.5f, -pz - 15.0f);
						img_a[k1++].color = D3DXCOLOR((float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff) / 255,
							(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff) / 255,
							(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff) / 255, 1.0f);
					}
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
				imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] = (r << 16) + (g << 8) + b;//色代入
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

	if (ff == 0){ xx = 0; yy = 0, zz = 0; }//完成状態でオフセット量更新
	else{ xx = 19; yy = 19, zz = -57; }

	//パズル部画像頂点処理開始
	for (k = 0; k < prs->idx; k++){
		for (j = 0; j < prs->bsize; j++){     //1ブロック内y方向
			for (i = 0; i < prs->bsize; i++){//1ブロック内x方向

				//エッジ検出処理
				if (d.gfr == 3 && imgpi[j + img[k].fy + (yrs - 400) / 2]
					[i + img[k].fx + (xrs - 400) / 2] == 0)continue;

				//ブロック間は描画しない(フィルター値150,1 size==5)
				if ((filter->block[j + img[k].fy][i + img[k].fx] == 150 ||
					filter->block[j + img[k].fy][i + img[k].fx] == 1) &&
					move->size != 5)continue;

				//通常表示,表示座標位置＋オフセット, 表示色情報(完成座標位置－オフセット)
				cl = D3DXCOLOR((float)(imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 16 & 0xff) / 255,
					(float)(imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 8 & 0xff) / 255,
					(float)(imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] & 0xff) / 255, 1.0f);

				j2 = (float)j + img[k].cy + (yrs - 400) * 0.5f + yy + y, 0.0f;
				i2 = (float)i + img[k].cx + (xrs - 400) * 0.5f + xx + x;
				z2 = (float)img[k].cz + zz + z;

				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z方向色情報による

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
	pMyVB->Lock(0, 0, (void**)&img_p, 0);
	if (d3 == 1)memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 4);
	else memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 8);
	pMyVB->Unlock();
	dx->pD3DDevice->BeginScene();// 描画開始宣言
	// 頂点バッファーをレンダリングパイプラインに設定
	dx->pD3DDevice->SetStreamSource(0, pMyVB, 0, sizeof(MY_VERTEX));

	// マトリックスをレンダリングパイプラインに設定
	dx->pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

	//テクスチャー
	dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// 頂点データのＦＶＦフォーマットを設定
	dx->pD3DDevice->SetFVF(MY_VERTEX_FVF);

	// 描画
	if (d3 == 1)dx->pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, yrs * xrs * 4);
	else dx->pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, yrs * xrs * 8);
	dx->pD3DDevice->EndScene();// 描画終了宣言
	free(img_a);    //頂点配列解放

	//完成,未完成直後処理(再帰は1度だけ処理される)
	if (ffr == 1){
		if (ff == 0){//完成直後処理
			for (i = 0; i < 19; i++){
				drawing_img(dx, filter, move, 19 - i, 19 - i, -57 + i * 3, 0);//再帰
				dx->drawscreen();//描画
			}
		}
		if (ff == 1){//未完成直後処理
			for (i = 0; i < 19; i++){
				drawing_img(dx, filter, move, i - 19, i - 19, 57 - i * 3, 0);//再帰(19はxx,yyの分の補正)
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
	if (finish == 1){
		k1 = 0;
		fimg = (MY_VERTEX2*)malloc(sizeof(MY_VERTEX2) * 200 * 200);
		for (j = 0; j < 200; j++){
			for (i = 0; i < 200; i++){
				if (d.gfr == 3 && imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2] == 0)continue;
				fimg[k1].x = 590.0f + i;
				fimg[k1].y = 360.0f + (199.0f - j);
				fimg[k1].z = 0.0f;
				fimg[k1].rhw = 0.0f;
				fimg[k1].color = imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2];
				fimg[k1].tu = 0.0f;
				fimg[k1++].tv = 0.0f;
			}
		}
		dx->pD3DDevice->BeginScene();// 描画開始宣言
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 200 * 200, fimg, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();// 描画終了宣言
		free(fimg);
	}

	/********************************************************************************************************************************/
	/********************************************最終画像描画処理終了****************************************************************/
	/********************************************************************************************************************************/
	return S_OK;
}

