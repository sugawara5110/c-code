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
	int sf;       //効果音使用可否
	int d3f;     //画像回転用パネル出現フラグ
	int d3b;    //3D切り替えボタン通過履歴 
	bool fin_g;//画像完成フラグ(移動パネル上にカーソルが有る状態=true それ以外=false)

	Dx9Init::MY_VERTEX2 re[4], sq[4], tr[3];//ボタン

	int square_button(Dx9Init *dx, ImageDraw *draw, int xm, int ym, float x, float y, char *str, int fr);//方向ボタン
	int rectangle_button(Dx9Init *dx, int xm, int ym, float x, float y, char *str, int f, int fr);       //押しボタン

public:
	static int xm, ym, clf;//マウス座標,クリックフラグ(WindowProcで操作)

	Menu();      //規定コンストラクタ
	Menu(int s);//効果音無しコンストラクタ
	int mouse(Dx9Init *dx, ImageDraw *draw, int f, int *offset);//メニュー
	~Menu();//デストラクタ

};

#endif