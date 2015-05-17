//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@Move�N���X�����\�b�h��`(�R���X�g���N�^)                    **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Filter.h"

Move::Move(){//�R���X�g���N�^

	tkf = 0;              //�萔�v�Z���s�t���O
	tkc = 0;             //�萔�v�Z�J�E���g
	cnt_results = 0;    //auto_matic�֐��̃J�E���g����
	size = 0;          //�G���[�h�~ 
	br = 0;           //�A�v���I��

	paras[0] = { 16, 4, 100, 300, 300, 400, 15, 31, 5, 10 };    //�u���b�N�����̃p�����[�^�[
	paras[1] = { 64, 8, 50, 350, 350, 800, 63, 127, 5, 10 };
	paras[2] = { 256, 16, 25, 375, 375, 1000, 255, 511, 5, 5 };
	paras[3] = { 1600, 40, 10, 390, 390, 2000, 1599, 3199, 2, 5 };
	paras[4] = { 10000, 100, 4, 396, 396, 9000, 9999, 19999, 2, 4 };
	paras[5] = { 160000, 400, 1, 399, 399, 300000, 159999, 319999, 1, 1 };

	img = (Move::imxy*)malloc(sizeof(Move::imxy) * paras[size].idx);//�摜���W�m��

	int k = 0;
	for (int j = 0; j <= paras[size].lastposy; j += paras[size].bsize){    //���W�f�[�^������
		for (int i = 0; i <= paras[size].lastposx; i += paras[size].bsize){
			img[k].cz = img[k].chz = 0;
			img[k].cx = img[k].fx = img[k].chx = i;
			img[k].cy = img[k].fy = img[k++].chy = j;
		}
	}
}

Move::~Move(){//�f�X�g���N�^

	free(img);//���W���
}

void Move::coordinates(int si){//���W�Ď擾

	size = si;//size���
	free(img);//���W���
	img = (Move::imxy*)malloc(sizeof(Move::imxy) * paras[size].idx);//�摜���W�m��

	int k = 0;
	for (int j = 0; j <= paras[size].lastposy; j += paras[size].bsize){    //���W�f�[�^������
		for (int i = 0; i <= paras[size].lastposx; i += paras[size].bsize){
			img[k].cz = img[k].chz = 0;
			img[k].cx = img[k].fx = img[k].chx = i;
			img[k].cy = img[k].fy = img[k++].chy = j;
		}
	}
}
