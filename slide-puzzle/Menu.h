//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@Menu�N���X��`                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Menu_Header
#define Class_Menu_Header
// #ifndef ���� #endif �܂ł̊Ԃɒ�`���L�ڂ��܂��B

class Sound_;

class Menu{

private:
	Sound_ *sound;//�{�^�����ʉ�
	int sf;      //���ʉ��g�p��
	int d3f;    //�摜��]�p�p�l���o���t���O
	int d3b;   //3D�؂�ւ��{�^���ʉߗ��� 

	// ���_�f�[�^�̍\����
	struct MY_VERTEX2{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tu, tv;
	};
	MY_VERTEX2 re[4], sq[4], tr[3];

	int square_button(Dx9Init *dx, ImageDraw *draw, int xm, int ym, float x, float y, char *str, int fr);//�����{�^��
	int rectangle_button(Dx9Init *dx, int xm, int ym, float x, float y, char *str, int f, int fr);      //�����{�^��

public:
	int fin_g;//�摜�����t���O
	static int xm, ym, clf;//�}�E�X���W,�N���b�N�t���O

	Menu();      //�K��R���X�g���N�^
	Menu(int s);//���ʉ������R���X�g���N�^
	int mouse(Dx9Init *dx, ImageDraw *draw, int f, int *offset);//���j���[
	~Menu();//�f�X�g���N�^

};

#endif