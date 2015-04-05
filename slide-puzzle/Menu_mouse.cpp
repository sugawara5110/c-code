//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@Menu�N���X�����\�b�h��`                                **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Menu.h"
                    
int Menu::rectangle_button(int xm, int ym, int bt2on, int bt2of, int x, int y, char *str, int f, int fr){//�{�^���p�֐�
	DrawGraph(x, y, bt2on, TRUE);//�����ĂȂ��{�^���`��
	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y)DrawGraph(x, y, bt2of, TRUE);//�}�E�X���W==�{�^���͈͓����H
	DrawFormatString(x, y + 2, GetColor(10, 10, 10), str);
	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
		return f;//�{�^���͈͓��N���b�N
	return fr;  //���������ĂȂ��ꍇ���̐��l��Ԃ�
}

int Menu::square_button(ImageDraw *draw, int xm, int ym, int bt1on, int bt1of, int x, int y, char *str, int fr) {//���p�֐�

	int cr;//�L���F
	cr = GetColor(0, 0, 255);
	
	if (str == "LF"){//File�֐��p�����摜�\������

		DrawGraph(x, y, bt1on, TRUE);//�����ĂȂ��{�^���`��,���
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)DrawGraph(x, y, bt1of, TRUE);//�}�E�X���W==�{�^���͈͓����H
		DrawTriangle(5 + x, 25 + y, 30 + x, 5 + y, 30 + x, 45 + y, cr, TRUE);   //���}�`�`��,��
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
			return 1;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "RF"){//File�֐��p�����摜�\������

		DrawGraph(x, y, bt1on, TRUE);//�����ĂȂ��{�^���`��,���E
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)DrawGraph(x, y, bt1of, TRUE);//�}�E�X���W==�{�^���͈͓����H
		DrawTriangle(45 + x, 25 + y, 20 + x, 5 + y, 20 + x, 45 + y, cr, TRUE); //���}�`�`��,�E
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
			return 2;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "L"){

		DrawGraph(x, y, bt1on, TRUE);//�����ĂȂ��{�^���`��,���
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }//�}�E�X���W==�{�^���͈͓����H
		DrawTriangle(5 + x, 25 + y, 30 + x, 5 + y, 30 + x, 45 + y, cr, TRUE);   //���}�`�`��,��
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
			return 1;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "R"){

		DrawGraph(x, y, bt1on, TRUE);//�����ĂȂ��{�^���`��,���E
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }//�}�E�X���W==�{�^���͈͓����H
		DrawTriangle(45 + x, 25 + y, 20 + x, 5 + y, 20 + x, 45 + y, cr, TRUE); //���}�`�`��,�E
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
			return 2;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "U"){

		DrawGraph(x, y, bt1on, TRUE);//����
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }
		DrawTriangle(25 + x, 5 + y, 5 + x, 30 + y, 45 + x, 30 + y, cr, TRUE);       //����}�`�`��
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			return 3;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	if (str == "D"){

		DrawGraph(x, y, bt1on, TRUE);//���
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }
		DrawTriangle(25 + x, 45 + y, 5 + x, 20 + y, 45 + x, 20 + y, cr, TRUE);   //��󉺐}�`�`��
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			return 4;//�{�^���͈͓��N���b�N
		return fr;//��N���b�N�����̐��l��Ԃ�
	}

	return fr;
}

int Menu::mouse(ImageDraw *draw, int f, int *offset){//f�ŏ�������,offset�ŉ摜�S�̈ړ�

	static int bt1on, bt1of, bt2on, bt2of;   //�{�^���摜�n���h��
	int i, j;   //for�p
	int fr = 0;//�L�[����
	int cr;    //�L���F
	int xm, ym;   //�}�E�X���W
	static int xof, yof;//���N���b�N�����W,�}�E�X����4�Ŏg�p
	static int mf;     //���N���b�N����,�}�E�X����4�Ŏg�p
	cr = GetColor(0, 0, 255);

	if (f != 4)mf = 0;//�摜�I�t�Z�b�g����������

	if (f == 0){//��������

		bt1on = MakeXRGB8ColorSoftImage(50, 50);//�n���h���ɃJ���摜�ݒ�
		bt1of = MakeXRGB8ColorSoftImage(50, 50);
		bt2on = MakeXRGB8ColorSoftImage(100, 20);
		bt2of = MakeXRGB8ColorSoftImage(100, 20);

		for (i = 0; i < 50; i++){                    //�{�^������1
			for (j = 0; j < 50; j++){

				DrawPixelSoftImage(bt1on, j, i, 185, 185, 185, 0);  //�����Ȃ���Ԃ̐F�ݒ�
				DrawPixelSoftImage(bt1of, j, i, 250, 250, 250, 0); //�����Ă��Ԃ̐F�ݒ�
				if (j == 0 || j == 49 || i == 0 || i == 49){      //�O���F�ݒ�1
					DrawPixelSoftImage(bt1on, j, i, 20, 20, 20, 0);
					DrawPixelSoftImage(bt1of, j, i, 90, 90, 90, 0);
				}
				if ((i != 0 && i != 49 && j != 0 && j != 49) && (j == 1 || j == 48 || i == 1 || i == 48)){//�O���F�ݒ�2
					DrawPixelSoftImage(bt1on, j, i, 70, 70, 70, 0);
					DrawPixelSoftImage(bt1of, j, i, 140, 140, 140, 0);
				}
				if ((i >= 2 && i <= 47 && j >= 2 && j <= 47) && (j == 2 || j == 47 || i == 2 || i == 47)){//�O���F�ݒ�3
					DrawPixelSoftImage(bt1on, j, i, 140, 140, 140, 0);
					DrawPixelSoftImage(bt1of, j, i, 210, 210, 210, 0);
				}
			}
		}

		for (i = 0; i < 20; i++){                        //�{�^������2
			for (j = 0; j < 100; j++){

				DrawPixelSoftImage(bt2on, j, i, 180, 185, 185, 0);  //�����Ȃ���Ԃ̐F�ݒ�
				DrawPixelSoftImage(bt2of, j, i, 250, 250, 250, 0); //��������Ԃ̐F�ݒ�
				if (j == 0 || j == 99 || i == 0 || i == 19){      //�O���F�ݒ�1
					DrawPixelSoftImage(bt2on, j, i, 20, 20, 20, 0);
					DrawPixelSoftImage(bt2of, j, i, 90, 90, 90, 0);
				}
				if ((i != 0 && i != 19 && j != 0 && j != 99) && (j == 1 || j == 98 || i == 1 || i == 18)){//�O���F�ݒ�2
					DrawPixelSoftImage(bt2on, j, i, 70, 70, 70, 0);
					DrawPixelSoftImage(bt2of, j, i, 140, 140, 140, 0);
				}
				if ((i >= 2 && i <= 17 && j >= 2 && j <= 97) && (j == 2 || j == 97 || i == 2 || i == 17)){//�O���F�ݒ�3
					DrawPixelSoftImage(bt2on, j, i, 140, 140, 140, 0);
					DrawPixelSoftImage(bt2of, j, i, 210, 210, 210, 0);
				}
			}
		}

		bt1on = CreateGraphFromSoftImage(bt1on);//�{�^��������n���h���Ɋi�[
		bt1of = CreateGraphFromSoftImage(bt1of);
		bt2on = CreateGraphFromSoftImage(bt2on);
		bt2of = CreateGraphFromSoftImage(bt2of);

	}

	if (f == 1){                       //�}�E�X����1 
		GetMousePoint(&xm, &ym);      //�}�E�X���W�擾                
		fr = square_button(draw, xm, ym, bt1on, bt1of, 0, 0, "LF", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 60, 0, "RF", fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 5, 60, "  �摜����", 3, fr);

	}

	if (f == 2){                  //�}�E�X����2
		GetMousePoint(&xm, &ym);  //�}�E�X���W�擾
		fr = rectangle_button(xm, ym, bt2on, bt2of, 0, 20, "   16��", 1, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 110, 20, "   64��", 2, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 220, 20, "   256��", 3, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 330, 20, "  1600��", 4, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 440, 20, "  10000��", 5, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 550, 20, " 160000��", 6, fr);

	}

	if (f == 3){                 //�}�E�X����3
		draw->finish = 0;//�摜�����t���O������
		fin_g = 0;      //�摜�����t���O������

		GetMousePoint(&xm, &ym);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 0, 50, "L", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 100, 50, "R", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 50, 0, "U", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 50, 100, "D", fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 170, 50, "   ���ʂ�", 5, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 290, 50, " �V���b�t��", 6, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 410, 50, "   ����{", 7, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 530, 50, "  ���ύX", 8, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 650, 50, "  �摜�ύX", 9, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 160, " �ʏ�摜", 10, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 200, " ���m�N��", 11, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 240, " ���U�C�N", 12, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 410, 75, "  �萔�v�Z", 13, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 280, " �G�b�W���o", 14, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 320, " �G���{�X", 15, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 360, "  �G�敗", 16, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 400, " ��ʌ��o", 17, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 440, "��ʃ��U�C�N", 18, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 480, " �l�K�|�W", 19, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 520, "�摜�G���{�X", 20, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 560, "�炷���ւ�", 21, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 650, 75, " 3D���[�h", 22, fr);

		if (fin_g == 1)draw->finish = 1;

	}

	if (f == 4){                 //�}�E�X����4
		GetMousePoint(&xm, &ym);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 100, 20, "�n�C�X�s�[�h", 1, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 220, 20, "   ���~", 2, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 340, 20, " �҃X�s�[�h", 3, fr);

		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && mf == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0){ //�摜�I�t�Z�b�g����
			GetMousePoint(&xof, &yof); mf = 1;
		}
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0){
			xof = yof = 0; mf = 0;
		}
		if (mf == 1 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0){ offset[0] = xm - xof; offset[1] = ym - yof; }//�摜�I�t�Z�b�g����

	}

	if (f == 5){   //�}�E�X����5,�萔�v�Z
		GetMousePoint(&xm, &ym);
		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)fr = 1;//�摜��������,�����N���b�N

	}

	if (f == 6){//�}�E�X����6,�摜�G���{�X
		GetMousePoint(&xm, &ym);
		square_button(draw, xm, ym, bt1on, bt1of, 0, 50, "L", fr);   //draw->finish = 1���擾�̈�
		square_button(draw, xm, ym, bt1on, bt1of, 100, 50, "R", fr);//draw->finish = 1���擾�̈�
		square_button(draw, xm, ym, bt1on, bt1of, 50, 0, "U", fr);   //draw->finish = 1���擾�̈�
		square_button(draw, xm, ym, bt1on, bt1of, 50, 100, "D", fr);//draw->finish = 1���擾�̈�
		fr = square_button(draw, xm, ym, bt1on, bt1of, 150, 500, "LF", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 600, 500, "RF", fr);

	}

	if (f == 7){//�}�E�X����7,3D�摜
		fr = square_button(draw, xm, ym, bt1on, bt1of, 150, 200, "LF", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 600, 200, "RF", fr);
	}

	return fr;
}