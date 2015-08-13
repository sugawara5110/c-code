//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@ImageDraw�N���X��`                                   **//
//**                             (ImageRead�̔h���N���X)                                 **//
//*****************************************************************************************//

#ifndef Class_ImageDraw_Header
#define Class_ImageDraw_Header

class ImageDraw :public ImageRead{

private:
	//�q�X�g�O�����`��
	ID3DXLine *pLine;  //���C���`��C���^�[�t�F�[�X(2D)(ID3DXLine *LPD3DXLINE)
	D3DXVECTOR2 hi[2];//���_

	int ver;          //���_�S�`��t���O0:�`����s 1:�`���
	int **pi16;      //�G�b�W,�G���{�X�����ꎞ�ۊǗp
	int *slit;          //�X���b�g�X�L���������p�ꎞ�ۊ�
	float cpx, cpz;    //�J�����ʒu
	ImageRead *read; //�摜�G���{�X�p
	int theta_lr;    //3D�J�����p�x 

	Dx9Init::MY_VERTEX2 *fimg;//�����摜�p

	int **block; //�t�B���^�[�i�[�p
	bool finish;//�����摜�\���t���O 0:��\�� 1:�\�� (�}�E�X�֐��ő���,�ړ��{�^���͈͓��ɃJ�[�\�����\��)
	int d3;     //3D�؂�ւ�

	HRESULT f_vertex(Dx9Init *dx, int x, int y, int z); //�t�@�C���I��p���_
	HRESULT processing_img();                          //�摜�ϊ�
	HRESULT p_vertex(Dx9Init *dx, Move *move, int x, int y, int z);//�p�Y���摜���_
	void obj_create(Dx9Init *dx, char *name);  //�I�u�W�F�N�g�����֐�(�摜�G���{�X�p)
	void obj_delete();                         //�I�u�W�F�N�g�j���֐�(�摜�G���{�X�p)
	void capture(Move *move);//�摜�ۑ�
	void releaseVB();       //���_�o�b�t�@,�z����

public:
	Dx9Init::MY_VERTEX *img_a;      //�z��p
	LPDIRECT3DVERTEXBUFFER9 pMyVB;//���_�o�b�t�@�[
	
	ImageDraw();                             //�K��R���X�g���N�^
	ImageDraw(Dx9Init *dx, char *name);    //�����L�R���X�g���N�^(�t�@�C���������N���X�ɓn��)
	void filter(Move *move);               //�t�B���^�[
	int draw_img(Dx9Init *dx, int x, int y, int z);              //�t�@�C���I��p�摜�`��
	int draw_img(Dx9Init *dx, Move *move, int x, int y, int z);//�摜�`��
	int processing_img_flg(Dx9Init *dx, Move *move, int flg);//�摜�����t���O
	int theta(int f, int t);  //�J�����p�x����
	void putfin(bool fin);   //�����摜ON OFF����
	~ImageDraw();           //�f�X�g���N�^

};

#endif