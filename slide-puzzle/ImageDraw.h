//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@ImageDraw�N���X��`                                   **//
//**                             (ImageRead�̔h���N���X)                                 **//
//*****************************************************************************************//

#ifndef Class_ImageDraw_Header
#define Class_ImageDraw_Header

class ImageDraw :public ImageRead{

public:
	ImageRead *read;//�摜�G���{�X�p

	ImageDraw();                                          //�K��R���X�g���N�^
	ImageDraw(char *name);                               //�����L�R���X�g���N�^(�t�@�C���������N���X�ɓn��)
	void draw(Filter *filter, Move *move, int x, int y);//�ŏI�`��֐�
	void obj_create(char *name);//�I�u�W�F�N�g�����֐�
	void obj_delete();         //�I�u�W�F�N�g�j���֐�
	~ImageDraw();             //�f�X�g���N�^

};

#endif