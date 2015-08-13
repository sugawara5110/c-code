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

int ImageDraw::processing_img_flg(Dx9Init *dx, Move *move, int flg){

	if (flg != 0 && d.gfl == 3)ver = 0;
	if (flg >= 8 && flg <= 12 || flg >= 14 && flg <= 19 || flg == 21){
		obj_delete();//�I�u�W�F�N�g�j��
	}

	if (flg == 8){//���ύX
		if (d.gfl == 9)d.gfl = 0;
		return 1;//���C�����[�v����
	}

	if (flg == 10)d.gfl = 0;//�ʏ�摜�摜����
	if (flg == 11)d.gfl = 1;//���m�N���摜����
	if (flg == 12)d.gfl = 2;//���U�C�N�摜����
	if (flg == 14)d.gfl = 3;//�G�b�W���o����
	if (flg == 15)d.gfl = 4;//�G���{�X����
	if (flg == 16)d.gfl = 5;//�G�敗����
	if (flg == 25)d.gfl = 11;//�X���b�g�X�L��������
	if (flg == 26)d.gfl = 12;//�m�C�Y��������
	if (flg == 17) {//��ʌ��o����
		d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
		d.th_st = 0;//�X���b�h�i�s�󋵏�����
		d.gfl = 6;
	}

	if (flg == 18) {//��ʃ��U�C�N����
		d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
		d.th_st = 0;//�X���b�h�i�s�󋵏�����
		d.gfl = 7; 
	}

	if (flg == 19) {//�l�K�|�W����
		d.gfl = 8; 
	}

	if (flg == 21) {//�炷���ւ�����
		d.th_f = 0; //���o�X���b�h�X�^�[�g�t���O0:�X�g�b�v 1:�X�^�[�g
		d.th_st = 0;//�X���b�h�i�s�󋵏�����
		d.gfl = 10; 
	}

	if (flg == 20 || d.gfl == 9) {//�摜�G���{�X����
		static Menu menu(0);
		static File file;
		int fr = 0;
		fr = menu.mouse(dx, this, 6, 0);
		if (fr != 0 || d.gfl != 9){//���j���[����L����gfl==9��ŏ�
			obj_delete();   //�摜�G���{�X�p�I�u�W�F�N�g�j��
			obj_create(dx, file.e_file(this, fr));//�摜�G���{�X�p�I�u�W�F�N�g����
		}
		d.gfl = 9;
	}

	if (flg == 22){//3D�ϊ�
		d3 = d3 % 3 + 1;
		releaseVB();//���_�o�b�t�@,�z����
		ver = 0;
	}

	if (flg == 23){//�L���v�`��
		capture(move);
	}

	if (flg == 24)putrate(1.0f);//�Đ����x�W���߂�

	return 0;
}