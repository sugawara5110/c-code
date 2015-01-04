//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�}�E�X���W����                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "struct_set.h"
                    
int rectangle_button(int xm,int ym, int bt2on,int bt2of,int x,int y,char *str,int f,int fr){//�{�^���p�֐�
	DrawGraph(x, y, bt2on, TRUE);//�����ĂȂ��{�^���`��
	if (xm >= x && xm < 100+x && ym >= y && ym < 20+y)DrawGraph(x, y, bt2of, TRUE);//�}�E�X���W==�{�^���͈͓����H
	DrawFormatString(x, y+2, GetColor(10, 10, 10) , str);
	if (xm >= x && xm < 100+x && ym >= y && ym < 20+y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
		return f;//�{�^���͈͓��N���b�N
	return fr;  //���������ĂȂ��ꍇ���̐��l��Ԃ�
}

int mouse(alldata *p,int f,int *offset){//f�ŏ�������,offset�ŉ摜�S�̈ړ�
	
	static int bt1on, bt1of, bt2on, bt2of;   //�{�^���摜�n���h��
	int i, j;   //for�p
	int fr = 0;//�L�[����
	int cr,crs;    //�L���F
	int xm, ym;   //�}�E�X���W
	static int xof, yof;//���N���b�N�����W,�}�E�X����4�Ŏg�p
	static int mf;     //���N���b�N����,�}�E�X����4�Ŏg�p
	
	if (f != 4)mf = 0;//�摜�I�t�Z�b�g����������
	cr = GetColor(0, 0, 255);
	crs = GetColor(10, 10, 10);
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

		return 0;
	}

	if (f == 1){                       //�}�E�X����1 
		GetMousePoint(&xm, &ym);      //�}�E�X���W�擾                
		DrawGraph(0, 0, bt1on, TRUE);//�����ĂȂ��{�^���`��,���
		if (xm >= 0 && xm < 50 && ym >= 0 && ym < 50)DrawGraph(0, 0, bt1of, TRUE);//�}�E�X���W==�{�^���͈͓����H
		if (xm >= 0 && xm < 50 && ym >= 0 && ym < 50 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
			fr = 1;//�Ԃ�l
		DrawGraph(60, 0, bt1on, TRUE);//�����ĂȂ��{�^���`��,���E
		if (xm >= 60 && xm < 110 && ym >= 0 && ym < 50)DrawGraph(60, 0, bt1of, TRUE);//�}�E�X���W==�{�^���͈͓����H
		if (xm >= 60 && xm < 110 && ym >= 0 && ym < 50 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�͈͓������N���b�N
			fr = 2;//�Ԃ�l
		DrawTriangle(5, 25, 30, 5, 30, 45, cr, TRUE);   //���}�`�`��,��
		DrawTriangle(105, 25, 80, 5, 80, 45, cr, TRUE); //���}�`�`��,�E
		fr = rectangle_button(xm, ym, bt2on, bt2of, 5, 60, "  �摜����", 3, fr);
        
		return fr;
	}

	if (f == 2){                  //�}�E�X����2
		GetMousePoint(&xm, &ym);  //�}�E�X���W�擾
		fr = rectangle_button(xm, ym, bt2on, bt2of, 0, 20,   "   16��", 1, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 110, 20, "   64��", 2, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 220, 20, "   256��", 3, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 330, 20, "  1600��", 4, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 440, 20, "  10000��", 5, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 550, 20, " 160000��", 6, fr);

		return fr;
	}

	if (f == 3){                 //�}�E�X����3
		GetMousePoint(&xm, &ym);                       
		DrawGraph(0, 50, bt1on, TRUE);//���
		if (xm >= 0 && xm < 50 && ym >= 50 && ym < 100)DrawGraph(0, 50, bt1of, TRUE);
		if (xm >= 0 && xm < 50 && ym >= 50 && ym < 100 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 1;
		DrawGraph(100, 50, bt1on, TRUE);//���E
		if (xm >= 100 && xm < 150 && ym >= 50 && ym < 100)DrawGraph(100, 50, bt1of, TRUE);
		if (xm >= 100 && xm < 150 && ym >= 50 && ym < 100 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 2;
		DrawGraph(50, 0, bt1on, TRUE);//����
		if (xm >= 50 && xm < 100 && ym >= 0 && ym < 50)DrawGraph(50, 0, bt1of, TRUE);
		if (xm >= 50 && xm < 100 && ym >= 0 && ym < 50 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 3;
		DrawGraph(50, 100, bt1on, TRUE);//���
		if (xm >= 50 && xm < 100 && ym >= 100 && ym < 150)DrawGraph(50, 100, bt1of, TRUE);
		if (xm >= 50 && xm < 100 && ym >= 100 && ym < 150 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 4;
		DrawTriangle(5, 75, 30, 55, 30, 95, cr, TRUE);       //��󍶐}�`�`��
		DrawTriangle(145, 75, 120, 55, 120, 95, cr, TRUE);   //���E�}�`�`��
		DrawTriangle(75, 5, 55, 30, 95, 30, cr, TRUE);       //����}�`�`��
		DrawTriangle(75, 145, 55, 120, 95, 120, cr, TRUE);   //��󉺐}�`�`��
		
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
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 360, " ���ʉ敗", 16, fr);
		
		return fr;
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
		
			return fr;
	}

	if (f == 5){   //�}�E�X����5,�萔�v�Z
		GetMousePoint(&xm, &ym);
		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)fr = 1;//�摜��������,�����N���b�N
		return fr;
	}
	return 0;
}