//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義(ファイル選択用頂点処理)       **//
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

HRESULT ImageDraw::f_vertex(Dx9Init *dx, int x, int y, int z){//ファイル選択用頂点処理

	if (pMyVB == NULL)dx->getVBarray(this, d3);

	int i, j, k1;
	DWORD cl;
	float pz;

	//スクリーンクリア 
	if (FAILED(dx->sclear()))return E_FAIL;
	
	//カメラ位置更新
	cpx = xrs * 0.5f + sin(theta_lr * 3.14f / 180.0f) * 750.0f;
	cpz = -cos(theta_lr * 3.14f / 180.0f) * 750.0f;

	// カメラの位置と方向を設定
	dx->cameraset(xrs, yrs, cpx, cpz);

	back_image(dx, 1);
	k1 = 0;
	for (j = 0; j < yrs; j++){
		for (i = 0; i < xrs; i++){
			cl = imgpi[j][i];
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
	
	dx->d3primitive(this, d3);//3D描画

	return S_OK;
}

	