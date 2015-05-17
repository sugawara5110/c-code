//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@          Dx9Init�N���X                                     **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Dx9Init_Header
#define Class_Dx9Init_Header

class Dx9Init{//DirectX������

public:
	IDirect3D9 *pD3D;			   //IDirect3D9�C���^�[�t�F�C�X�ւ̃|�C���^
	IDirect3DDevice9 *pD3DDevice; //IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
	D3DPRESENT_PARAMETERS D3DPP; //�f�o�C�X�̃v���[���e�[�V�����p�����[�^
	D3DDISPLAYMODE d3ddm;       //�f�B�X�v���C���[�h
	LPD3DXFONT pD3DFont;       //ID3DXFont�R���|�[�l���g����

	//�p�Y���摜���_
	struct MY_VERTEX{   //���_�f�[�^�̍\����
		D3DXVECTOR3 p; //�ʒu
		DWORD color;  //�f�t���[�Y�F
	};

	HRESULT init(HWND hWnd);//DirectX9������
	HRESULT verbufcr(LPDIRECT3DVERTEXBUFFER9 *pMyVB, int xrs, int yrs);//���_�o�b�t�@�[����(pMyVB�͕ԋp�l�p)
	void drawscreen();//�`��
	~Dx9Init();      //�f�X�g���N�^ 

};

#endif