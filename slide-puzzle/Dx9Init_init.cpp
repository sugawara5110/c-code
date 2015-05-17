//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　Dx9Initクラス内メソッド定義                           **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"

// 頂点データのＦＶＦフォーマットの設定
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )

HRESULT Dx9Init::init(HWND hWnd){

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
	D3DPP.Windowed = TRUE;				       // ウインドウ内表示の指定
	D3DPP.BackBufferFormat = d3ddm.Format;	  // カラーモードの指定
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;// 
	D3DPP.EnableAutoDepthStencil = TRUE;	   //Zバッファ
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
	
	return S_OK;
}

HRESULT Dx9Init::verbufcr(LPDIRECT3DVERTEXBUFFER9 *pMyVB, int xrs, int yrs){

	//頂点バッファを作る
	if (FAILED(pD3DDevice->CreateVertexBuffer(
		sizeof(MY_VERTEX) * yrs * xrs * 4,// 頂点バッファ全体のサイズ
		D3DUSAGE_WRITEONLY,		         // 頂点バッファの使われ方
		MY_VERTEX_FVF,		   // 頂点データのFVFフォーマット
		D3DPOOL_MANAGED, 	  // システムの頂点バッファ扱い方
		pMyVB, 			     // 頂点バッファを格納する変数
		NULL)))             // 常にNULLに指定
	{
		return E_FAIL;
	}
	return S_OK;
}

void Dx9Init::drawscreen(){//描画

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