//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@Dx9Init�N���X�����\�b�h��`                           **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"

// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )

HRESULT Dx9Init::init(HWND hWnd){

	// Direct3D9�I�u�W�F�N�g�̍쐬
	if ((pD3D = ::Direct3DCreate9(D3D_SDK_VERSION)) == 0){
		return E_FAIL;	// �擾���s
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
	ZeroMemory(&D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	D3DPP.BackBufferCount = 1;
	D3DPP.Windowed = TRUE;				       // �E�C���h�E���\���̎w��
	D3DPP.BackBufferFormat = d3ddm.Format;	  // �J���[���[�h�̎w��
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;// 
	D3DPP.EnableAutoDepthStencil = TRUE;	   //Z�o�b�t�@
	D3DPP.AutoDepthStencilFormat = D3DFMT_D16;//Z�o�b�t�@�̃t�H�[�}�b�g

	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&D3DPP, &pD3DDevice))) {
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&D3DPP, &pD3DDevice))) {
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&D3DPP, &pD3DDevice))) {
				// ���������s
				return E_FAIL;
			}
		}
	}

	// �t�H���g�̐���
	int fontsize = 15;
	D3DXFONT_DESC lf = { fontsize, 0, 0, 1, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
		PROOF_QUALITY, FIXED_PITCH | FF_MODERN, ("�l�r �S�V�b�N") };

	// ID3DXFont�R���|�[�l���g����
	if (FAILED(D3DXCreateFontIndirect(pD3DDevice, &lf, &pD3DFont))){
		pD3DDevice->Release();  pD3D->Release();
		return E_FAIL;
	}

	//�����_�����O�X�e�[�g�p�����[�^�̐ݒ�
	// ���ʕ`�惂�[�h�̎w��
	pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// �y��r���s�Ȃ�(������Ȃ��ƕ\�����Ԃ��������Ȃ�)
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	// �f�B�U�����O���s�Ȃ��i���i���`��j
	pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	// �m�[���C�e�B���O���[�h 	
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//�A���t�@�u�����f�B���O
	pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//�_�T�C�Y
	float PointSize = 1.0f;//�L���X�g���Ďg��
	pD3DDevice->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&PointSize));
	
	return S_OK;
}

HRESULT Dx9Init::verbufcr(LPDIRECT3DVERTEXBUFFER9 *pMyVB, int xrs, int yrs){

	//���_�o�b�t�@�����
	if (FAILED(pD3DDevice->CreateVertexBuffer(
		sizeof(MY_VERTEX) * yrs * xrs * 4,// ���_�o�b�t�@�S�̂̃T�C�Y
		D3DUSAGE_WRITEONLY,		         // ���_�o�b�t�@�̎g����
		MY_VERTEX_FVF,		   // ���_�f�[�^��FVF�t�H�[�}�b�g
		D3DPOOL_MANAGED, 	  // �V�X�e���̒��_�o�b�t�@������
		pMyVB, 			     // ���_�o�b�t�@���i�[����ϐ�
		NULL)))             // ���NULL�Ɏw��
	{
		return E_FAIL;
	}
	return S_OK;
}

void Dx9Init::drawscreen(){//�`��

	if (FAILED(pD3DDevice->Present(0, 0, 0, 0))) {//�`�挋�ʂ̓]��
		// �f�o�C�X�������畜�A
		pD3DDevice->Reset(&D3DPP);
	}

}

Dx9Init::~Dx9Init(){//�f�X�g���N�^

	pD3DFont->Release();              //ID3DXFont�R���|�[�l���g
	pD3DFont = NULL;
	pD3DDevice->Release();          //IDirect3DDevice9�C���^�[�t�F�C�X
	pD3DDevice = NULL;
	pD3D->Release();	          //IDirect3D9�C���^�[�t�F�C�X
	pD3D = NULL;
}