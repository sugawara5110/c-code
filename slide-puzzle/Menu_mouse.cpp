//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@Menu�N���X�����\�b�h��`                                **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Menu.h"
#include "Sound_.h"

int Menu::xm;
int Menu::ym;
int Menu::clf;

int Menu::rectangle_button(Dx9Init *dx, int xm, int ym, float x, float y, char *str, int f, int fr){//�{�^���p�֐�
	
	re[0].x = x;
	re[0].y = y;
	re[0].z = 0.0f;
	re[0].rhw = 0.0f;
	re[0].color = (0 << 16) + (0 << 8) + 100;
	re[0].tu = 0.0f;
	re[0].tv = 0.0f;

	re[1].x = x;
	re[1].y = y + 20;
	re[1].z = 0.0f;
	re[1].rhw = 0.0f;
	re[1].color = (0 << 16) + (0 << 8) + 50;
	re[1].tu = 0.0f;
	re[1].tv = 0.0f;

	re[2].x = x + 100;
	re[2].y = y + 20;
	re[2].z = 0.0f;
	re[2].rhw = 0.0f;
	re[2].color = (0 << 16) + (0 << 8) + 50;
	re[2].tu = 0.0f;
	re[2].tv = 0.0f;

	re[3].x = x + 100;
	re[3].y = y;
	re[3].z = 0.0f;
	re[3].rhw = 0.0f;
	re[3].color = (0 << 16) + (0 << 8) + 100;
	re[3].tu = 0.0f;
	re[3].tv = 0.0f;

	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y){//�}�E�X���W==�{�^���͈͓����H

		re[0].color = (0 << 16) + (0 << 8) + 255;

		re[1].color = (0 << 16) + (0 << 8) + 200;

		re[2].color = (0 << 16) + (0 << 8) + 200;

		re[3].color = (0 << 16) + (0 << 8) + 255;
	}
	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y && strcmp(str, " 3D���[�h") == 0){ //3D��]�p�l���o���ۏ���
		if (d3f == 0 && d3b == 0){ d3f = 1; d3b = 1; }
		if (d3f == 1 && d3b == 0){ d3f = 0; d3b = 1; }
	}
	else if (strcmp(str, " 3D���[�h") == 0)d3b = 0;

	dx->d2rectangle_button(str, x, y, re);

	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y && clf == 1)//�͈͓������N���b�N
		return f;//�{�^���͈͓��N���b�N
	return fr;  //���������ĂȂ��ꍇ���̐��l��Ԃ�
}

int Menu::square_button(Dx9Init *dx, ImageDraw *draw, int xm, int ym, float x, float y, char *str, int fr) {//���p�֐�

	sq[0].x = x;
	sq[0].y = y;
	sq[0].z = 0.0f;
	sq[0].rhw = 0.0f;
	sq[0].color = (0 << 16) + (0 << 8) + 150;
	sq[0].tu = 0.0f;
	sq[0].tv = 0.0f;

	sq[1].x = x;
	sq[1].y = y + 50;
	sq[1].z = 0.0f;
	sq[1].rhw = 0.0f;
	sq[1].color = (0 << 16) + (0 << 8) + 100;
	sq[1].tu = 0.0f;
	sq[1].tv = 0.0f;

	sq[2].x = x + 50;
	sq[2].y = y + 50;
	sq[2].z = 0.0f;
	sq[2].rhw = 0.0f;
	sq[2].color = (0 << 16) + (0 << 8) + 100;
	sq[2].tu = 0.0f;
	sq[2].tv = 0.0f;

	sq[3].x = x + 50;
	sq[3].y = y;
	sq[3].z = 0.0f;
	sq[3].rhw = 0.0f;
	sq[3].color = (0 << 16) + (0 << 8) + 150;
	sq[3].tu = 0.0f;
	sq[3].tv = 0.0f;

	if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){//�}�E�X���W==�{�^���͈͓����H
		
		sq[0].color = (0 << 16) + (0 << 8) + 255;
		
		sq[1].color = (0 << 16) + (0 << 8) + 200;
		
		sq[2].color = (0 << 16) + (0 << 8) + 200;
		
		sq[3].color = (0 << 16) + (0 << 8) + 255;
	}

	dx->d2square_button(2, sq);

	if (str == "LF"){//File�֐��p�����摜�\������

		tr[0].x = x + 5;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 30;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 30;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->d2square_button(1, tr);

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//�͈͓������N���b�N
			return 1;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "RF"){//File�֐��p�����摜�\������

		tr[0].x = x + 45;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 20;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 20;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->d2square_button(1, tr);

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//�͈͓������N���b�N
			return 2;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}
	
	if (str == "L"){

		tr[0].x = x + 5;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 30;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 30;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->d2square_button(1, tr);

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = true;//�}�E�X���W==�{�^���͈͓����H
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//�͈͓������N���b�N
			return 1;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "R"){

		tr[0].x = x + 45;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 20;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 20;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->d2square_button(1, tr);

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = true;//�}�E�X���W==�{�^���͈͓����H
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//�͈͓������N���b�N
			return 2;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "U"){

		tr[0].x = x + 25;
		tr[0].y = y + 5;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 5;
		tr[1].y = y + 30;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 45;
		tr[2].y = y + 30;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->d2square_button(1, tr);

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = true;
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)
			return 4;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "D"){

		tr[0].x = x + 25;
		tr[0].y = y + 45;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 5;
		tr[1].y = y + 20;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 45;
		tr[2].y = y + 20;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->d2square_button(1, tr);

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = true;
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)
			return 3;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	return fr;
}

int Menu::mouse(Dx9Init *dx, ImageDraw *draw, int f, int *offset){//f�ŏ�������,offset�ŉ摜�S�̈ړ�

	int fr = 0;   //�L�[����
	int d3r = 0; //3D��]�p�l���L�[����
	static int mf = 0; //���N���b�N����

	if (f == 1){                       //�}�E�X����1 
		fr = square_button(dx, draw, xm, ym, 0, 0, "LF", fr);
		fr = square_button(dx, draw, xm, ym, 60, 0, "RF", fr);
		fr = rectangle_button(dx, xm, ym, 5, 60, "  �摜����", 3, fr);

	}

	if (f == 2){                  //�}�E�X����2
		fr = rectangle_button(dx, xm, ym, 0, 20, "   16��", 1, fr);
		fr = rectangle_button(dx, xm, ym, 110, 20, "   64��", 2, fr);
		fr = rectangle_button(dx, xm, ym, 220, 20, "   256��", 3, fr);
		fr = rectangle_button(dx, xm, ym, 330, 20, "  1600��", 4, fr);
		fr = rectangle_button(dx, xm, ym, 440, 20, "  10000��", 5, fr);
		fr = rectangle_button(dx, xm, ym, 550, 20, " 160000��", 6, fr);

	}

	if (f == 3){  //�}�E�X����3
		draw->putfin(false); //�摜�����t���O������
		fin_g = false;      //�摜�����t���O������

		fr = square_button(dx, draw, xm, ym, 0, 50, "L", fr);
		fr = square_button(dx, draw, xm, ym, 100, 50, "R", fr);
		fr = square_button(dx, draw, xm, ym, 50, 0, "U", fr);
		fr = square_button(dx, draw, xm, ym, 50, 100, "D", fr);
		fr = rectangle_button(dx, xm, ym, 170, 50, "   ���ʂ�", 5, fr);
		fr = rectangle_button(dx, xm, ym, 170, 75, " �V���b�t��", 6, fr);
		fr = rectangle_button(dx, xm, ym, 290, 50, "   ����{", 7, fr);
		fr = rectangle_button(dx, xm, ym, 410, 50, "  ���ύX", 8, fr);
		fr = rectangle_button(dx, xm, ym, 410, 75, "  �摜�ύX", 9, fr);
		fr = rectangle_button(dx, xm, ym, 4, 160, "  �ʏ�摜", 10, fr);
		fr = rectangle_button(dx, xm, ym, 4, 190, "  ���m�N��", 11, fr);
		fr = rectangle_button(dx, xm, ym, 4, 220, "  ���U�C�N", 12, fr);
		fr = rectangle_button(dx, xm, ym, 290, 75, "  �萔�v�Z", 13, fr);
		fr = rectangle_button(dx, xm, ym, 4, 250, " �G�b�W���o", 14, fr);
		fr = rectangle_button(dx, xm, ym, 4, 280, "  �G���{�X", 15, fr);
		fr = rectangle_button(dx, xm, ym, 4, 310, "   �G�敗", 16, fr);
		fr = rectangle_button(dx, xm, ym, 4, 340, "  ��ʌ��o", 17, fr);
		fr = rectangle_button(dx, xm, ym, 4, 370, "��ʃ��U�C�N", 18, fr);
		fr = rectangle_button(dx, xm, ym, 4, 400, "  �l�K�|�W", 19, fr);
		fr = rectangle_button(dx, xm, ym, 4, 430, "�摜�G���{�X", 20, fr);
		fr = rectangle_button(dx, xm, ym, 4, 460, " �炷���ւ�", 21, fr);
		fr = rectangle_button(dx, xm, ym, 530, 50, " 3D���[�h", 22, fr);
		fr = rectangle_button(dx, xm, ym, 530, 75, " �L���v�`��", 23, fr);
		if (draw->getmcf() == 1)fr = rectangle_button(dx, xm, ym, 350, 521, "  �W�����x", 24, fr);
		if (draw->getmcf() != 0)fr = rectangle_button(dx, xm, ym, 4, 520, "  �دĽ���", 25, fr);
		fr = rectangle_button(dx, xm, ym, 4, 490, " �m�C�Y����", 26, fr);

		if (d3f == 1){//3D�摜��]

			d3r = square_button(dx, draw, xm, ym, 150, 200, "LF", d3r);
			d3r = square_button(dx, draw, xm, ym, 600, 200, "RF", d3r);
			draw->theta(d3r, 10);//�J�����p�x����
		}

		if (draw->getmcf() == 1){//����t�@�C�����̂ݎ��s
			//�V�[�N�o�[
			re[0].x = 200;
			re[0].y = 460;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (0 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 480;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (0 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 599;
			re[2].y = 480;
			re[2].z = 0.0f;
			re[2].rhw = 0.0f;
			re[2].color = (0 << 16) + (0 << 8) + 255;
			re[2].tu = 1.0f;
			re[2].tv = 1.0f;

			re[3].x = 599;
			re[3].y = 460;
			re[3].z = 0.0f;
			re[3].rhw = 0.0f;
			re[3].color = (0 << 16) + (0 << 8) + 155;
			re[3].tu = 1.0f;
			re[3].tv = 0.0f;

			dx->d2square_button(2, re);

			re[0].x = 200;
			re[0].y = 460;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (200 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 480;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (200 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 200 + ((float)draw->gettime1() / (float)draw->gettime2()) * 399.0f;
			re[2].y = 480;
			re[2].z = 0.0f;
			re[2].rhw = 0.0f;
			re[2].color = (200 << 16) + (0 << 8) + 255;
			re[2].tu = 1.0f;
			re[2].tv = 1.0f;

			re[3].x = 200 + ((float)draw->gettime1() / (float)draw->gettime2()) * 399.0f;
			re[3].y = 460;
			re[3].z = 0.0f;
			re[3].rhw = 0.0f;
			re[3].color = (200 << 16) + (0 << 8) + 155;
			re[3].tu = 1.0f;
			re[3].tv = 0.0f;

			dx->d2square_button(2, re);

			//�V�[�N�o�[���Ƀ}�E�X�J�[�\���L
			if (xm >= 200 && xm <= 599 && ym >= 460 && ym <= 480){
				//���ԕ\��
				char time_s[256];
				sprintf(time_s, " %d��%d��%d�b", (int)draw->gettime1() / 3600, ((int)draw->gettime1() / 60) % 60, (int)draw->gettime1() % 60);
				char time_e[256];
				sprintf(time_e, " %d��%d��%d�b", (int)draw->gettime2() / 3600, ((int)draw->gettime2() / 60) % 60, (int)draw->gettime2() % 60);
				dx->text(time_s, 100, 460);
				dx->text(time_e, 600, 460);
				if (clf)//���N���b�N
					//�Đ��ʒu�ύX
					draw->putmediapos(draw->gettime2() * (xm - 200) / 399);
			}

			//�Đ����x�����o�[(//�Đ����x1.0�W�� �L���͈�0.1�`4.0)
			re[0].x = 200;
			re[0].y = 500;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (0 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 520;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (0 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 599;
			re[2].y = 520;
			re[2].z = 0.0f;
			re[2].rhw = 0.0f;
			re[2].color = (0 << 16) + (0 << 8) + 255;
			re[2].tu = 1.0f;
			re[2].tv = 1.0f;

			re[3].x = 599;
			re[3].y = 500;
			re[3].z = 0.0f;
			re[3].rhw = 0.0f;
			re[3].color = (0 << 16) + (0 << 8) + 155;
			re[3].tu = 1.0f;
			re[3].tv = 0.0f;

			dx->d2square_button(2, re);

			re[0].x = 200;
			re[0].y = 500;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (200 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 520;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (200 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 200 + draw->getrate() / 4.0f * 399.0f;
			re[2].y = 520;
			re[2].z = 0.0f;
			re[2].rhw = 0.0f;
			re[2].color = (200 << 16) + (0 << 8) + 255;
			re[2].tu = 1.0f;
			re[2].tv = 1.0f;

			re[3].x = 200 + draw->getrate() / 4.0f* 399.0f;
			re[3].y = 500;
			re[3].z = 0.0f;
			re[3].rhw = 0.0f;
			re[3].color = (200 << 16) + (0 << 8) + 155;
			re[3].tu = 1.0f;
			re[3].tv = 0.0f;

			dx->d2square_button(2, re);

			//�Đ����x�����o�[�Ƀ}�E�X�J�[�\���L
			if (xm >= 200 && xm <= 599 && ym >= 500 && ym <= 520){

				if (clf)//���N���b�N
					//�Đ��ʒu�ύX
					draw->putrate((xm - 200) / 399.0f * 4.0f);
			}

		}//����t�@�C�����̂ݎ��s

		if (fin_g == true)draw->putfin(1);

	}

	if (f == 4){                 //�}�E�X����4
		fr = rectangle_button(dx, xm, ym, 100, 20, "�n�C�X�s�[�h", 1, fr);
		fr = rectangle_button(dx, xm, ym, 220, 20, "   ���~", 2, fr);
		fr = rectangle_button(dx, xm, ym, 340, 20, " �҃X�s�[�h", 3, fr);

	}

	if (f == 5){   //�}�E�X����5,�萔�v�Z
		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && clf == 1)fr = 1;//�摜��������,�����N���b�N
	}

	if (f == 6){//�}�E�X����6,�摜�G���{�X
		fr = square_button(dx, draw, xm, ym, 150, 400, "LF", fr);
		fr = square_button(dx, draw, xm, ym, 600, 400, "RF", fr);
	}

	if (sf == 1 && fr != 0 && f != 1 && mf == 0)sound->sound();//�{�^�������ꂽ��炷

	//�{�^���������ςȂ��̏ꍇ�̏���
	if (fr == 0){ mf = 0; return 0; }
	if (f == 1 && mf == 1)return 0;
	if (f == 3 && mf == 1 && fr > 4)return 0;
	mf = 1;

	return fr;
}

Menu::Menu(){

	d3f = 0;
	d3b = 0;
	sf = 1;//���ʉ��L
	sound = new Sound_();
}

Menu::Menu(int s){
	sf = 0;//���ʉ�����,�I�u�W�F�N�g��������
}

Menu::~Menu(){
	if (sf){//���ʉ��L�̎��̂ݎ��s
		delete sound;
		sound = NULL;
	}
}