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
#include "sound.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Menu.h"
#include "File.h"
#include "Filter.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){    //���C���֐�

	//�E�B���h�E���[�h�ύX, DxLib������, �E�B���h�E�T�C�Y�ύX, ����ʐݒ�
	ChangeWindowMode(TRUE), DxLib_Init(), SetGraphMode(800, 600, 32); SetDrawScreen(DX_SCREEN_BACK);
	//��A�N�e�B�u�ł��������s
	SetAlwaysRunFlag(TRUE);

	int i, j, k;          //for��
	int frg, frgr;       //�L�[����,���͗���
	int fr;
	int Color;         //�F
	int sm[2];        //�萔�v�Z�����O�X�y�[�X���W�ޔ�p

	Move move;          //�ړ��N���X�I�u�W�F�N�g����
	File file;         //�t�@�C���N���X�I�u�W�F�N�g����
	Filter filter;    //�t�B���^�[�N���X�I�u�W�F�N�g����
	Menu menu;       //���j���[�N���X�I�u�W�F�N�g����
	ImageDraw *draw;//�摜�`��N���X�I�u�W�F�N�g��`

	srand((unsigned)time(NULL));
	InitSoftImage();                 //�\�t�g�C���[�W�S�J��
	Color = GetColor(255, 255, 255);//���̐F

	menu.mouse(NULL, 0, 0); //�}�E�X�֐������ݒ� 
	sound(0);              //�T�E���h�֐���������

	draw = new ImageDraw(file.file());//�t�@�C���֐��Ԃ�l��draw�I�u�W�F�N�g����

	while (1){ //1�ԊO����while��

		frg = 0;       //�ړ��t���O������

		while (frg == 0){    //������
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(move.img); return -1; }//�����I��
			draw->drawing_img(&filter, NULL, 0, 0, 1); //�`��O����
			draw->drawing_img(&filter, NULL, 0, 0, 0);//�`��
			frg = menu.mouse(draw, 2, 0);  //�}�E�X�֐�
			ScreenFlip();          //�\��ʂɕ`��
		}                         //while�I���
		move.size = frg - 1;     //size����
		sound(1);               //�T�E���h�֐��{�^����

		//�������m��
		move.img = (Move::imxy*)malloc(sizeof(Move::imxy)*move.paras[move.size].idx);//�摜���W

		//�u���b�N�摜�t�B���^�[�쐬
		filter.filter(&move, draw);
		//�u���b�N�摜�t�B���^�[�쐬�I��

		k = 0;
		for (j = 100; j <= move.paras[move.size].lastposy; j += move.paras[move.size].bsize){    //���W�f�[�^������
			for (i = 200; i <= move.paras[move.size].lastposx; i += move.paras[move.size].bsize){
				move.img[k].cx = move.img[k].fx = move.img[k].chx = i;
				move.img[k].cy = move.img[k].fy = move.img[k++].chy = j;
			}
		}

		move.shuffle(); //�V���b�t���֐�
		move.change(&filter, draw);//�u���b�N�����֐�

		while (1){//�p�Y�����s�����[�v

			frg = 0; //�ړ��t���O������

			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(move.img); return -1; }

			frg = menu.mouse(draw, 3, 0); //�}�E�X�֐�
		 
			if (frg == 20 || draw->d.gfr == 9) {//�摜�G���{�X����
				fr = 0;
				fr = menu.mouse(draw, 6, 0);
				if (fr != 0 || draw->d.gfr != 9){//���j���[����L����gfr==9��ŏ�
					draw->obj_delete();   //�摜�G���{�X�p�I�u�W�F�N�g�j��
					draw->obj_create(file.e_file(draw, fr));//�摜�G���{�X�p�I�u�W�F�N�g����
					draw->read->drawing_img(NULL, NULL, 0, 0, 1);//�Î~��p�X�V
				}
				draw->d.gfr = 9;
			}

			if (draw->d.d3f == 1 || draw->d.d3f == 2){//3D�摜
				fr = 0;
				fr = menu.mouse(draw, 7, 0);
				if (fr == 1)
				{
					draw->d.theta_lr -= 1;
					if (draw->d.theta_lr < 0)draw->d.theta_lr = 360;
				}
				if (fr == 2)
				{
					draw->d.theta_lr += 1;
					if (draw->d.theta_lr > 360)draw->d.theta_lr = 0;
				}
			}

			ScreenFlip();       //�\��ʕ`��
			if (frg >= 5) {    //�ړ��t���O�̎��͔�΂�
				if (frg == 5){//�S�u���b�N�ʒu���ʂ菈����
					sound(1);//�T�E���h�֐��{�^����  
					for (i = 0; i < move.paras[move.size].idx; i++){ move.img[i].chx = move.img[i].fx; move.img[i].chy = move.img[i].fy; } //�����摜�f�[�^����R�s�[
					move.change(&filter, draw);//�u���b�N�����֐�
				}             //if�I���

				if (frg == 6){        //���ёւ�
					sound(1);        //�T�E���h�֐��{�^����
					move.shuffle(); //�V���b�t���֐�
					move.change(&filter, draw);//�u���b�N�����֐�
				}                     //if�I���

				if (frg == 7){ //automatic�֐����s
					sound(1); //�T�E���h�֐��{�^����
					move.auto_matic(&filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0);
					draw->drawing_img(&filter, &move, 0, 0, 0);
				}

				if (frg >= 8 && frg <= 12 || frg >= 14 && frg <= 19 || frg == 21){ //�摜�����O����
					draw->obj_delete();//�I�u�W�F�N�g�j��
				}

				if (frg == 8){//���ύX
					InitSoftImage(); free(move.img);
					draw->d.th_st = 0;           //�X���b�h�i�s�󋵏�����
					draw->d.gfr = 0;            //�摜���[�h���Z�b�g
					break;
				} //�ǂݍ��ݍς݃O���t�B�b�N�f�[�^�폜,���������,break

				if (frg == 9){             //�摜�ύX
					draw->d.th_f = 0;     //���o�X���b�h�t���O������
					draw->d.gfr = 0;     //�摜���[�h���Z�b�g
					delete draw;        //�I�u�W�F�N�g�j��  
					draw = new ImageDraw(file.file()); //�t�@�C���֐��Ăяo��,�摜�`��I�u�W�F�N�g����
					draw->d.th_st = 0; //���o�X���b�h�i�s�󋵏�����
					draw->drawing_img(&filter, &move, 0, 0, 1);//�w�i,�摜�`��
				}

				if (frg == 10) {//�ʏ�摜�摜����
					draw->d.gfr = 0; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 11) {//���m�N���摜����
					draw->d.gfr = 1; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 12) {//���U�C�N�摜����
					draw->d.gfr = 2; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 13){//�萔�v�Z
					move.tkf = 1; move.tkc = 0;//�t���O,�J�E���g������
					for (i = 0; i < move.paras[move.size].idx; i++){
						move.img[i].chx = move.img[i].cx;//�萔�v�Z�O���W�l�ޔ�
						move.img[i].chy = move.img[i].cy;//�萔�v�Z�O���W�l�ޔ�
					}
					sm[0] = move.space[0];//�萔�v�Z�O���W�l�ޔ�
					sm[1] = move.space[1];//�萔�v�Z�O���W�l�ޔ�
					DrawFormatString(305, 502, Color, "�摜���N���b�N�Œ��~");
					ScreenFlip();
					move.auto_matic(&filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0);//auto_matic�֐�����

					for (i = 0; i < move.paras[move.size].idx; i++){
						move.img[i].cx = move.img[i].chx;//�萔�v�Z����W�l�߂�
						move.img[i].cy = move.img[i].chy;//�萔�v�Z����W�l�߂�
					}
					move.space[0] = sm[0];//�萔�v�Z����W�l�߂�
					move.space[1] = sm[1];//�萔�v�Z����W�l�߂�
					if (move.cnt_results != -1){//-1���ƒ��~���Ă鎖�ɂȂ�
						DrawBox(300, 500, 500, 540, 1, TRUE);
						DrawFormatString(305, 502, Color, "%d��Ŋ������܂�", move.tkc);
						DrawFormatString(305, 522, Color, "�摜���N���b�N�ŏI��");
						ScreenFlip();
						while (menu.mouse(draw, 5, 0) != 1);
					}//if�I���
					draw->drawing_img(&filter, &move, 0, 0, 1);
					move.tkf = 0; move.tkc = 0;//�t���O,�J�E���g������
				}//�萔�v�Z�I��

				if (frg == 14) {//�G�b�W���o����
					draw->d.gfr = 3; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 15) {//�G���{�X����
					draw->d.gfr = 4; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 16) {//�G�敗����
					draw->d.gfr = 5; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 17) {//��ʌ��o����
					draw->d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
					draw->d.th_st = 0;//�X���b�h�i�s�󋵏�����
					draw->d.gfr = 6; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 18) {//��ʃ��U�C�N����
					draw->d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
					draw->d.th_st = 0;//�X���b�h�i�s�󋵏�����
					draw->d.gfr = 7; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 19) {//�l�K�|�W����
					draw->d.gfr = 8; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frg == 21) {//�炷���ւ�����
					draw->d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
					draw->d.th_st = 0;//�X���b�h�i�s�󋵏�����
					draw->d.gfr = 10; draw->drawing_img(&filter, &move, 0, 0, 1);
				}

				if (frgr != 22 && frg == 22){//3D�摜
					if (draw->d.d3f == 0)draw->d.d3f = 1;
					else if (draw->d.d3f == 1)draw->d.d3f = 2;
					else if (draw->d.d3f == 2)draw->d.d3f = 0;
					draw->drawing_img(&filter, &move, 0, 0, 1);//�w�i,�摜�`��
				}

			}//�ړ��t���O�̎��͔�΂�

			move.mov(&filter, draw, 0, frg, 0); //�ړ�����

			draw->drawing_img(&filter, &move, 0, 0, 0);//�ړ����Ȃ��ꍇ�ׂ̈̏���

			frgr = frg;//�L�[���͗���
		}//�p�Y�����s�����[�v

	}//��ԊO��while�I���
	InitSoftImage();        //�\�t�g�C���[�W�S�J��
	DxLib_End();           // DX���C�u�����I������
	free(move.img); return 0;//���������,�I��
} // winmain�I���

//********************************************************************************************************//
//**** libjpeg�@Copyright (C) 1991-2013, Thomas G. Lane, Guido Vollbeding.  ******************************//
//**** DX Library Copyright (C) 2001-2014 Takumi Yamada.   ***********************************************//
//********************************************************************************************************//