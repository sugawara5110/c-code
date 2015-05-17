//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(���T�C�Y)                     **//
//**                                  (ImageDraw�̊��N���X)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )

int ImageRead::pic_resize(Dx9Init *dx){

	int j, i;            //for
	static int xrsc, yrsc; //�������m�ۃ`�F�b�N

	switch (d.mcf){
	case 0://�Î~��
		xs = g2.cols;
		ys = g2.rows;
		break;

	case 1://����
		xs = pVideoInfoHeader->bmiHeader.biWidth;
		ys = pVideoInfoHeader->bmiHeader.biHeight;
		break;

	case 2://�J����
		xs = c_img.cols;
		ys = c_img.rows;
		break;

	}//switch

	if (xs == ys){//���摜��xy�������T�C�Y
		xrsc = yrsc = 400;
	}

	if (xs > ys){//���摜��x�̕����T�C�Y�傫��(����������400�Œ�)
		xrsc = (int)((double)xs / ys * 400);
		yrsc = 400;
	}

	if (xs < ys){ //���摜��y�̕����T�C�Y�傫��(����������400�Œ�)
		xrsc = 400;
		yrsc = (int)((double)ys / xs * 400);
	}

	if (xrs != xrsc || yrs != yrsc){  //�ǂ��炩���^�̏ꍇ�摜�ύX���ꂽ�Ɣ��f,���������,�������Ċm��
		if (mc == 1){                //mc==0�̏ꍇ�̓������m�ۂ��ĂȂ��׉���������Ȃ�(�A�N�Z�X�ᔽ�h�~)
			//���X�V�O�摜�T�C�Y���̉���Ȃ̂�xrsc,yrsc��xrs,yrs�X�V�O�ɏ���
			for (i = 0; i < yrs; i++){ free(imgpi[i]); free(imcpy[i]); free(imcpy1[i]); }
			free(imgpi); free(imcpy); free(imcpy1);
			pMyVB->Release(); pMyVB = NULL;//���_�o�b�t�@���
		}
		xrs = xrsc; //�R�s�[
		yrs = yrsc;//�R�s�[
		imgpi = (int**)malloc(sizeof(int*) * yrs);//�X�V��摜�T�C�Y���̊m�ۂȂ̂�xrsc,yrsc��xrs,yrs�X�V��ɏ���
		imcpy = (int**)malloc(sizeof(int*) * yrs);
		imcpy1 = (int**)malloc(sizeof(int*) * yrs);
		for (i = 0; i < yrs; i++){
			imgpi[i] = (int*)malloc(sizeof(int) * xrs);
			imcpy[i] = (int*)malloc(sizeof(int) * xrs);
			imcpy1[i] = (int*)malloc(sizeof(int) * xrs);
		}
		
		if (d3 == 1){ if (FAILED(dx->verbufcr(&pMyVB, xrs, yrs)))return -1; }//���_�o�b�t�@�[�m��(�ʏ�)
		else  { if (FAILED(dx->verbufcr(&pMyVB, xrs, yrs * 2)))return -1; } //���_�o�b�t�@�[�m��(Z��������*2)

		mc = 1;//�m�ۃt���OON
	}//�������m�ۉ�������I���

	switch (d.mcf){

	case 0://�Î~��
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				imcpy1[yrs - j - 1][i] = imcpy[yrs - j - 1][i] = imgpi[yrs - j - 1][i] =
					(g2.data[(j * ys / yrs) * g2.step + (i * xs / xrs) * g2.channels() + 2] << 16) +
					(g2.data[(j * ys / yrs) * g2.step + (i * xs / xrs) * g2.channels() + 1] << 8) +
					g2.data[(j * ys / yrs) * g2.step + (i * xs / xrs) * g2.channels() + 0];
			}
		}
		break;

	case 1://����
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				int offset = (j * ys / yrs) * linesize + (i * xs / xrs) * 3;

				imcpy[j][i] = imgpi[j][i] =
					(pBuffer[offset + 2] << 16) +
					(pBuffer[offset + 1] << 8) +
					pBuffer[offset + 0];
			}
		}
		break;

	case 2://�J����
		for (j = 0; j < yrs; j++){//���T�C�Yc_img.rows
			for (i = 0; i < xrs; i++){//���T�C�Yc_img.cols

				imcpy[yrs - j - 1][i] = imgpi[yrs - j - 1][i] =
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 2] << 16) +
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 1] << 8) +
					c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 0];
			}
		}
		break;

	}//switch�I���
	return 0;
}

