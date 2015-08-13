//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx9Init�N���X                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx9Init_Header
#define Class_Dx9Init_Header

class ImageDraw;//�O���錾

class Dx9Init{//DirectX������

private:
	IDirect3D9 *pD3D;			   //IDirect3D9�C���^�[�t�F�C�X�ւ̃|�C���^
	IDirect3DDevice9 *pD3DDevice; //IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	D3DPRESENT_PARAMETERS D3DPP; //�f�o�C�X�̃v���[���e�[�V�����p�����[�^
	D3DDISPLAYMODE d3ddm;        //�f�B�X�v���C���[�h
	LPD3DXFONT pD3DFont;        //ID3DXFont�R���|�[�l���g����

	//�}�g���b�N�X
	D3DXMATRIX	m_proj;		// �J�����̉�p�Ȃ�(�`��֐��ɏ���)
	D3DXMATRIX	m_view;	   // �J�����̔z�u
	D3DXMATRIX	m_world; // ���f���̔z�u
	D3DVIEWPORT9 vp;   //�r���[�|�[�g

public:
	//�p�Y���摜���_
	struct MY_VERTEX{    //���_�f�[�^�̍\����
		D3DXVECTOR3 p;  //�ʒu
		D3DCOLOR color;//�F
	};
	MY_VERTEX *img_p;//�o�b�t�@�p

	//�����摜���_(2�����\��),�{�^��,�w�i
	struct MY_VERTEX2{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tu, tv;
	};

	HRESULT initialize(HWND hWnd);                //DirectX9������
	HRESULT getVBarray(ImageDraw *draw, int d3);//���_�o�b�t�@,�z��m��
	HRESULT sclear();                    //�X�N���[���N���A
	void cameraset(int xrs, int yrs, float cpx, float cpz);//�J�����Z�b�g
	void d3primitive(ImageDraw *draw, int d3);//3D�`��
	void d2primitive(MY_VERTEX2 *fimg);      //2D�`��(�����摜)
	void d2rectangle_button(char *str, float x, float y, MY_VERTEX2 *re);//��������{�^��
	void d2square_button(int num, MY_VERTEX2 *sq);                      //�l�p�`�{�^��,�O�p�`��
	void line(LPD3DXLINE *pLine);                                        //���C���`��
	void getbackimage(LPDIRECT3DTEXTURE9 *pTexture, int r);           //�w�i�摜�擾
	void d2backimage(LPDIRECT3DTEXTURE9 pTexture, MY_VERTEX2 *h);   //�w�i�`��
	void text(char str[30], int x, int y);//�e�L�X�g�`��
    void drawscreen();//�X�N���[���`��
	~Dx9Init();      //�f�X�g���N�^ 
};

#endif