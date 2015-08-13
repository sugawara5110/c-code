//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(�摜�ϊ�)                     **//
//**                                    (ImageDraw�̊��N���X)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"

HRESULT ImageDraw::processing_img(){ //�摜�ϊ�

	int i, j, i1, j1, k; //for
	int temp;
	int r, g, b;        //�F
	int mono;          //���m�N���p
	int pi;           //�G�b�W���o�p
	double ghs, gvs; //�G�b�W���o�p
	static int kr = 1;//�X���b�g�X�L�����o�̓y�[�W
	int krs;         //�X���b�g�X�L�����X�V�y�[�W�X�^�[�g�ʒu
	int noise[9];   //�m�C�Y�����ꎞ�i�[

	/********************************************************************************************************************************/
	/********************************************�e��摜�����J�n********************************************************************/
	/********************************************************************************************************************************/

	//�G�b�W,�G���{�X,�摜�G���{�X,�m�C�Y���������ꎞ�ۊǗp�������m��
	if (pi16 == NULL && (d.gfl == 3 || d.gfl == 4 || d.gfl == 9 || d.gfl == 12)){
		pi16 = (int**)malloc(sizeof(int*) * yrs);
		for (i = 0; i < yrs; i++){ pi16[i] = (int*)malloc(sizeof(int) * xrs); }
	}
	//�G�b�W,�G���{�X,�摜�G���{�X,�m�C�Y���������ꎞ�ۊǗp���������
	if (pi16 != NULL && (d.gfl != 3 && d.gfl != 4 && d.gfl != 9 && d.gfl != 12)){
		for (i = 0; i < yrs; i++)free(pi16[i]);
		free(pi16);
		pi16 = NULL;
	}

	//�X���b�g�X�L�����p�������m��
	if (slit == NULL && d.gfl == 11){
		int kyrs = (int)(yrs * 0.5);
		slit = (int*)malloc(sizeof(int) * kyrs * yrs * xrs);//�������m��
		for (k = 0; k < kyrs; k++){
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					slit[k * yrs * xrs + j * xrs + i] = imgpi[j][i];//������
				}
			}
		}
	}
	//�X���b�g�X�L�����p���������
	if (slit != NULL && d.gfl != 11){ free(slit); slit = NULL; }

	//���m�N���摜����,�G�b�W���o,�G���{�X,�摜�G���{�X����,�G�敗����(�Î~��,����,�ǂ���ł��g�p����)
	if (d.gfl == 1 || d.gfl == 3 || d.gfl == 4 || d.gfl == 5 || d.gfl == 8 || d.gfl == 9) {

		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				if (d.gfl == 1){//���m�N���摜����
					r = imgpi[j][i] >> 16 & 0xff;//0x��16�i�� 0xff�ŉ�8�r�b�g &�_����
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					mono = (r + g + b) / 3;//(�ԁ{�΁{��)��3
					imgpi[j][i] = (mono << 16) + (mono << 8) + mono;//�F���𕽋ω�����ƃ��m�N���ɂȂ�
				}//���m�N�������I���

				if (d.gfl == 5){//�G�敗����
					r = (imgpi[j][i] >> 16 & 0xff) / 60 * 60;
					g = (imgpi[j][i] >> 8 & 0xff) / 60 * 60;
					b = (imgpi[j][i] & 0xff) / 60 * 60;
					imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfl == 8){//�l�K�|�W����
					r = 255 - (imgpi[j][i] >> 16 & 0xff);
					g = 255 - (imgpi[j][i] >> 8 & 0xff);
					b = 255 - (imgpi[j][i] & 0xff);
					imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfl == 3 || d.gfl == 4 || d.gfl == 9){//�G�b�W���o,�G���{�X,�摜�G���{�X�O����
					r = imgpi[j][i] >> 16 & 0xff;
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					pi16[j][i] = (r + g + b) / 3;
				}//�G�b�W���o,�G���{�X�O�����I���

			}   //for�I���
		}      //for�I���

		if (d.gfl == 3 || d.gfl == 4 || d.gfl == 9) {//�G�b�W���o,�G���{�X,�摜�G���{�X�㏈��

			if (d.gfl == 9){       //�摜�G���{�X�p�摜�X�V,���ʒ���
				read->read_img(); //�摜�X�V
				if (read->getmcf() == 1)read->putVol(-2000);//���ʒ���,����ȊO���Ƃ܂��O���t������
			}

			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					if (i == 0 || i == xrs - 1 || j == 0 || j == yrs - 1){//�z��͈͊O�܂œǂ�ł��܂��̂ŊO���͌v�Z�Ɋ܂܂Ȃ��悤�ɂ���
						imgpi[j][i] = (1 << 16) + (1 << 8) + 1; continue;
					}
					//��������ghs 
					ghs = pi16[j - 1][i - 1] * -1;     //����,�~-1���W��,�v�Z���ĂȂ��s�N�Z���͌W��0
					ghs += pi16[j][i - 1] * -2;       //��
					ghs += pi16[j + 1][i - 1] * -1;  //����
					ghs += pi16[j - 1][i + 1] * 1;  //�E��
					ghs += pi16[j][i + 1] * 2;     //�E
					ghs += pi16[j + 1][i + 1] * 1;//�E��
					//��������gvs
					gvs = pi16[j - 1][i - 1] * -1;     //����
					gvs += pi16[j - 1][i] * -2;       //��
					gvs += pi16[j - 1][i + 1] * -1;  //�E��
					gvs += pi16[j + 1][i - 1] * 1;  //����
					gvs += pi16[j + 1][i] * 2;     //��
					gvs += pi16[j + 1][i + 1] * 1;//�E��

					if (d.gfl == 3){                  //�G�b�W���o�v�Z
						pi = (int)(sqrt(ghs*ghs + gvs*gvs));//sqrt������(pow�֐��ł����肾���������܂������Ȃ�)
						if (pi < 50)pi = 0;         //�w�i�F(��)��0�ɂ���
						if (pi > 255)pi = 255;
						imgpi[j][i] = (pi << 16) + (pi << 8) + pi;
					}

					if (d.gfl == 4 || d.gfl == 9){  //�G���{�X,�摜�G���{�X�v�Z
						pi = (int)((ghs + gvs) * 0.5);//�Ƃ肠�������̌v�Z���@�ɂ���ƃG���{�X�ɂȂ�݂���
						pi += 128;          //�D�F���ۂ�������������炵��������
						if (pi < 1)pi = 1;
						if (pi > 255)pi = 255;
						imgpi[j][i] = (pi << 16) + (pi << 8) + pi;

						if (d.gfl == 9){//�摜�G���{�X
							pi -= 128;
							if (xrs == read->xrs && yrs == read->yrs){//�T�C�Y����
								r = (read->imgpi[j][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i] & 0xff) - pi;
							}

							if (xrs > read->xrs && yrs == read->yrs){//���摜�̕���x�傫��,y����
								if (i <= (xrs - read->xrs) * 0.5 || i >= (xrs - read->xrs) * 0.5 + read->xrs)continue;
								r = (read->imgpi[j][i - (xrs - read->xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i - (xrs - read->xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i - (xrs - read->xrs) / 2] & 0xff) - pi;
							}

							if (xrs < read->xrs && yrs == read->yrs){//���摜�̕���x������,y����
								r = (read->imgpi[j][i + (read->xrs - xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i + (read->xrs - xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i + (read->xrs - xrs) / 2] & 0xff) - pi;
							}

							if (yrs > read->yrs && xrs == read->xrs){//���摜�̕���y�傫��,x����
								if (j <= (yrs - read->yrs) * 0.5 || j >= (yrs - read->yrs) * 0.5 + read->yrs)continue;
								r = (read->imgpi[j - (yrs - read->yrs) / 2][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j - (yrs - read->yrs) / 2][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j - (yrs - read->yrs) / 2][i] & 0xff) - pi;
							}

							if (yrs < read->yrs && xrs == read->xrs){//���摜�̕���y������,x����
								r = (read->imgpi[j + (read->yrs - yrs) / 2][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j + (read->yrs - yrs) / 2][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j + (read->yrs - yrs) / 2][i] & 0xff) - pi;
							}

							if (yrs < read->yrs && xrs > read->xrs){//���摜�̕���y������,���摜�̕���x�傫��
								if (i <= (xrs - read->xrs) * 0.5 || i >= (xrs - read->xrs) * 0.5 + read->xrs)continue;
								r = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] & 0xff) - pi;
							}

							if (yrs > read->yrs && xrs < read->xrs){//���摜�̕���y�傫��,���摜�̕���x������
								if (j <= (yrs - read->yrs) * 0.5 || j >= (yrs - read->yrs) * 0.5 + read->yrs)continue;
								r = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] & 0xff) - pi;
							}

							if (r < 1)r = 1;
							if (r > 255)r = 255;
							if (g < 1)g = 1;
							if (g > 255)g = 255;
							if (b < 1)b = 1;
							if (b > 255)b = 255;
							imgpi[j][i] = (r << 16) + (g << 8) + b;//�摜�G���{�X�v�Z�I��
						}
					}

				}//for�I���
			}   //for�I���
		}      //if �G�b�W,�G���{�X�㏈���I���

	}//���m�N��,�G�b�W���o,�G���{�X����,�G�敗�����I���

	//���U�C�N����(����,�Î~��ǂ���ł��g��)
	if (d.gfl == 2) {
		for (j = 0; j < yrs - 10; j += 10){
			for (i = 0; i < xrs - 10; i += 10){
				for (j1 = j; j1 < j + 10; j1++){
					for (i1 = i; i1 < i + 10; i1++){
						imgpi[j1][i1] = imgpi[j][i];//10pixel�A���œ��F�`�ʂŃ��U�C�N�ɂȂ�
					}
				}
			}
		}
	}//���U�C�N�����I���

	//�X���b�g�X�L��������(����,�J�����̂ݏ���,�Î~��̏ꍇ�̓��j���[�ɏo�Ȃ�)
	if (d.gfl == 11){
		int kyrs = (int)(yrs * 0.5);
		krs = kr;//�X�V�y�[�W
		for (j = 0; j < yrs; j += 2){
			krs = krs%kyrs + 1;
			for (i = 0; i < xrs; i++){
				slit[(krs - 1) * yrs * xrs + j * xrs + i] = imgpi[j][i];//�X���b�g�X�L�����ꎞ�ۊǍX�V
				if (j < yrs - 1){
					slit[(krs - 1) * yrs * xrs + (j + 1) * xrs + i] = imgpi[j][i];//�X���b�g�X�L�����ꎞ�ۊǍX
				}
			}
		}

		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				imgpi[j][i] = slit[(kr - 1) * yrs * xrs + j * xrs + i];//�o�͉摜�X�V
			}
		}
		kr = kr%kyrs + 1;
	}

	//�m�C�Y����
	if (d.gfl == 12){
		for (j = 1; j < yrs - 1; j++){
			for (i = 1; i < xrs - 1; i++){
				noise[0] = imgpi[j - 1][i - 1];    //����
				noise[1] = imgpi[j - 1][i];       //��
				noise[2] = imgpi[j - 1][i + 1];  //�E��
				noise[3] = imgpi[j][i - 1];     //��
				noise[4] = imgpi[j][i];        //�^��
				noise[5] = imgpi[j][i + 1];       //�E
				noise[6] = imgpi[j + 1][i - 1];  //����
				noise[7] = imgpi[j + 1][i];     //��
				noise[8] = imgpi[j + 1][i + 1];//�E��

				//3*3�̈���ёւ��Œ��Ԃ̗v�f���m��,�i�[
				for (j1 = 8; j1 >= 6; j1--){
					for (i1 = 0; i1 < j1; i1++){
						if (noise[i1] > noise[i1 + 1]){
							temp = noise[i1];
							noise[i1] = noise[i1 + 1];
							noise[i1 + 1] = temp;
						}
					}
				}
				pi16[j][i] = noise[5];//���ёւ���̒����l���ꎞ�i�[
			}
		}
		//�R�s�[
		for (j = 1; j < yrs - 1; j++){
			for (i = 1; i < xrs; i++){
				imgpi[j][i] = pi16[j][i];
			}
		}
	}

	return S_OK;
}