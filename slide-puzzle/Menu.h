//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@Menu�N���X��`                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Menu_Header
#define Class_Menu_Header
// #ifndef ���� #endif �܂ł̊Ԃɒ�`���L�ڂ��܂��B

class Menu{

public:
	int fin_g;//�摜�����t���O

	int square_button(ImageDraw *draw, int xm, int ym, int bt1on, int bt1of, int x, int y, char *str, int fr);
	int rectangle_button(int xm, int ym, int bt2on, int bt2of, int x, int y, char *str, int f, int fr);
    int mouse(ImageDraw *draw, int f, int *offset);

};

#endif