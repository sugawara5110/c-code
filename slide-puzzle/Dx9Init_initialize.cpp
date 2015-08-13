//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@Dx9Init�N���X�����\�b�h��`                           **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"

// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

HRESULT Dx9Init::initialize(HWND hWnd){

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
	D3DPP.Windowed = TRUE;				         // �E�C���h�E���\���̎w��
	D3DPP.BackBufferFormat = d3ddm.Format;	// �J���[���[�h�̎w��
	D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;// 
	D3DPP.EnableAutoDepthStencil = TRUE;	  //Z�o�b�t�@
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

	//�e�}�g���b�N�X������
	D3DXMatrixIdentity(&m_view);

	D3DXMatrixIdentity(&m_proj);

	D3DXMatrixIdentity(&m_world);

	// �r���[�|�[�g�̎擾
	pD3DDevice->GetViewport(&vp);

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// �ˉe�}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian(45.0f),	// �J�����̉�p
		aspect,				   // �A�X�y�N�g��
		1.0f,		          // near�v���[��(0.0f���ƕ`�您�������B3���Y��)
		10000.0f);			 // far�v���[��

	pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
	pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);

	return S_OK;
}

HRESULT Dx9Init::getVBarray(ImageDraw *draw, int d3){//���_�o�b�t�@,�z��m��

	int n;
	if (d3 == 1)n = 4;
	else n = 8;
	
	//���_�o�b�t�@�����
	if (FAILED(pD3DDevice->CreateVertexBuffer(
		sizeof(MY_VERTEX) * draw->yrs * draw->xrs * n,// ���_�o�b�t�@�S�̂̃T�C�Y
		D3DUSAGE_WRITEONLY,		// ���_�o�b�t�@�̎g����
		MY_VERTEX_FVF,		   // ���_�f�[�^��FVF�t�H�[�}�b�g
		D3DPOOL_MANAGED, 	  // �V�X�e���̒��_�o�b�t�@������
		&draw->pMyVB, 	     // ���_�o�b�t�@���i�[����ϐ�
		NULL)))             // ���NULL�Ɏw��
	{
		return E_FAIL;
	}

	draw->img_a = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * draw->yrs * draw->xrs * n);//�p�Y���摜���_�z��m��
	
	return S_OK;
}

HRESULT Dx9Init::sclear(){//�X�N���[���N���A
	
	// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
	if (FAILED(pD3DDevice->Clear(0, NULL,   // �N���A����̈�͑S��
		D3DCLEAR_TARGET |			// �o�b�N�o�b�t�@���w��
		D3DCLEAR_ZBUFFER, 		   // �[�x�o�b�t�@�iZ�o�b�t�@�j���w��
		D3DCOLOR_XRGB(0, 0, 255), // �N���A����F
		1.0f, 					 // �N���A����[�x�o�b�t�@�iZ�o�b�t�@�j�̒l
		0)))				    // �N���A����X�e���V���o�b�t�@�̒l
	{
		return E_FAIL;
	}
	return S_OK;
}

void Dx9Init::cameraset(int xrs, int yrs, float cpx, float cpz){
	
	// �J�����̈ʒu�ƕ�����ݒ�
	D3DXMatrixLookAtLH(&m_view,
		&D3DXVECTOR3(cpx, yrs * 0.5f, cpz),          //�J�����̈ʒu
		&D3DXVECTOR3(xrs * 0.5f, yrs * 0.5f, 0.0f),//�J�����̕�����������_
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));          //�J�����̏�̕���
}

void Dx9Init::d3primitive(ImageDraw *draw, int d3){//3D�`��

	int n;
	if (d3 == 1)n = 4;
	else n = 8;

	draw->pMyVB->Lock(0, 0, (void**)&img_p, 0);
	memcpy(img_p, draw->img_a, sizeof(Dx9Init::MY_VERTEX) * draw->yrs * draw->xrs * n);
	draw->pMyVB->Unlock();
	pD3DDevice->BeginScene();// �`��J�n�錾
	// ���_�o�b�t�@�[�������_�����O�p�C�v���C���ɐݒ�
	pD3DDevice->SetStreamSource(0, draw->pMyVB, 0, sizeof(Dx9Init::MY_VERTEX));

	// �}�g���b�N�X�������_�����O�p�C�v���C���ɐݒ�
	pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

	//�e�N�X�`���[
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF);

	// �`��
	if (d3 == 1)pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, draw->yrs * draw->xrs * 4);
	else pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, draw->yrs * draw->xrs * 8);
	pD3DDevice->EndScene();// �`��I���錾
}

void Dx9Init::d2primitive(MY_VERTEX2 *fimg){//2D�`��(�����摜)

	pD3DDevice->BeginScene();// �`��J�n�錾
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// �`��
	pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 200 * 200, fimg, sizeof(MY_VERTEX2));
	pD3DDevice->EndScene();// �`��I���錾
}

void Dx9Init::d2rectangle_button(char *str, float x, float y, MY_VERTEX2 *re){//��������{�^��

	pD3DDevice->BeginScene();
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// �`��
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, re, sizeof(MY_VERTEX2));
	//����
	RECT r = { (long)x, (long)y + 2, (long)x, (long)y + 2 };
	pD3DFont->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	//�e�N�X�`���[
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	pD3DDevice->EndScene();
}

void Dx9Init::d2square_button(int num, MY_VERTEX2 *sq){//�l�p�`�{�^��,�O�p�`��

	pD3DDevice->BeginScene();
	//�e�N�X�`���[
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// �`��
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, num, sq, sizeof(MY_VERTEX2));
	pD3DDevice->EndScene();
}

void Dx9Init::line(LPD3DXLINE *pLine){//���C���`��

	D3DXCreateLine(pD3DDevice, pLine);
}

void Dx9Init::getbackimage(LPDIRECT3DTEXTURE9 *pTexture, int r){//�w�i�摜�擾

	if (r == 0)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk0.dat", pTexture);//�w�i�摜�ǂݍ���
	if (r == 1)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk1.dat", pTexture);
	if (r == 2)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk2.dat", pTexture);
	if (r == 3)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk3.dat", pTexture);
	if (r == 4)D3DXCreateTextureFromFile(pD3DDevice, "./dat/hk/hk4.dat", pTexture);
}

void Dx9Init::d2backimage(LPDIRECT3DTEXTURE9 pTexture, MY_VERTEX2 *h){//�w�i�`��

	//�e�N�X�`���[
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pD3DDevice->SetTexture(0, pTexture);
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// �`��
	pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, h, sizeof(MY_VERTEX2));
}

void Dx9Init::text(char str[30], int x, int y){//�e�L�X�g�`��

	pD3DDevice->BeginScene();
	RECT r = { x, y, x, y };
	pD3DFont->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	pD3DDevice->EndScene();
}

void Dx9Init::drawscreen(){//�X�N���[���`��

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