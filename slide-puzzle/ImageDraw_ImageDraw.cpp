//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageDrawクラス内メソッド定義コンストラクタ等               **//
//**                                    (ImageReadの派生クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h" 
#include "back_image.h"

ImageDraw::ImageDraw(){}//規定コンストラクタ

ImageDraw::ImageDraw(Dx9Init *dx, char *name) :ImageRead(dx, name){//コンストラクタ

	read = NULL;
	back_image(dx, 0);//背景描画関数,引数0==画像をハンドルに格納
	D3DXCreateLine(dx->pD3DDevice, &pLine);//線ひくやつ
	theta_lr = 0;     //3Dモードカメラ角度

	//各マトリックス初期化
	D3DXMatrixIdentity(&m_view);

	D3DXMatrixIdentity(&m_proj);

	D3DXMatrixIdentity(&m_world);

	// ビューポートの取得
	dx->pD3DDevice->GetViewport(&vp);

	// アスペクト比の計算
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// 射影マトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian(45.0f),	// カメラの画角
		aspect,				   // アスペクト比
		1.0f,		          // nearプレーン(0.0fだと描画おかしい。3日悩んだ)
		10000.0f);			 // farプレーン

	dx->pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
	dx->pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);
}

ImageDraw::~ImageDraw(){//デストラクタ

	pLine->Release();//線引くやつ解放
	pLine = NULL;
	obj_delete();//オブジェクト破棄関数
}

void ImageDraw::obj_create(Dx9Init *dx, char *name){//オブジェクト生成関数

	if (read == NULL){//オブジェクト生成されていない時のみ実行
		read = new ImageRead(dx, name);//画像エンボス用オブジェクト生成
	}
}

void ImageDraw::obj_delete(){//オブジェクト破棄関数

	if (read != NULL){//オブジェクト生成されている時のみ実行
		read->d.cap.release();         //カメラ終了処理
		delete read;                  //オブジェクト破棄
		read = NULL;
	}
}

