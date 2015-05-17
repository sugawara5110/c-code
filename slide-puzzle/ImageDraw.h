//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　ImageDrawクラス定義                                   **//
//**                             (ImageReadの派生クラス)                                 **//
//*****************************************************************************************//

#ifndef Class_ImageDraw_Header
#define Class_ImageDraw_Header

class ImageDraw :public ImageRead{

private:
	//マトリックス
	D3DXMATRIX	m_proj;		// カメラの画角など(描画関数に書く)
	D3DXMATRIX	m_view;	   // カメラの配置
	D3DXMATRIX	m_world;  // モデルの配置

	D3DVIEWPORT9 vp;//ビューポート

	//パズル画像頂点
	struct MY_VERTEX{   //頂点データの構造体
		D3DXVECTOR3 p; //位置
		DWORD color;  //デフューズ色
	};
	MY_VERTEX *img_p, *img_a;//バッファ用,配列用

	//完成画像頂点
	struct MY_VERTEX2{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tu, tv;
	};
	MY_VERTEX2 *fimg;
	
	//ヒストグラム描画
	ID3DXLine *pLine;  //線引くインターフェース(2D)
	D3DXVECTOR2 hi[2];//頂点

public:
	ImageRead *read;//画像エンボス用

	int theta_lr;   //3Dカメラ角度 
	float cpx, cpz;//カメラ位置

	ImageDraw();                            //規定コンストラクタ
	ImageDraw(Dx9Init *dx, char *name);    //引数有コンストラクタ(ファイル名を基底クラスに渡す)
	HRESULT draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z);//最終描画関数
	void obj_create(Dx9Init *dx, char *name); //オブジェクト生成関数
	void obj_delete();                 //オブジェクト破棄関数
	int image_processing_flg(Dx9Init *dx, Filter *filter, Move *move, int flg);//画像処理フラグ
	int theta(int f, int t);          //カメラ角度操作
	~ImageDraw();                    //デストラクタ

};

#endif