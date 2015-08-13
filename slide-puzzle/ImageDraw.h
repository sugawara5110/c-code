//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　ImageDrawクラス定義                                   **//
//**                             (ImageReadの派生クラス)                                 **//
//*****************************************************************************************//

#ifndef Class_ImageDraw_Header
#define Class_ImageDraw_Header

class ImageDraw :public ImageRead{

private:
	//ヒストグラム描画
	ID3DXLine *pLine;  //ライン描画インターフェース(2D)(ID3DXLine *LPD3DXLINE)
	D3DXVECTOR2 hi[2];//頂点

	int ver;          //頂点全描画フラグ0:描画実行 1:描画済
	int **pi16;      //エッジ,エンボス処理一時保管用
	int *slit;          //スリットスキャン処理用一時保管
	float cpx, cpz;    //カメラ位置
	ImageRead *read; //画像エンボス用
	int theta_lr;    //3Dカメラ角度 

	Dx9Init::MY_VERTEX2 *fimg;//完成画像用

	int **block; //フィルター格納用
	bool finish;//完成画像表示フラグ 0:非表示 1:表示 (マウス関数で操作,移動ボタン範囲内にカーソル時表示)
	int d3;     //3D切り替え

	HRESULT f_vertex(Dx9Init *dx, int x, int y, int z); //ファイル選択用頂点
	HRESULT processing_img();                          //画像変換
	HRESULT p_vertex(Dx9Init *dx, Move *move, int x, int y, int z);//パズル画像頂点
	void obj_create(Dx9Init *dx, char *name);  //オブジェクト生成関数(画像エンボス用)
	void obj_delete();                         //オブジェクト破棄関数(画像エンボス用)
	void capture(Move *move);//画像保存
	void releaseVB();       //頂点バッファ,配列解放

public:
	Dx9Init::MY_VERTEX *img_a;      //配列用
	LPDIRECT3DVERTEXBUFFER9 pMyVB;//頂点バッファー
	
	ImageDraw();                             //規定コンストラクタ
	ImageDraw(Dx9Init *dx, char *name);    //引数有コンストラクタ(ファイル名を基底クラスに渡す)
	void filter(Move *move);               //フィルター
	int draw_img(Dx9Init *dx, int x, int y, int z);              //ファイル選択用画像描画
	int draw_img(Dx9Init *dx, Move *move, int x, int y, int z);//画像描画
	int processing_img_flg(Dx9Init *dx, Move *move, int flg);//画像処理フラグ
	int theta(int f, int t);  //カメラ角度操作
	void putfin(bool fin);   //完成画像ON OFF入力
	~ImageDraw();           //デストラクタ

};

#endif