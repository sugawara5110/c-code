//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(���T�C�Y)                     **//
//**                                  (ImageDraw�̊��N���X)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

int ImageRead::resize(){

	int j, i;//for
	
	//�s�N�Z���z��m��
	if (imgpi == NULL){
		//���T�C�Y�O�摜�T�C�Y�擾
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
			xrs = yrs = 400;
		}

		if (xs > ys){//���摜��x�̕����T�C�Y�傫��(����������400�Œ�)
			xrs = (int)((double)xs / ys * 400);
			yrs = 400;
		}

		if (xs < ys){ //���摜��y�̕����T�C�Y�傫��(����������400�Œ�)
			xrs = 400;
			yrs = (int)((double)ys / xs * 400);
		}

		imgpi = (int**)malloc(sizeof(int*) * yrs);
		for (i = 0; i < yrs; i++){
			imgpi[i] = (int*)malloc(sizeof(int) * xrs);
		}
	}//�s�N�Z���z��m�ۊ���

	//�摜���T�C�Y
	switch (d.mcf){

	case 0://�Î~��
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				imgpi[yrs - j - 1][i] =
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

				imgpi[j][i] =
					(pBuffer[offset + 2] << 16) +
					(pBuffer[offset + 1] << 8) +
					pBuffer[offset + 0];
			}
		}
		break;

	case 2://�J����
		for (j = 0; j < yrs; j++){//���T�C�Yc_img.rows
			for (i = 0; i < xrs; i++){//���T�C�Yc_img.cols

				imgpi[yrs - j - 1][i] =
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 2] << 16) +
					(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 1] << 8) +
					c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 0];
			}
		}
		break;

	}//�摜���T�C�Y����

	return 0;
}

