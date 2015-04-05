//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(���T�C�Y)                     **//
//**                                  (ImageDraw�̊��N���X)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

void ImageRead::pic_resize(){

	int j, i;            //for
	int r, g, b, a;     //�F���
	static int xrsc, yrsc; //�������m�ۃ`�F�b�N

	switch (d.mcf){
	case 0://�Î~��
		GetSoftImageSize(g2, &xs, &ys);//�ϊ����摜�T�C�Y�擾
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
			mc = 1;//�m�ۃt���OON
		}//�������m�ۉ�������I���

		switch (d.mcf){

		case 0://�Î~��
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					GetPixelSoftImage(g2, i * xs / xrs, j * ys / yrs, &r, &g, &b, &a);       //���摜����F���擾
					imcpy1[j][i] = imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;//�摜�F���i�[�ϐ��ɐF���i�[
				}
			}
			break;

		case 1://����
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					int offset = (j * ys / yrs) * linesize + (i * xs / xrs) * 3;

					imcpy[yrs - j - 1][i] = imgpi[yrs - j - 1][i] =
						(pBuffer[offset + 2] << 16) +
						(pBuffer[offset + 1] << 8) +
						pBuffer[offset + 0];
				}
			}
			break;

		case 2://�J����
			for (j = 0; j < yrs; j++){//���T�C�Yc_img.rows
				for (i = 0; i < xrs; i++){//���T�C�Yc_img.cols

					imcpy[j][i] = imgpi[j][i] =
						(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 2] << 16) +
						(c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 1] << 8) +
						 c_img.data[(j * ys / yrs) * c_img.step + (i * xs / xrs) * c_img.channels() + 0];
				}
			}
			break;

		}//switch�I���
			
	InitSoftImage();//�\�t�g�C���[�W�S�J��
}

