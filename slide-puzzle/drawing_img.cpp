//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�摜�`��֐�                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include "struct_set.h"
#include "back_image.h"
#include "pic_resize.h"
#include "ewclib.h"//�J�����p���C�u����

void drawing_img(alldata *p, int x, int y, int f){ //x,y�͕`�掞�̃I�t�Z�b�g��, f 0:�Î~��X�V��, 1:�Î~��X�V�L, 2,3,4:�t�@�C���֐��p(����͏�ɍX�V)
	
	int i, j, k, i1, j1;    //for
	int rd, rd1, rd2, rd3; //�����_��
	int r, g, b;          //�F
	int g2;              //���摜�n���h��
	int stl;            //�摜�t�@�C���������񒷂�
	int **pi16;        //�G�b�W,�G���{�X�����ꎞ�ۊǗp
	int mf = 0;       //�Î~��,���攻��  0:�Î~��,1:����
	int camf = 0;          //�J�����t�@�C���I�� 0:�I�����ĂȂ� 1:�I�����Ă���
	static int xs, ys;    //����`��O�����p�̉摜�T�C�Y
	static int buffer[320 * 240];//�J�����摜�p�o�b�t�@(1�v�f�ɐF��񂪓����Ă�)
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
	int temp;         //�l�����p
	
	para *prs = &p->paras[p->size];
	imxy *img = p->img;

	if (f == 4){ EWC_Close(0); return; }//�J�����I������

	stl = strlen(p->g_name) - 4;
	if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi"))mf = 1;//�Î~��,���攻�ʍX�V
	if (!strcmp(p->g_name, "z_cam_ewc.bmp")){ mf = 1; camf = 1;}//�J�����t�@�C���I��, ���攻��ON, �J�����t�@�C���I��ON
		
	if (mf == 1) {        //����t�@�C���I�[�v��,�`��,���T�C�Y����
		if (camf == 0){  //�J�����t�@�C���I�����Ă��Ȃ��ꍇ�͓���t�@�C��������
			if (GetMovieStateToGraph(p->mof) != 1){//����Đ�,��~���ʗp�n���h�����Đ��p�Ɏg��(file�֐��ł̔��ʂɂ��g�p���Ă�)
				p->mof = LoadGraph(p->g_name);    //�摜�t�@�C��������摜�ǂݍ���
				GetGraphSize(p->mof, &xs, &ys);  //�T�C�Y�ǂݍ���
				PlayMovieToGraph(p->mof);       //����t�@�C��open
			}
			DrawGraph(0, 0, p->mof, TRUE);    //����t�@�C��open��̕`��̑O�����p�̕`��                
			SaveDrawScreen(0, 0, xs, ys, "./save/save.bmp");//�`�悵���摜�̕ۑ�
			g2 = LoadSoftImage("./save/save.bmp");         //�ۑ������摜�̃\�t�g�C���[�W�ǂݍ���(g2==���摜�\�t�g�C���[�W)
		}
		else {//�J�����t�@�C���I�����Ă�ꍇ
			g2 = MakeXRGB8ColorSoftImage(320, 240);//�n���h���ɃJ���摜�ݒ�

			if (EWC_GetCamera() == 0){      //�J�������ڑ�,����OPEN����ĂȂ��ꍇ�J����OPEN����
				EWC_Open(0, 320, 240, 30.);//�J����open �J�������ʔԍ�,x��,y��,FPS(�ŏ��������s�K�v)
			}

			EWC_GetImage(0, buffer);//EWCLIB�����ɂ���t���[���o�b�t�@���� buffer �։摜�f�[�^���R�s�[
			k = 0;
			for (j = 0; j < 240; j++){
				for (i = 0; i < 320; i++){
					r = buffer[k] >> 16;//�e�F���(�e8bit�v24bit)���V�t�g���Z�Ŏ��o��
					g = (buffer[k] - (r << 16)) >> 8;
					b = buffer[k++] - (r << 16) - (g << 8);
					DrawPixelSoftImage(g2, i, j, r, g, b, 0);//�J���C���[�W�n���h���Ɏ擾�����F����`��(g2==���摜�\�t�g�C���[�W)
				}
			}
			if (EWC_GetCamera() == 0){ DeleteGraph(g2); g2 = LoadSoftImage("z_cam_ewc.bmp"); }//�J�������ڑ��̏ꍇ"cam_ewc.bmp"�����̂܂�cam�Ɋi�[
		}//else�I���(�J��������)

		pic_resize(p, g2);//pic_resize�֐����s(p.imgpi�ɐF���i�[)
		DeleteSoftImage(g2);//g2���

		if (f == 3 || f == 2) {//�t�@�C���֐����s���̂ݏ���(�t�@�C���֐��ł�1���G�ŕ\���̈ו��������ɂ��̂܂ܕ\��)
			back_image(1);
			for (j = 0; j < p->yrs; j++){
				for (i = 0; i < p->xrs; i++){
					DrawPixel(i + (800 - p->xrs) / 2 + x, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
				}
			}
			return;
		}//�t�@�C���֐����s���̂ݏ����I��

	}//����t�@�C���I�[�v��,�`��,���T�C�Y�����I��


	if (f == 1 && mf != 1){  //�Î~��ōX�V���K�v�Ȏ��̂ݎ��s����(����f�ő���,mf !=1(����ł͂Ȃ����̏���))
		g2 = LoadSoftImage(p->g_name); //�摜�ǂݍ���
		pic_resize(p, g2);     //pic_resize�֐����s(p.imgpi�ɐF���i�[)
		DeleteSoftImage(g2);  //�n���h��g2���
	}

	if (f == 2 && mf != 1){             //�Î~��Ńt�@�C���֐��p�̏�������(����LoadSoftImage���Ă΂Ȃ��悤�ɂ����)
		g2 = LoadSoftImage(p->g_name); //�摜�ǂݍ���
		pic_resize(p, g2);            //pic_resize�֐����s(p.imgpi�ɐF���i�[)
		DeleteSoftImage(g2);         //�n���h��g2���
		return;
	}

	if (f == 3 && mf != 1){//�Î~��Ńt�@�C���֐��p�̎又��
		back_image(1);
		for (j = 0; j < p->yrs; j++){
			for (i = 0; i < p->xrs; i++){
				DrawPixel(i + (800 - p->xrs) / 2 + x, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
			}
		}
		return;
	}
		
    //���m�N���摜����,�G�b�W���o,�G���{�X����(�Î~��,����,�ǂ���ł��g�p����)p->gfr�̒l��puzzle_main���瑀�삵�Ă���
	//mf==1����ON��gfr����v�Ŏ��s, f==1�Î~��X�V�L���gfr��v�Ŏ��s
	if ((p->gfr == 1 || p->gfr == 3 || p->gfr == 4) && (mf == 1 || f == 1)) {
		pi16 = (int**)malloc(sizeof(int*) * p->xrs);//�G�b�W,�G���{�X�����ꎞ�ۊǗp�������m��
		for (i = 0; i < p->xrs; i++){ pi16[i] = (int*)malloc(sizeof(int) * p->yrs); }

		for (j = 0; j < p->yrs; j++){
			for (i = 0; i < p->xrs; i++){
				if (p->gfr == 1){//���m�N���摜����
					r = p->imgpi[i][j] >> 16;
					g = (p->imgpi[i][j] - (r << 16)) >> 8;
					b = p->imgpi[i][j] - (r << 16) - (g << 8);
					mono = (r + g + b) / 3;//(�ԁ{�΁{��)��3
					p->imcpy[i][j] = p->imgpi[i][j] = (mono << 16) + (mono << 8) + mono;//�F���𕽋ω�����ƃ��m�N���ɂȂ�
				}//���m�N�������I���

				if (p->gfr == 3 || p->gfr == 4){//�G�b�W���o,�G���{�X�O����
					r = p->imgpi[i][j] >> 16;
					g = (p->imgpi[i][j] - (r << 16)) >> 8;
					b = p->imgpi[i][j] - (r << 16) - (g << 8);
					pi16[i][j] = (r + g + b) / 3;
				}//�G�b�W���o,�G���{�X�O�����I���
			}   //for�I���
		}      //for�I���
		if ((p->gfr == 3 || p->gfr == 4) && (mf == 1 || f == 1)) {//�G�b�W���o,�G���{�X�㏈��

			for (j = 0; j < p->yrs; j++){
				for (i = 0; i < p->xrs; i++){
					if (i == 0 || i == p->xrs - 1 || j == 0 || j == p->yrs - 1){//�z��͈͊O�܂œǂ�ł��܂��̂ŊO���͌v�Z�Ɋ܂܂Ȃ��悤�ɂ���
						p->imgpi[i][j] = (1 << 16) + (1 << 8) + 1; continue;
					}
					//��������ghs 
					ghs = pi16[i - 1][j - 1] * -1;     //����,�~-1���W��,�v�Z���ĂȂ��s�N�Z���͌W��0
					ghs += pi16[i - 1][j] * -2;       //��
					ghs += pi16[i - 1][j + 1] * -1;  //����
					ghs += pi16[i + 1][j - 1] * 1;  //�E��
					ghs += pi16[i + 1][j] * 2;     //�E
					ghs += pi16[i + 1][j + 1] * 1;//�E��
					//��������gvs
					gvs = pi16[i - 1][j - 1] * -1;     //����
					gvs += pi16[i][j - 1] * -2;       //��
					gvs += pi16[i + 1][j - 1] * -1;  //�E��
					gvs += pi16[i - 1][j + 1] * 1;  //����
					gvs += pi16[i][j + 1] * 2;     //��
					gvs += pi16[i + 1][j + 1] * 1;//�E��

					if (p->gfr == 3){                 //�G�b�W���o�v�Z
						pi = sqrt(ghs*ghs + gvs*gvs);//sqrt������(pow�֐��ł����肾���������܂������Ȃ�)
					}
					if (p->gfr == 4){         //�G���{�X�v�Z
						pi = (ghs + gvs) / 2;//�Ƃ肠�������̌v�Z���@�ɂ���ƃG���{�X�ɂȂ�݂���
						pi += 128;          //�D�F���ۂ�������������炵��������
					}
					if (pi < 1)pi = 1;
					if (pi > 255)pi = 255;
					p->imcpy[i][j] = p->imgpi[i][j] = (pi << 16) + (pi << 8) + pi;
				}//for�I���
			}   //for�I���
		}      //if �G�b�W,�G���{�X�㏈���I���
		for (i = 0; i < p->yrs; i++)free(pi16[i]);//�G�b�W,�G���{�X�����ꎞ�ۊǗp���������
		free(pi16);
	}//���m�N��,�G�b�W���o,�G���{�X�����I���

    //���U�C�N����,���ʉ敗����(����,�Î~��ǂ���ł��g��)
	//gfr��v��mf==1����ON�Ŏ��s  gfr��v��f==1�Î~��X�V�L��Ŏ��s
	if ((p->gfr == 2 || p->gfr == 5) && (mf == 1 || f == 1)) {
		for (j = 0; j < p->yrs - 10; j += 10){
			for (i = 0; i < p->xrs - 10; i += 10){
				//���U�C�N����
				if (p->gfr == 2){
					for (j1 = j; j1 < j + 10; j1++){
						for (i1 = i; i1 < i + 10; i1++){
							p->imcpy[i1][j1] = p->imgpi[i1][j1] = p->imgpi[i][j];//10pixel�A���œ��F�`�ʂŃ��U�C�N�ɂȂ�
						}
					}
				}
				//���ʉ敗����
				if (p->gfr == 5){
					for (i1 = 0; i1 < 20; i1++){//20�񃉃��_���œ���ւ�
						rd = rand() % 10;
						rd1 = rand() % 10;
						rd2 = rand() % 10;
						rd3 = rand() % 10;
						temp = p->imgpi[i + rd][j + rd1];
						p->imcpy[i + rd][j + rd1] = p->imgpi[i + rd][j + rd1] = p->imgpi[i + rd2][j + rd3];
						p->imcpy[i + rd2][j + rd3] = p->imgpi[i + rd2][j + rd3] = temp;
					}
				}
			}
		}
	}

	//�q�X�g�O�����f�[�^����(�X�V�K�v��,���s)
	if (mf == 1 || f == 1){
		for (i = 0; i < 128; i++){  //������
			hist[i].rp = 0;
			hist[i].gp = 0;
			hist[i].bp = 0;
		}
		for (j = 0; j < p->yrs; j++){//�e�F,�e�K�����Ɍ�����
			for (i = 0; i < p->xrs; i++){
				r = p->imgpi[i][j] >> 16;
				g = (p->imgpi[i][j] - (r << 16)) >> 8;
				b = p->imgpi[i][j] - (r << 16) - (g << 8);
				hist[r / 2].rp++;
				hist[g / 2].gp++;
				hist[b / 2].bp++;
			}
		}
	}//�q�X�g�O�����f�[�^�����I��

			
    //��������s���鏈��(�t�@�C���֐��ȊO)
	ClearDrawScreen();  //��ʏ���
	back_image(1);     //�w�i�`��֐�,����1==�摜�`��
	//�p�Y�����ȊO�̉摜�`�捶��
	for (i = 0; i < (p->xrs - 400) / 2; i++){
		for (j = 0; j < 400; j++){
			DrawPixel(i + (800 - p->xrs) / 2 + 1, j + 100, p->imgpi[i][j]);//+1�͕␳
		}
	}  
	//�p�Y�����ȊO�̉摜�`��E��
	for (i = p->xrs - (p->xrs - 400) / 2; i < p->xrs; i++){
		for (j = 0; j < 400; j++){
			DrawPixel(i + (800 - p->xrs) / 2, j + 100, p->imgpi[i][j]);
		}
	}
    //�p�Y�����ȊO�̉摜�`��㑤
	for (j = 0; j < (p->yrs - 400) / 2; j++){
		for (i = 0; i < 400; i++){
			DrawPixel(i + 200, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
		}
	}
	//�p�Y�����ȊO�̉摜�`�扺��
	for (j = p->yrs - (p->yrs - 400) / 2; j < p->yrs; j++){
		for (i = 0; i < 400; i++){
			DrawPixel(i + 200, j + (600 - p->yrs) / 2, p->imgpi[i][j]);
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
	if (ff == 1 && p->size < 5){
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (p->block[i][j] == 0)continue;   //�F�ϊ������X�L�b�v
				//�����̂܂ܕϊ�����ƐF���ς���Ă����Ă��܂��̂ň�U���ɖ߂�
				p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] = p->imcpy[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2];
				//���F�擾�ϊ�
				r = (p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] >> 16);
                g = ((p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] - (r << 16)) >> 8);
			    b = (p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] - (r << 16) - (g << 8));
				r -= p->block[i][j];
				g -= p->block[i][j];
				b -= p->block[i][j];
				if (r <= 0)r = 1;
				if (g <= 0)g = 1;
				if (b <= 0)b = 1;
				p->imgpi[i + (p->xrs - 400) / 2][j + (p->yrs - 400) / 2] = GetColor(r, g, b);//�F���
			}
		}
	}//�u���b�N�摜�����I���

	//�����u���b�N����
	if (ff == 0 && mf == 0){    //�����ςݐÎ~�揈��
		for (j = 0; j < p->yrs; j++){
			for (i = 0; i < p->xrs; i++){
				p->imgpi[i][j] = p->imcpy[i][j];//�u���b�N�摜�����O�f�[�^���R�s�[
			}
		}
	}

	//����,���������㏈��(�ċA��1�x�������������)
	if (ffr == 1){
		if (ff == 0){//�������㏈��
			for (i = 0; i < 20; i++){
				drawing_img(p, 20 - i, -20 + i, 0);//�ċA
				ScreenFlip();
			}
		}
		if (ff == 1){//���������㏈��
			for (i = 0; i < 20; i++){
				drawing_img(p, i - 20, -i + 20, 0);//�ċA(20��xx,yy�̕��̕␳)
				ScreenFlip();
			}
		}
	}//����,���������㏈���I���

	if (ff == 0){ xx = 0; yy = 0; }//������ԂŃI�t�Z�b�g�ʍX�V
	else{ xx = 20; yy = -20; }
	
	for (i = 0; i < prs->idx; i++){ //�p�Y�����摜�`�揈��
		for (j = 0; j < prs->bsize; j++){     //1�u���b�N��y����
			for (k = 0; k < prs->bsize; k++){//1�u���b�N��x����
							
				//�\�����W�ʒu�{�I�t�Z�b�g, �\���F���(�������W�ʒu�|�I�t�Z�b�g)
				DrawPixel(k + img[i].cx + xx + x, j + img[i].cy + yy + y,     
					p->imgpi[k + img[i].fx - 200 + (p->xrs - 400) / 2]
					[j + img[i].fy - 100 + (p->yrs - 400) / 2]);
			}
		}
	}//�p�Y�����摜�`�揈���I���

	for (i = 0; i < 128; i++){//�q�X�g�O�����`��
		DrawLine(640 + i, 200, 640 + i, 200 - hist[i].rp / 80, GetColor(i * 2, 0, 0));
		DrawLine(640 + i, 300, 640 + i, 300 - hist[i].gp / 80, GetColor(0, i * 2, 0));
		DrawLine(640 + i, 400, 640 + i, 400 - hist[i].bp / 80, GetColor(0, 0, i * 2));
	}//�q�X�g�O�����`��I��

	InitSoftImage();//�\�t�g�C���[�W�S�J��
}                 //drawing_img()�I��