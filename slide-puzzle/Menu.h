//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　Menuクラス定義                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Menu_Header
#define Class_Menu_Header
// #ifndef から #endif までの間に定義を記載します。

class Sound_;

class Menu{

private:
	Sound_ *sound;//ボタン効果音
	int sf;      //効果音使用可否
	int d3f;    //画像回転用パネル出現フラグ
	int d3b;   //3D切り替えボタン通過履歴 

	// 頂点データの構造体
	struct MY_VERTEX2{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tu, tv;
	};
	MY_VERTEX2 re[4], sq[4], tr[3];

	int square_button(Dx9Init *dx, ImageDraw *draw, int xm, int ym, float x, float y, char *str, int fr);//方向ボタン
	int rectangle_button(Dx9Init *dx, int xm, int ym, float x, float y, char *str, int f, int fr);      //押しボタン

public:
	int fin_g;//画像完成フラグ
	static int xm, ym, clf;//マウス座標,クリックフラグ

	Menu();      //規定コンストラクタ
	Menu(int s);//効果音無しコンストラクタ
	int mouse(Dx9Init *dx, ImageDraw *draw, int f, int *offset);//メニュー
	~Menu();//デストラクタ

};

#endif