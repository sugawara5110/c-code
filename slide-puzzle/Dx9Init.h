//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　          Dx9Initクラス                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx9Init_Header
#define Class_Dx9Init_Header

class ImageDraw;//前方宣言

class Dx9Init{//DirectX初期化

private:
	IDirect3D9 *pD3D;			   //IDirect3D9インターフェイスへのポインタ
	IDirect3DDevice9 *pD3DDevice; //IDirect3DDevice9インターフェイスへのポインタ
	D3DPRESENT_PARAMETERS D3DPP; //デバイスのプレゼンテーションパラメータ
	D3DDISPLAYMODE d3ddm;        //ディスプレイモード
	LPD3DXFONT pD3DFont;        //ID3DXFontコンポーネント生成

	//マトリックス
	D3DXMATRIX	m_proj;		// カメラの画角など(描画関数に書く)
	D3DXMATRIX	m_view;	   // カメラの配置
	D3DXMATRIX	m_world; // モデルの配置
	D3DVIEWPORT9 vp;   //ビューポート

public:
	//パズル画像頂点
	struct MY_VERTEX{    //頂点データの構造体
		D3DXVECTOR3 p;  //位置
		D3DCOLOR color;//色
	};
	MY_VERTEX *img_p;//バッファ用

	//完成画像頂点(2次元表示),ボタン,背景
	struct MY_VERTEX2{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tu, tv;
	};

	HRESULT initialize(HWND hWnd);                //DirectX9初期化
	HRESULT getVBarray(ImageDraw *draw, int d3);//頂点バッファ,配列確保
	HRESULT sclear();                    //スクリーンクリア
	void cameraset(int xrs, int yrs, float cpx, float cpz);//カメラセット
	void d3primitive(ImageDraw *draw, int d3);//3D描画
	void d2primitive(MY_VERTEX2 *fimg);      //2D描画(完成画像)
	void d2rectangle_button(char *str, float x, float y, MY_VERTEX2 *re);//文字入りボタン
	void d2square_button(int num, MY_VERTEX2 *sq);                      //四角形ボタン,三角描画
	void line(LPD3DXLINE *pLine);                                        //ライン描画
	void getbackimage(LPDIRECT3DTEXTURE9 *pTexture, int r);           //背景画像取得
	void d2backimage(LPDIRECT3DTEXTURE9 pTexture, MY_VERTEX2 *h);   //背景描画
	void text(char str[30], int x, int y);//テキスト描画
    void drawscreen();//スクリーン描画
	~Dx9Init();      //デストラクタ 
};

#endif