//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　ImageDrawクラス定義                                   **//
//**                             (ImageReadの派生クラス)                                 **//
//*****************************************************************************************//

#ifndef Class_ImageDraw_Header
#define Class_ImageDraw_Header

class ImageDraw :public ImageRead{

public:
	ImageRead *read;//画像エンボス用

	ImageDraw();                                          //規定コンストラクタ
	ImageDraw(char *name);                               //引数有コンストラクタ(ファイル名を基底クラスに渡す)
	void draw(Filter *filter, Move *move, int x, int y);//最終描画関数
	void obj_create(char *name);//オブジェクト生成関数
	void obj_delete();         //オブジェクト破棄関数
	~ImageDraw();             //デストラクタ

};

#endif