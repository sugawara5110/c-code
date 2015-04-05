//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageDraw�N���X�����\�b�h��`(�摜�`��)                 �@�@**//
//**                                    (ImageRead�̔h���N���X)                          **//
//*****************************************************************************************//

#include "DxLib.h"
#include <math.h>
#include "back_image.h"
#include "Move.h"
#include "ImageRead.h"
#include "ImageDraw.h" 
#include "Filter.h"

void ImageDraw::draw(Filter *filter, Move *move, int x, int y){//�摜�ϊ�,�`�揈��  drawing_img������Ăяo�����(�I�[�o�[���C�h)

	int i, j, k, i1, j1; //for
	int r, g, b;        //�F
	int **pi16;        //�G�b�W,�G���{�X�����ꎞ�ۊǗp
	int mono;         //���m�N���p
	int pi;          //�G�b�W���o�p
	double ghs, gvs;//�G�b�W���o�p
	typedef struct{//�q�X�g�O�����쐬�p
		int rp;   //�Ԃ̐�
		int gp;  //�΂̐�
		int bp; //�̐�
	}his;
	static his hist[128]; //�q�X�g�O�����e�F�̊e�K����(2�K������1��)
	int xx, yy;          //�����O,������̃p�Y���摜�I�t�Z�b�g
	static int ff = 0;  //�������  0:����  1:������
	int ffr = 1;       //������ԗ��� 0:�X�V���� 1:�X�V����
	int d3_im, d3_z;  //3D�p�ꎞ�i�[�p

	if (move == NULL) {//NULL�̏ꍇ�摜���������`��
		back_image(1);
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				DrawPixel(i + (800 - xrs) / 2 + x, j + (600 - yrs) / 2, imgpi[j][i]);
			}
		}
		return;
	}//�t�@�C���֐����s���̂ݏ����I��

	/********************************************************************************************************************************/
	/********************************************�e��摜�����J�n********************************************************************/
	/********************************************************************************************************************************/

	//���m�N���摜����,�G�b�W���o,�G���{�X,�摜�G���{�X����,�G�敗����(�Î~��,����,�ǂ���ł��g�p����)gfr�̒l��puzzle_main���瑀�삵�Ă���
	if (d.gfr == 1 || d.gfr == 3 || d.gfr == 4 || d.gfr == 5 || d.gfr == 8 || d.gfr == 9) {
		pi16 = (int**)malloc(sizeof(int*) * yrs);//�G�b�W,�G���{�X,�摜�G���{�X�����ꎞ�ۊǗp�������m��
		for (i = 0; i < yrs; i++){ pi16[i] = (int*)malloc(sizeof(int) * xrs); }

		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				if (d.gfr == 1){//���m�N���摜����
					r = imgpi[j][i] >> 16 & 0xff;//0x��16�i�� 0xff�ŉ�8�r�b�g &�_����
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					mono = (r + g + b) / 3;//(�ԁ{�΁{��)��3
					imcpy[j][i] = imgpi[j][i] = (mono << 16) + (mono << 8) + mono;//�F���𕽋ω�����ƃ��m�N���ɂȂ�
				}//���m�N�������I���

				if (d.gfr == 5){//�G�敗����
					r = (imgpi[j][i] >> 16 & 0xff) / 60 * 60;
					g = (imgpi[j][i] >> 8 & 0xff) / 60 * 60;
					b = (imgpi[j][i] & 0xff) / 60 * 60;
					imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfr == 8){//�l�K�|�W����
					r = 255 - (imgpi[j][i] >> 16 & 0xff);
					g = 255 - (imgpi[j][i] >> 8 & 0xff);
					b = 255 - (imgpi[j][i] & 0xff);
					imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfr == 3 || d.gfr == 4 || d.gfr == 9){//�G�b�W���o,�G���{�X,�摜�G���{�X�O����
					r = imgpi[j][i] >> 16 & 0xff;
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					pi16[j][i] = (r + g + b) / 3;
				}//�G�b�W���o,�G���{�X�O�����I���

			}   //for�I���
		}      //for�I���

		if (d.gfr == 3 || d.gfr == 4 || d.gfr == 9) {//�G�b�W���o,�G���{�X,�摜�G���{�X�㏈��

			if (d.gfr == 9){    //�摜�G���{�X�p�摜�X�V,���ʒ���
                read->drawing_img(NULL, NULL, 0, 0, 0); //�摜�X�V
				if (read->d.mcf == 1)read->pBasicAudio->put_Volume(-2000);//���ʒ���,����ȊO���Ƃ܂��O���t������
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

					if (d.gfr == 3){                  //�G�b�W���o�v�Z
						pi = (int)(sqrt(ghs*ghs + gvs*gvs));//sqrt������(pow�֐��ł����肾���������܂������Ȃ�)
						if (pi < 50)pi = 0;         //�w�i�F(��)��0�ɂ���
						if (pi > 255)pi = 255;
						imcpy[j][i] = imgpi[j][i] = (pi << 16) + (pi << 8) + pi;
					}

					if (d.gfr == 4 || d.gfr == 9){  //�G���{�X,�摜�G���{�X�v�Z
						pi = (int)((ghs + gvs) * 0.5);//�Ƃ肠�������̌v�Z���@�ɂ���ƃG���{�X�ɂȂ�݂���
						pi += 128;          //�D�F���ۂ�������������炵��������
						if (pi < 1)pi = 1;
						if (pi > 255)pi = 255;
						imcpy[j][i] = imgpi[j][i] = (pi << 16) + (pi << 8) + pi;

						if (d.gfr == 9){//�摜�G���{�X
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
							imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;//�摜�G���{�X�v�Z�I��
						}
					}

				}//for�I���
			}   //for�I���
		}      //if �G�b�W,�G���{�X�㏈���I���
		for (i = 0; i < yrs; i++)free(pi16[i]);//�G�b�W,�G���{�X�����ꎞ�ۊǗp���������
		free(pi16);
	}//���m�N��,�G�b�W���o,�G���{�X�����I���

	//���U�C�N����(����,�Î~��ǂ���ł��g��)
	//gfr��v��mf==1����ON�Ŏ��s  gfr��v��f==1�Î~��X�V�L��Ŏ��s
	if (d.gfr == 2) {
		for (j = 0; j < yrs - 10; j += 10){
			for (i = 0; i < xrs - 10; i += 10){
				for (j1 = j; j1 < j + 10; j1++){
					for (i1 = i; i1 < i + 10; i1++){
						imcpy[j1][i1] = imgpi[j1][i1] = imgpi[j][i];//10pixel�A���œ��F�`�ʂŃ��U�C�N�ɂȂ�
					}
				}
			}
		}
	}//���U�C�N�����I���

	/********************************************************************************************************************************/
	/********************************************�e��摜�����I��********************************************************************/
	/********************************************************************************************************************************/

	//�q�X�g�O�����f�[�^����
	for (i = 0; i < 128; i++){  //������
		hist[i].rp = 0;
		hist[i].gp = 0;
		hist[i].bp = 0;
	}
	for (j = 0; j < yrs; j++){//�e�F,�e�K�����Ɍ�����
		for (i = 0; i < xrs; i++){
			r = imgpi[j][i] >> 16 & 0xff;
			g = imgpi[j][i] >> 8 & 0xff;
			b = imgpi[j][i] & 0xff;
			hist[r / 2].rp++;
			hist[g / 2].gp++;
			hist[b / 2].bp++;
		}
	}
	//�q�X�g�O�����f�[�^�����I��

	/********************************************************************************************************************************/
	/********************************************�ŏI�摜�`�揈���J�n****************************************************************/
	/********************************************************************************************************************************/

	Move::para *prs = &move->paras[move->size];
	Move::imxy *img = move->img;

	if (d.d3f == 1 || d.d3f == 2){
		//�J�����ʒu�X�V
		CameraPos.x = 400.0f + sin(d.theta_lr * 3.14 / 180) * 550;
		CameraPos.z = -550.0f + 550 - cos(d.theta_lr * 3.14 / 180) * 550;

		// �J�����̈ʒu�ƒ����_���Z�b�g
		SetCameraPositionAndTarget_UpVecY(CameraPos, VGet(400.0f, 250.0f, 0.0f));
	}

	//��������s���鏈��(�t�@�C���֐��ȊO)
	ClearDrawScreen();  //��ʏ���
	back_image(1);     //�w�i�`��֐�,����1==�摜�`��
	//�p�Y�����ȊO�̉摜�`�捶��
	for (j = 0; j < 400; j++){
		for (i = 0; i < (xrs - 400) * 0.5; i++){
			//if�G�b�W���o���̔w�i�F�`��X�L�b�v�����ȉ����������L��
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + (800 - xrs) * 0.5 + 1, j + 100, imgpi[j][i]);//+1�͕␳
		}
	}
	//�p�Y�����ȊO�̉摜�`��E��
	for (j = 0; j < 400; j++){
		for (i = xrs - (xrs - 400) * 0.5; i < xrs; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + (800 - xrs) * 0.5 - 1, j + 100, imgpi[j][i]);
		}
	}
	//�p�Y�����ȊO�̉摜�`��㑤
	for (j = 0; j < (yrs - 400) * 0.5; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + 200, j + (600 - yrs) * 0.5, imgpi[j][i]);
		}
	}
	//�p�Y�����ȊO�̉摜�`�扺��
	for (j = yrs - (yrs - 400) * 0.5; j < yrs; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			DrawPixel(i + 200, j + (600 - yrs) * 0.5, imgpi[j][i]);
		}
	}
	//�p�Y���Ԃ̉摜�`��
	if (move->size != 5){//size5�͏������Ȃ�
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (d.gfr == 3 && imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] == 0)continue;
				if (filter->block[j][i] == 150 || filter->block[j][i] == 1){//�t�B���^�[�l150,1�̂ݕ`��
					DrawPixel(i + 200, j + 100, imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2]);
				}
			}
		}
	}

	//�p�Y���������菈��,�����O������̃I�t�Z�b�g����
	for (i = 0; i < prs->idx; i++){
		if (img[i].cx != img[i].fx || img[i].cy != img[i].fy)break;
	}
	if (i == prs->idx && ff == 0)ffr = 0;               //����,�X�V����
	if (i == prs->idx && ff == 1){ ffr = 1; ff = 0; }  //����,�X�V�L��
	if (i != prs->idx && ff == 1)ffr = 0;             //������,�X�V����
	if (i != prs->idx && ff == 0){ ffr = 1; ff = 1; }//������,�X�V�L��

	//�u���b�N�摜����
	if (ff == 1 && move->size < 5){
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (filter->block[j][i] == 0)continue;   //�F�ϊ������X�L�b�v
				//�����̂܂ܕϊ�����ƐF���ς���Ă����Ă��܂��̂ň�U���ɖ߂�
				imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] = imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2];
				//���F�擾�ϊ�
				r = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff;
				g = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff;
				b = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff;
				r -= filter->block[j][i];
				g -= filter->block[j][i];
				b -= filter->block[j][i];
				if (r <= 0)r = 1;
				if (g <= 0)g = 1;
				if (b <= 0)b = 1;
				imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] = GetColor(r, g, b);//�F���
			}
		}
	}//�u���b�N�摜�����I���

	//�����u���b�N����
	if (ff == 0 && d.mcf == 0){    //�����ςݐÎ~�揈��
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				if (d.gfr == 3 && imgpi[j][i] == 0)continue;
				imgpi[j][i] = imcpy[j][i];//�u���b�N�摜�����O�f�[�^���R�s�[
			}
		}
	}

	//����,���������㏈��(�ċA��1�x�������������)
	if (ffr == 1){
		if (ff == 0){//�������㏈��
			for (i = 0; i < 19; i++){
				drawing_img(filter, move, 19 - i, -19 + i, 0);//�ċA
				ScreenFlip();
			}
		}
		if (ff == 1){//���������㏈��
			for (i = 0; i < 19; i++){
				drawing_img(filter, move, i - 19, -i + 19, 0);//�ċA(19��xx,yy�̕��̕␳)
				ScreenFlip();
			}
		}
	}//����,���������㏈���I���

	if (ff == 0){ xx = 0; yy = 0; }//������ԂŃI�t�Z�b�g�ʍX�V
	else{ xx = 19; yy = -19; }

	//�p�Y�����摜�`�揈���J�n
	for (i = 0; i < prs->idx; i++){
		for (j = 0; j < prs->bsize; j++){     //1�u���b�N��y����
			for (k = 0; k < prs->bsize; k++){//1�u���b�N��x����

				//�G�b�W���o����
				if (d.gfr == 3 && imgpi[j + img[i].fy - 100 + (yrs - 400) / 2]
					[k + img[i].fx - 200 + (xrs - 400) / 2] == 0)continue;

				//�u���b�N�Ԃ͕`�悵�Ȃ�(�t�B���^�[�l150,1 size==5)
				if ((filter->block[j + img[i].fy - 100][k + img[i].fx - 200] == 150 ||
					filter->block[j + img[i].fy - 100][k + img[i].fx - 200] == 1) &&
					move->size != 5)continue;

				if (d.d3f == 1 || d.d3f == 2){
					//3D�\��
					d3_im = imgpi[j + img[i].fy - 100 + (yrs - 400) / 2]
						[k + img[i].fx - 200 + (xrs - 400) / 2];
					if (d.d3f == 2)d3_z = ((d3_im >> 16) & 0xff + (d3_im >> 8) & 0xff + d3_im & 0xff) / 6;
					else d3_z = 0;
					DrawLine3D(VGet(k + img[i].cx + xx + x, 550 - (j + img[i].cy + yy + y),
						       d3_z + 20),
						       VGet(k + img[i].cx + xx + x, 550 - (j + img[i].cy + yy + y),
						       -(d3_z + 20)),
						       d3_im);
				}
				else{
					//�ʏ�\��,�\�����W�ʒu�{�I�t�Z�b�g, �\���F���(�������W�ʒu�|�I�t�Z�b�g)
					DrawPixel(k + img[i].cx + xx + x, j + img[i].cy + yy + y,
						imgpi[j + img[i].fy - 100 + (yrs - 400) / 2]
						[k + img[i].fx - 200 + (xrs - 400) / 2]);
				}

			}
		}
	}//�p�Y�����摜�`�揈���I���

	for (i = 0; i < 128; i++){//�q�X�g�O�����`��
		DrawLine(640 + i, 200, 640 + i, 200 - hist[i].rp / 80, GetColor(i * 2, 0, 0));
		DrawLine(640 + i, 300, 640 + i, 300 - hist[i].gp / 80, GetColor(0, i * 2, 0));
		DrawLine(640 + i, 400, 640 + i, 400 - hist[i].bp / 80, GetColor(0, 0, i * 2));
	}//�q�X�g�O�����`��I��

	//������摜�`��
	if (finish == 1){
		for (j = 0; j < 200; j++){
			for (i = 0; i < 200; i++){
				if (d.gfr == 3 && imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2] == 0)continue;
				DrawPixel(i + 600, j + 400, imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2]);
			}
		}
	}
	/********************************************************************************************************************************/
	/********************************************�ŏI�摜�`�揈���I��****************************************************************/
	/********************************************************************************************************************************/

}

void ImageDraw::obj_create(char *name){//�I�u�W�F�N�g�����֐�

	if (read == NULL){//�I�u�W�F�N�g��������Ă��Ȃ����̂ݎ��s
		read = new ImageRead(name);//�摜�G���{�X�p�I�u�W�F�N�g����
	}
}

void ImageDraw::obj_delete(){//�I�u�W�F�N�g�j���֐�

	if (read != NULL){//�I�u�W�F�N�g��������Ă��鎞�̂ݎ��s
		read->d.cap.release();         //�J�����I������
		delete read;                  //�I�u�W�F�N�g�j��
		read = NULL;
	}
}

ImageDraw::ImageDraw(){}//�K��R���X�g���N�^

ImageDraw::ImageDraw(char *name):ImageRead(name){

	read = NULL;
	back_image(0);//�w�i�`��֐�,����0==�摜���n���h���Ɋi�[
}

ImageDraw::~ImageDraw(){//�f�X�g���N�^

	obj_delete();//�I�u�W�F�N�g�j���֐�

}