//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　Dx9Initクラス内メソッド定義                           **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"

// 頂点データのＦＶＦフォーマットの設定
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

HRESULT Dx9Init::initialize(HWND hWnd){

	// Direct3D9オブジェクトの作成
	if ((pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0){
		return E_FAIL;	// 取得失敗
	}

	// 現在のディスプレイモードを取得
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータを初期化
	ZeroMemory(&D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	D3DPP.BackBufferCount = 1;
	D3DPP.Windowed = TRUE;				         // ウインドウ内表示の指定
	D3DPP.BackBufferFormat = d3ddm.Format;	// カラーモードの指定
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;// 
	D3DPP.EnableAutoDepthStencil = TRUE;	  //Zバッファ
	D3DPP.AutoDepthStencilFormat = D3DFMT_D16;//Zバッファのフォーマット

	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&D3DPP, &pD3DDevice))) {
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&D3DPP, &pD3DDevice))) {
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&D3DPP, &pD3DDevice))) {
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// フォントの生成
	int fontsize = 15;
	D3DXFONT_DESC lf = { fontsize, 0, 0, 1, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, ("ＭＳ ゴシック") };

	// ID3DXFontコンポーネント生成
	if (FAILED(D3DXCreateFontIndirect(pD3DDevice, &lf, &pD3DFont))){
		pD3DDevice->Release();  pD3D->Release();
		return E_FAIL;
	}

	//レンダリングステートパラメータの設定
	// 両面描画モードの指定
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Ｚ比較を行なう(これやらないと表示順番おかしくなる)
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// ディザリングを行なう（高品質描画）
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// ノーライティングモード 	
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//アルファブレンディング
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//点サイズ
	float PointSize = 1.0f;//キャストして使う
	pD3DDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&PointSize));

	//各マトリックス初期化
	D3DXMatrixIdentity(&m_view);

	D3DXMatrixIdentity(&m_proj);

	D3DXMatrixIdentity(&m_world);

	// ビューポートの取得
	pD3DDevice->GetViewport(&vp);

	// アスペクト比の計算
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// 射影マトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian(45.0f),	// カメラの画角
		aspect,				   // アスペクト比
		1.0f,		          // nearプレーン(0.0fだと描画おかしい。3日悩んだ)
		10000.0f);			 // farプレーン

	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);

	return S_OK;
}

HRESULT Dx9Init::getVBarray(ImageDraw *draw, int d3){//頂点バッファ,配列確保

	int n;
	if (d3 == 1)n = 4;
	else n = 8;
	
	//頂点バッファを作る
	if (FAILED(pD3DDevice->CreateVertexBuffer(
		sizeof(MY_VERTEX) * draw->yrs * draw->xrs * n,// 頂点バッファ全体のサイズ
		D3DUSAGE_WRITEONLY,		// 頂点バッファの使われ方
		MY_VERTEX_FVF,		   // 頂点データのFVFフォーマット
		D3DPOOL_MANAGED, 	  // システムの頂点バッファ扱い方
		&draw->pMyVB, 	     // 頂点バッファを格納する変数
		NULL)))             // 常にNULLに指定
	{
		return E_FAIL;
	}

	draw->img_a = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * draw->yrs * draw->xrs * n);//パズル画像頂点配列確保
	
	return S_OK;
}

HRESULT Dx9Init::sclear(){//スクリーンクリア
	
	// ビューポートと深度バッファのクリアとステンシルバッファの削除
	if (FAILED(pD3DDevice->Clear(0, NULL,   // クリアする領域は全面
		D3DCLEAR_TARGET |			// バックバッファを指定
		D3DCLEAR_ZBUFFER, 		   // 深度バッファ（Zバッファ）を指定
		D3DCOLOR_XRGB(0, 0, 255), // クリアする色
		1.0f, 					 // クリアする深度バッファ（Zバッファ）の値
		0)))				    // クリアするステンシルバッファの値
	{
		return E_FAIL;
	}
	return S_OK;
}

void Dx9Init::cameraset(int xrs, int yrs, float cpx, float cpz){
	
	// カメラの位置と方向を設定
	D3DXMatrixLookAtLH(&m_view,
		&D3DXVECTOR3(cpx, yrs * 0.5f, cpz),          //カメラの位置
		&D3DXVECTOR3(xrs * 0.5f, yrs * 0.5f, 0.0f),//カメラの方向を向ける点
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));          //カメラの上の方向
}

void Dx9Init::d3primitive(ImageDraw *draw, int d3){//3D描画

	int n;
	if (d3 == 1)n = 4;
	else n = 8;

	draw->pMyVB->Lock(0, 0, (void**)&img_p, 0);
	memcpy(img_p, draw->img_a, sizeof(Dx9Init::MY_VERTEX) * draw->yrs * draw->xrs * n);
	draw->pMyVB->Unlock();
	pD3DDevice->BeginScene();// 描画開始宣言
	// 頂点バッファーをレンダリングパイプラインに設定
	pD3DDevice->SetStreamSource(0, draw->pMyVB, 0, sizeof(Dx9Init::MY_VERTEX));

	// マトリックスをレンダリングパイプラインに設定
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

	//テクスチャー
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// 頂点データのＦＶＦフォーマットを設定
	pD3DDevice->SetFVF(MY_VERTEX_FVF);

	// 描画
	if (d3 == 1)pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, draw->yrs * draw->xrs * 4);
	else pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, draw->yrs * draw->xrs * 8);
	pD3DDevice->EndScene();// 描画終了宣言
}

void Dx9Init::d2primitive(MY_VERTEX2 *fimg){//2D描画(完成画像)

	pD3DDevice->BeginScene();// 描画開始宣言
	// 頂点データのＦＶＦフォーマットを設定
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// 描画
	pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 200 * 200, fimg, sizeof(MY_VERTEX2));
	pD3DDevice->EndScene();// 描画終了宣言
}

void Dx9Init::d2rectangle_button(char *str, float x, float y, MY_VERTEX2 *re){//文字入りボタン

	pD3DDevice->BeginScene();
	// 頂点データのＦＶＦフォーマットを設定
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// 描画
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, re, sizeof(MY_VERTEX2));
	//文字
	RECT r = { (long)x, (long)y + 2, (long)x, (long)y + 2 };
	pD3DFont->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	//テクスチャー
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pD3DDevice->EndScene();
}

void Dx9Init::d2square_button(int num, MY_VERTEX2 *sq){//四角形ボタン,三角描画

	pD3DDevice->BeginScene();
	//テクスチャー
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	// 頂点データのＦＶＦフォーマットを設定
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// 描画
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, num, sq, sizeof(MY_VERTEX2));
	pD3DDevice->EndScene();
}

void Dx9Init::line(LPD3DXLINE *pLine){//ライン描画

	D3DXCreateLine(pD3DDevice, pLine);
}

void Dx9Init::getbackimage(LPDIRECT3DTEXTURE9 *pTexture, int r){//背景画像取得

	if (r == 0)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk0.dat", pTexture);//背景画像読み込み
	if (r == 1)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk1.dat", pTexture);
	if (r == 2)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk2.dat", pTexture);
	if (r == 3)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk3.dat", pTexture);
	if (r == 4)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk4.dat", pTexture);
}

void Dx9Init::d2backimage(LPDIRECT3DTEXTURE9 pTexture, MY_VERTEX2 *h){//背景描画

	//テクスチャー
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pD3DDevice->SetTexture(0, pTexture);
	// 頂点データのＦＶＦフォーマットを設定
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// 描画
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, h, sizeof(MY_VERTEX2));
}

void Dx9Init::text(char str[30], int x, int y){//テキスト描画

	pD3DDevice->BeginScene();
	RECT r = { x, y, x, y };
	pD3DFont->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	pD3DDevice->EndScene();
}

void Dx9Init::drawscreen(){//スクリーン描画

	if (FAILED(pD3DDevice->Present(0, 0, 0, 0))) {//描画結果の転送
		// デバイス消失から復帰
		pD3DDevice->Reset(&D3DPP);
	}
}

Dx9Init::~Dx9Init(){//デストラクタ

	pD3DFont->Release();              //ID3DXFontコンポーネント
	pD3DFont = NULL;
	pD3DDevice->Release();          //IDirect3DDevice9インターフェイス
	pD3DDevice = NULL;
	pD3D->Release();	          //IDirect3D9インターフェイス
	pD3D = NULL;
}