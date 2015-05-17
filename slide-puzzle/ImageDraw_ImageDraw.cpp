//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageDraw�N���X�����\�b�h��`�R���X�g���N�^��               **//
//**                                    (ImageRead�̔h���N���X)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h" 
#include "back_image.h"

ImageDraw::ImageDraw(){}//�K��R���X�g���N�^

ImageDraw::ImageDraw(Dx9Init *dx, char *name) :ImageRead(dx, name){//�R���X�g���N�^

	read = NULL;
	back_image(dx, 0);//�w�i�`��֐�,����0==�摜���n���h���Ɋi�[
	D3DXCreateLine(dx->pD3DDevice, &pLine);//���Ђ����
	theta_lr = 0;     //3D���[�h�J�����p�x

	//�e�}�g���b�N�X������
	D3DXMatrixIdentity(&m_view);

	D3DXMatrixIdentity(&m_proj);

	D3DXMatrixIdentity(&m_world);

	// �r���[�|�[�g�̎擾
	dx->pD3DDevice->GetViewport(&vp);

	// �A�X�y�N�g��̌v�Z
	float aspect;
	aspect = (float)vp.Width / (float)vp.Height;

	// �ˉe�}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_proj,
		D3DXToRadian(45.0f),	// �J�����̉�p
		aspect,				   // �A�X�y�N�g��
		1.0f,		          // near�v���[��(0.0f���ƕ`�您�������B3���Y��)
		10000.0f);			 // far�v���[��

	dx->pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_proj);
	dx->pD3DDevice->SetTransform(D3DTS_WORLD, &m_world);
}

ImageDraw::~ImageDraw(){//�f�X�g���N�^

	pLine->Release();//����������
	pLine = NULL;
	obj_delete();//�I�u�W�F�N�g�j���֐�
}

void ImageDraw::obj_create(Dx9Init *dx, char *name){//�I�u�W�F�N�g�����֐�

	if (read == NULL){//�I�u�W�F�N�g��������Ă��Ȃ����̂ݎ��s
		read = new ImageRead(dx, name);//�摜�G���{�X�p�I�u�W�F�N�g����
	}
}

void ImageDraw::obj_delete(){//�I�u�W�F�N�g�j���֐�

	if (read != NULL){//�I�u�W�F�N�g��������Ă��鎞�̂ݎ��s
		read->d.cap.release();         //�J�����I������
		delete read;                  //�I�u�W�F�N�g�j��
		read = NULL;
	}
}

