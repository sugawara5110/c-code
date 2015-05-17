//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　背景画像処理                                            **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <stdlib.h>
#include <time.h>

// 頂点データのＦＶＦフォーマットの設定
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

void back_image(Dx9Init *dx, int f){//f操作用

	int r;    //ランダム
	// 頂点データの構造体(背景)
	struct MY_VERTEX2{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tu, tv;
	};
	static MY_VERTEX2 h[4];
	static LPDIRECT3DTEXTURE9 pTexture;

	srand((unsigned)time(NULL));

	if (f == 0){
		h[0].x = 0;
		h[0].y = 0;
		h[0].z = 100.0f;
		h[0].rhw = 1.0f;
		h[0].color = (255 << 16) + (255 << 8) + 255;
		h[0].tu = 0.0f;
		h[0].tv = 0.0f;
		h[1].x = 0;
		h[1].y =565;
		h[1].z = 100.0f;
		h[1].rhw = 1.0f;
		h[1].color = (255 << 16) + (255 << 8) + 255;
		h[1].tu = 0.0f;
		h[1].tv = 1.0f;
		h[2].x = 790;
		h[2].y = 565;
		h[2].z = 100.0f;
		h[2].rhw = 1.0f;
		h[2].color = (255 << 16) + (255 << 8) + 255;
		h[2].tu = 1.0f;
		h[2].tv = 1.0f;
		h[3].x = 790;
		h[3].y = 0;
		h[3].z = 100.0f;
		h[3].rhw = 1.0f;
		h[3].color = (255 << 16) + (255 << 8) + 255;
		h[3].tu = 1.0f;
		h[3].tv = 0.0f;

		r = rand() % 5;
		if (r == 0)D3DXCreateTextureFromFile(dx->pD3DDevice, "./dat/hk/hk0.dat", &pTexture);//背景画像読み込み
		if (r == 1)D3DXCreateTextureFromFile(dx->pD3DDevice, "./dat/hk/hk1.dat", &pTexture);
		if (r == 2)D3DXCreateTextureFromFile(dx->pD3DDevice, "./dat/hk/hk2.dat", &pTexture);
		if (r == 3)D3DXCreateTextureFromFile(dx->pD3DDevice, "./dat/hk/hk3.dat", &pTexture);
		if (r == 4)D3DXCreateTextureFromFile(dx->pD3DDevice, "./dat/hk/hk4.dat", &pTexture);

		return;
	}

	if (f == 1){
		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		dx->pD3DDevice->SetTexture(0, pTexture);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);

		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &h, sizeof(MY_VERTEX2));
		
		return;
	}

}