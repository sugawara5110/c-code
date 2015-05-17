//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Dx9Initクラス                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx9Init_Header
#define Class_Dx9Init_Header

class Dx9Init{//DirectX初期化

public:
	IDirect3D9 *pD3D;			   //IDirect3D9インターフェイスへのポインタ
	IDirect3DDevice9 *pD3DDevice; //IDirect3DDevice9インターフェイスへのポインタ
	D3DPRESENT_PARAMETERS D3DPP; //デバイスのプレゼンテーションパラメータ
	D3DDISPLAYMODE d3ddm;       //ディスプレイモード
	LPD3DXFONT pD3DFont;       //ID3DXFontコンポーネント生成

	//パズル画像頂点
	struct MY_VERTEX{   //頂点データの構造体
		D3DXVECTOR3 p; //位置
		DWORD color;  //デフューズ色
	};

	HRESULT init(HWND hWnd);//DirectX9初期化
	HRESULT verbufcr(LPDIRECT3DVERTEXBUFFER9 *pMyVB, int xrs, int yrs);//頂点バッファー生成(pMyVBは返却値用)
	void drawscreen();//描画
	~Dx9Init();      //デストラクタ 

};

#endif