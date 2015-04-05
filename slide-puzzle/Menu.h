//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　Menuクラス定義                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Menu_Header
#define Class_Menu_Header
// #ifndef から #endif までの間に定義を記載します。

class Menu{

public:
	int fin_g;//画像完成フラグ

	int square_button(ImageDraw *draw, int xm, int ym, int bt1on, int bt1of, int x, int y, char *str, int fr);
	int rectangle_button(int xm, int ym, int bt2on, int bt2of, int x, int y, char *str, int f, int fr);
    int mouse(ImageDraw *draw, int f, int *offset);

};

#endif