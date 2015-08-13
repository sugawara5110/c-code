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

ImageDraw::ImageDraw(Dx9Init *dx, char *name) :ImageRead(name){//�R���X�g���N�^

	//�t�B���^�[�p�z��m��
	block = (int**)malloc(sizeof(int*) * 400);
	for (int i = 0; i < 400; i++)block[i] = (int*)malloc(sizeof(int) * 400);

	read = NULL;
	back_image(dx, 0); //�w�i�`��֐�,����0==�摜���n���h���Ɋi�[
	dx->line(&pLine); //���C���`��
	theta_lr = 0;   //3D���[�h�J�����p�x
	ver = 0;        //0:�`����s
	pi16 = NULL;   //�G�b�W,�G���{�X,�m�C�Y�����ꎞ�ۊ�
	slit = NULL;  //�X���b�g�X�L�����ꎞ�ۊ�
	pMyVB = NULL;//���_�o�b�t�@
	d3 = 1;         //3D���[�h�ؑ�
	finish = false;//�����摜ON OFF
}

void ImageDraw::releaseVB(){//���_�o�b�t�@,�z����

	if (pMyVB != NULL){
		pMyVB->Release(); pMyVB = NULL;
		free(img_a);
	}
}

void ImageDraw::obj_create(Dx9Init *dx, char *name){//�I�u�W�F�N�g�����֐�

	if (read == NULL){//�I�u�W�F�N�g��������Ă��Ȃ����̂ݎ��s
		read = new ImageRead(name);//�摜�G���{�X�p�I�u�W�F�N�g����
	}
}

void ImageDraw::obj_delete(){//�I�u�W�F�N�g�j���֐�

	if (read != NULL){//�I�u�W�F�N�g��������Ă��鎞�̂ݎ��s
		delete read;                  //�I�u�W�F�N�g�j��
		read = NULL;
	}
}

void ImageDraw::putfin(bool fin){ //�����摜ON OFF����
	finish = fin;
}

int ImageDraw::theta(int f, int t){//�J�����p�x����
	if (f == 1)theta_lr += t;
	if (f == 2)theta_lr -= t;
	if (theta_lr < 0)theta_lr = 360;
	if (theta_lr > 360)theta_lr = 0;
	return theta_lr;
}

ImageDraw::~ImageDraw(){//�f�X�g���N�^

	//���_�o�b�t�@,�z����
	releaseVB();

	//�t�B���^�[�p�z����
	for (int i = 0; i < 400; i++)free(block[i]);
	free(block);

	pLine->Release();//����������
	pLine = NULL;
	obj_delete();//�I�u�W�F�N�g�j���֐�

	if (pi16 != NULL){
		for (int i = 0; i < yrs; i++)free(pi16[i]);//�G�b�W�G���{�X�ꎞ�ۊǃ��������
		free(pi16);
		pi16 = NULL;
	}

	if (slit != NULL)free(slit);//�X���b�g�X�L�����ꎞ�ۊǃ��������
}
