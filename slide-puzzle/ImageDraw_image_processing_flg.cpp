//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageDraw�N���X�����\�b�h��`(�摜�����t���O)               **//
//**                                    (ImageRead�̔h���N���X)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h" 
#include "Menu.h"
#include "File.h"

int ImageDraw::image_processing_flg(Dx9Init *dx, Filter *filter, Move *move, int flg){

	if (flg >= 8 && flg <= 12 || flg >= 14 && flg <= 19 || flg == 21){
		obj_delete();//�I�u�W�F�N�g�j��
	}

	if (flg == 8){//���ύX
		if (d.gfr == 9)d.gfr = 0;
		return 1;//���C�����[�v����
	}

	if (flg == 10) {//�ʏ�摜�摜����
		d.gfr = 0; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 11) {//���m�N���摜����
		d.gfr = 1; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 12) {//���U�C�N�摜����
		d.gfr = 2; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 14) {//�G�b�W���o����
		d.gfr = 3; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 15) {//�G���{�X����
		d.gfr = 4; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 16) {//�G�敗����
		d.gfr = 5; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 17) {//��ʌ��o����
		d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
		d.th_st = 0;//�X���b�h�i�s�󋵏�����
		d.gfr = 6; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 18) {//��ʃ��U�C�N����
		d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
		d.th_st = 0;//�X���b�h�i�s�󋵏�����
		d.gfr = 7; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 19) {//�l�K�|�W����
		d.gfr = 8; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 21) {//�炷���ւ�����
		d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
		d.th_st = 0;//�X���b�h�i�s�󋵏�����
		d.gfr = 10; drawing_img(dx, filter, move, 0, 0, 0, 1);
	}

	if (flg == 20 || d.gfr == 9) {//�摜�G���{�X����
		static Menu menu(0);
		static File file;
		int fr = 0;
		fr = menu.mouse(dx, this, 6, 0);
		if (fr != 0 || d.gfr != 9){//���j���[����L����gfr==9��ŏ�
			obj_delete();   //�摜�G���{�X�p�I�u�W�F�N�g�j��
			obj_create(dx, file.e_file(this, fr));//�摜�G���{�X�p�I�u�W�F�N�g����
			read->drawing_img(dx, NULL, NULL, 0, 0, 0, 1);//�Î~��p�X�V
		}
		d.gfr = 9;
	}

	if (flg == 22){//3D�ϊ�
		d3 = d3 % 3 + 1;
		pMyVB->Release(); pMyVB = NULL;//���_�o�b�t�@���(��{�I�Ɋm�یサ�����s����Ȃ��̂�NULL�m�F�K�v����)
		if (d3 == 1)dx->verbufcr(&pMyVB, xrs, yrs);//���_�o�b�t�@�[�m��(�ʏ�)
		else  dx->verbufcr(&pMyVB, xrs, yrs * 2); //���_�o�b�t�@�[�m��(Z��������*2)
	}

	return 0;
}

int ImageDraw::theta(int f, int t){//�J�����p�x����
	if (f == 1)theta_lr += t;
	if (f == 2)theta_lr -= t;
	if (theta_lr < 0)theta_lr = 360;
	if (theta_lr > 360)theta_lr = 0;
	return theta_lr;
}