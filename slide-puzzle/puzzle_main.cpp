//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�X���C�h�p�Y��main                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct_set.h"
#include "drawing_img.h"
#include "file.h"
#include "mov.h"
#include "auto_set.h"
#include "cha_shu.h"
#include "back_image.h"
#include "mouse.h"
#include "sound.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){    //���C���֐�

	//�E�B���h�E���[�h�ύX, DxLib������, �E�B���h�E�T�C�Y�ύX, ����ʐݒ�
	ChangeWindowMode(TRUE), DxLib_Init(), SetGraphMode(800, 600, 32); SetDrawScreen(DX_SCREEN_BACK); 
	//��A�N�e�B�u�ł��������s
	SetAlwaysRunFlag(TRUE);

	alldata p;             //�قڑS�֐��Ŏg���f�[�^�Q
	int i, j, k;          //for��
	int frg = 0;         //�L�[����
	int Color;          //�F
	//time_t sec1,sec2;//�҂����ԗp
	int sm[2];        //�萔�v�Z�����O�X�y�[�X���W�ޔ�p
    
	p.cnt_results = 0;  //�萔�v�Z���ʊm�F�p
	p.gfr = 0;         //�摜���[�h������(���m�N����)
	p.size = 0;       //�G���[�h�~ 
	p.tkf = 0;       //�萔�v�Z���s�t���O������
	p.tkc = 0;      //�萔�v�Z�J�E���g������
	p.mc = 0;      //pic_resize���������m�ۃ`�F�b�N������ 0:���m��, 1:�m�ۍς�
	p.lock = 0;   //�f�[�^���쎞���b�N������(�{�X��)
	p.lock_t = 0;//�f�[�^���쎞���b�N������(���o�X���b�h)

	p.paras[0] = { 16, 4, 100, 500, 400, 400, 15, 31, 5, 10 };    //�u���b�N�����̃p�����[�^�[
	p.paras[1] = { 64, 8, 50, 550, 450, 800, 63, 127, 5, 10 };
	p.paras[2] = { 256, 16, 25, 575, 475, 1000, 255, 511, 5, 5 };
	p.paras[3] = { 1600, 40, 10, 590, 490, 2000, 1599, 3199, 2, 5 };
	p.paras[4] = { 10000, 100, 4, 596, 496, 9000, 9999, 19999, 2, 4 };
	p.paras[5] = { 160000, 400, 1, 599, 499, 300000, 159999, 319999, 1, 1 };
	
	srand((unsigned)time(NULL));
	InitSoftImage();                 //�\�t�g�C���[�W�S�J��
	Color = GetColor(255, 255, 255);//���̐F
    
	back_image(0);    //�w�i�`��֐�,����0==�摜���n���h���Ɋi�[
	mouse(&p,0,0);   //�}�E�X�֐������ݒ� 
	sound(0);       //�T�E���h�֐���������
	file(&p);      //�摜�t�@�C���I���֐��Ăяo����,p.g_name�ɉ摜�t�@�C�����i�[
	
				
	while (1){ //1�ԊO����while��

		back_image(1);         //�w�i�`��֐�,����1==�摜�`��

		while (frg == 0){    //������
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(p.img); return -1; }//�����I��
			drawing_img(&p, 0, 0, 2); //�`��O����
			drawing_img(&p, 0, 0, 3);//�`��
			frg = mouse(&p, 2, 0);  //�}�E�X�֐�
            ScreenFlip();          //�\��ʂɕ`��
        }                         //while�I���
		p.size = frg - 1;        //size����
		sound(1);               //�T�E���h�֐��{�^����

		//�������m��
		p.img = (imxy*)malloc(sizeof(imxy)*p.paras[p.size].idx);//�摜���W
		p.block = (int**)malloc(sizeof(int*) * 400);//�u���b�N�摜(drawing_img�ŏ������ƕ`��x���Ȃ�̂�)
		for (i = 0; i < 400; i++)p.block[i] = (int*)malloc(sizeof(int) * 400);

		//�u���b�N�摜�t�B���^�[�쐬
		for (j = 0; j < 400; j++){   
			for (i = 0; i < 400; i++){
				//�O��
				if (j%p.paras[p.size].bsize == 0 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 1 ||
					i%p.paras[p.size].bsize == 0 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 1){
					p.block[i][j] = 150; continue;
				}
				//��
				if (p.size < 4 &&   //size3�ȉ�����
					(j%p.paras[p.size].bsize == 1 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 2 ||
					i%p.paras[p.size].bsize == 1 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 2) &&
					j%p.paras[p.size].bsize != 0 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 1 &&
					i%p.paras[p.size].bsize != 0 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 1){
					p.block[i][j] = 90; continue;
				}
				//��
				if (p.size < 4 &&   //size3�ȉ�����
					(j%p.paras[p.size].bsize == 2 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 3 ||
					i%p.paras[p.size].bsize == 2 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 3) &&
					j%p.paras[p.size].bsize != 1 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					i%p.paras[p.size].bsize != 1 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					j%p.paras[p.size].bsize != 0 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 1 &&
					i%p.paras[p.size].bsize != 0 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 1){
					p.block[i][j] = 50; continue;
				}
				//����
				if (p.size < 4 &&   //size3�ȉ�����
					(j%p.paras[p.size].bsize == 3 || j%p.paras[p.size].bsize == p.paras[p.size].bsize - 4 ||
					i%p.paras[p.size].bsize == 3 || i%p.paras[p.size].bsize == p.paras[p.size].bsize - 4)&&
					j%p.paras[p.size].bsize != 2 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 3 &&
					i%p.paras[p.size].bsize != 2 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 3 &&
					j%p.paras[p.size].bsize != 1 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					i%p.paras[p.size].bsize != 1 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 2 &&
					j%p.paras[p.size].bsize != 0 && j%p.paras[p.size].bsize != p.paras[p.size].bsize - 1 &&
					i%p.paras[p.size].bsize != 0 && i%p.paras[p.size].bsize != p.paras[p.size].bsize - 1){
					p.block[i][j] = 20; continue;
				}
				//�Y������
				p.block[i][j] = 0;
			}
		}//�u���b�N�摜�t�B���^�[�쐬�I��
		
		k = 0;
		for (j = 100; j <= p.paras[p.size].lastposy; j += p.paras[p.size].bsize){    //���W�f�[�^������
			for (i = 200; i <= p.paras[p.size].lastposx; i += p.paras[p.size].bsize){
				p.img[k].cx = p.img[k].fx = p.img[k].chx = i;
				p.img[k].cy = p.img[k].fy = p.img[k++].chy = j;
			}
		}

		drawing_img(&p, 0, 0, 1);//�����摜�`��
		ScreenFlip();           //�\��ʕ`��

		//sec2 = 0; for (time(&sec1); sec2 - sec1 <= 0.7; time(&sec2));//�҂�����

		shuffle(&p);               //�V���b�t���֐�
		change(&p);               //�u���b�N�����֐�
		frg = 0;
		while (1){

				if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(p.img); return -1; }

				frg = mouse(&p,3,0);  //�}�E�X�֐�

				ScreenFlip();       //�\��ʕ`��
				if (frg >= 5) {    //�ړ��t���O�̎��͔�΂�
					if (frg == 5){//�S�u���b�N�ʒu���ʂ菈����
						sound(1);//�T�E���h�֐��{�^����  
						for (i = 0; i < p.paras[p.size].idx; i++){ p.img[i].chx = p.img[i].fx; p.img[i].chy = p.img[i].fy; } //�����摜�f�[�^����R�s�[
						change(&p);//�u���b�N�����֐�
					}             //if�I���

					if (frg == 6){     //�X�ɕ��ёւ�
						sound(1);     //�T�E���h�֐��{�^����
						shuffle(&p); //�V���b�t���֐�
						change(&p); //�u���b�N�����֐�
					}              //if�I���

					if (frg == 7){ //automatic�֐����s
						sound(1); //�T�E���h�֐��{�^����
						auto_matic(&p, 0, p.paras[p.size].pcs, p.paras[p.size].pcs, 0);
						drawing_img(&p, 0, 0, 0);
					}

					if (frg == 8){//���ύX
						InitSoftImage(); free(p.img);
						for (i = 0; i < 400; i++)free(p.block[i]);
						free(p.block);
						p.th_st = 0;           //�X���b�h�i�s�󋵏�����
						p.gfr = 0;            //�摜���[�h���Z�b�g
						frg = 0;break;
					} //�ǂݍ��ݍς݃O���t�B�b�N�f�[�^�폜,���������,break

					if (frg == 9){             //�摜�ύX
						p.th_f = 0;           //���o�X���b�h�t���O������
						p.gfr = 0;           //�摜���[�h���Z�b�g
						frg = 0; file(&p);      //�t�@�C���֐��Ăяo��
						p.th_st = 0;           //���o�X���b�h�i�s�󋵏�����
						back_image(1); drawing_img(&p, 0, 0, 1);//�w�i,�摜�`��
					} 

					if (frg == 10) {//�ʏ�摜�摜����
						p.gfr = 0; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 11) {//���m�N���摜����
						p.gfr = 1; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 12) {//���U�C�N�摜����
						p.gfr = 2; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 13){//�萔�v�Z
						p.tkf = 1; p.tkc = 0;//�t���O,�J�E���g������
						for (i = 0; i < p.paras[p.size].idx; i++){
							p.img[i].chx = p.img[i].cx;//�萔�v�Z�O���W�l�ޔ�
							p.img[i].chy = p.img[i].cy;//�萔�v�Z�O���W�l�ޔ�
						}
						sm[0] = p.space[0];//�萔�v�Z�O���W�l�ޔ�
						sm[1] = p.space[1];//�萔�v�Z�O���W�l�ޔ�
						DrawFormatString(305, 502, Color, "�摜���N���b�N�Œ��~");
						ScreenFlip();
						auto_matic(&p, 0, p.paras[p.size].pcs, p.paras[p.size].pcs, 0);//auto_matic�֐�����

						for (i = 0; i < p.paras[p.size].idx; i++){
							p.img[i].cx = p.img[i].chx;//�萔�v�Z����W�l�߂�
							p.img[i].cy = p.img[i].chy;//�萔�v�Z����W�l�߂�
						}
						p.space[0] = sm[0];//�萔�v�Z����W�l�߂�
						p.space[1] = sm[1];//�萔�v�Z����W�l�߂�
						if (p.cnt_results != -1){//-1���ƒ��~���Ă鎖�ɂȂ�
							DrawBox(300, 500, 500, 540, 1, TRUE);
							DrawFormatString(305, 502, Color, "%d��Ŋ������܂�", p.tkc);
							DrawFormatString(305, 522, Color, "�摜���N���b�N�ŏI��");
							ScreenFlip();
							while (mouse(&p, 5, 0) != 1);
						}//if�I���
						drawing_img(&p, 0, 0, 1);
						p.tkf = 0; p.tkc = 0;//�t���O,�J�E���g������
					}//�萔�v�Z�I��

					if (frg == 14) {//�G�b�W���o����
						p.gfr = 3; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 15) {//�G���{�X����
						p.gfr = 4; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 16) {//�G�敗����
						p.gfr = 5; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 17) {//��ʌ��o����
						p.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
						p.th_st = 0;//�X���b�h�i�s�󋵏�����
						p.gfr = 6; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 18) {//��ʃ��U�C�N����
						p.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
						p.th_st = 0;//�X���b�h�i�s�󋵏�����
						p.gfr = 7; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

					if (frg == 19) {//�l�K�|�W����
						p.gfr = 8; frg = 0; back_image(1); drawing_img(&p, 0, 0, 1);
					}

				}//�ړ��t���O�̎��͔�΂�
			
				mov(&p, 0, frg, 0); //�ړ�����
			
				drawing_img(&p, 0, 0, 0);
	
			frg = 0; //�ړ��t���O������

		}  //while�I���

	}//��ԊO��while�I���
	InitSoftImage();        //�\�t�g�C���[�W�S�J��
	DxLib_End();           // DX���C�u�����I������
	free(p.img); return 0;//���������,�I��
} // winmain�I���

//********************************************************************************************************//
//**** libjpeg�@Copyright (C) 1991-2013, Thomas G. Lane, Guido Vollbeding.  ******************************//
//**** DX Library Copyright (C) 2001-2014 Takumi Yamada.   ***********************************************//
//********************************************************************************************************//