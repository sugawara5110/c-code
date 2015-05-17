//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@ImageDraw�N���X��`                                   **//
//**                             (ImageRead�̔h���N���X)                                 **//
//*****************************************************************************************//

#ifndef Class_ImageDraw_Header
#define Class_ImageDraw_Header

class ImageDraw :public ImageRead{

private:
	//�}�g���b�N�X
	D3DXMATRIX	m_proj;		// �J�����̉�p�Ȃ�(�`��֐��ɏ���)
	D3DXMATRIX	m_view;	   // �J�����̔z�u
	D3DXMATRIX	m_world;  // ���f���̔z�u

	D3DVIEWPORT9 vp;//�r���[�|�[�g

	//�p�Y���摜���_
	struct MY_VERTEX{   //���_�f�[�^�̍\����
		D3DXVECTOR3 p; //�ʒu
		DWORD color;  //�f�t���[�Y�F
	};
	MY_VERTEX *img_p, *img_a;//�o�b�t�@�p,�z��p

	//�����摜���_
	struct MY_VERTEX2{
		float x, y, z;
		float rhw;
		D3DCOLOR color;
		float tu, tv;
	};
	MY_VERTEX2 *fimg;
	
	//�q�X�g�O�����`��
	ID3DXLine *pLine;  //�������C���^�[�t�F�[�X(2D)
	D3DXVECTOR2 hi[2];//���_

public:
	ImageRead *read;//�摜�G���{�X�p

	int theta_lr;   //3D�J�����p�x 
	float cpx, cpz;//�J�����ʒu

	ImageDraw();                            //�K��R���X�g���N�^
	ImageDraw(Dx9Init *dx, char *name);    //�����L�R���X�g���N�^(�t�@�C���������N���X�ɓn��)
	HRESULT draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z);//�ŏI�`��֐�
	void obj_create(Dx9Init *dx, char *name); //�I�u�W�F�N�g�����֐�
	void obj_delete();                 //�I�u�W�F�N�g�j���֐�
	int image_processing_flg(Dx9Init *dx, Filter *filter, Move *move, int flg);//�摜�����t���O
	int theta(int f, int t);          //�J�����p�x����
	~ImageDraw();                    //�f�X�g���N�^

};

#endif