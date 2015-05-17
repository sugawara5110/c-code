//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@Menu�N���X�����\�b�h��`                                **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Menu.h"
#include "Sound_.h"

// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

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

	dx->pD3DDevice->BeginScene();
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// �`��
	dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &re, sizeof(MY_VERTEX2));
	//����
	RECT r = { (long)x, (long)y + 2, (long)x, (long)y + 2 };
	dx->pD3DFont->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	//�e�N�X�`���[
	dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	dx->pD3DDevice->EndScene();
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
	dx->pD3DDevice->BeginScene();
	//�e�N�X�`���[
	dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// �`��
	dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &sq, sizeof(MY_VERTEX2));
	dx->pD3DDevice->EndScene();
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

		dx->pD3DDevice->BeginScene();
		//�e�N�X�`���[
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// �`��
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

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

		dx->pD3DDevice->BeginScene();
		//�e�N�X�`���[
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// �`��
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

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

		dx->pD3DDevice->BeginScene();
		//�e�N�X�`���[
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// �`��
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;//�}�E�X���W==�{�^���͈͓����H
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

		dx->pD3DDevice->BeginScene();
		//�e�N�X�`���[
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// �`��
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;//�}�E�X���W==�{�^���͈͓����H
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

		dx->pD3DDevice->BeginScene();
		//�e�N�X�`���[
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// �`��
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;
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

		dx->pD3DDevice->BeginScene();
		//�e�N�X�`���[
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// �`��
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;
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
		draw->finish = 0; //�摜�����t���O������
		fin_g = 0;       //�摜�����t���O������

		fr = square_button(dx, draw, xm, ym, 0, 50, "L", fr);
		fr = square_button(dx, draw, xm, ym, 100, 50, "R", fr);
		fr = square_button(dx, draw, xm, ym, 50, 0, "U", fr);
		fr = square_button(dx, draw, xm, ym, 50, 100, "D", fr);
		fr = rectangle_button(dx, xm, ym, 170, 50, "   ���ʂ�", 5, fr);
		fr = rectangle_button(dx, xm, ym, 290, 50, " �V���b�t��", 6, fr);
		fr = rectangle_button(dx, xm, ym, 410, 50, "   ����{", 7, fr);
		fr = rectangle_button(dx, xm, ym, 530, 50, "  ���ύX", 8, fr);
		fr = rectangle_button(dx, xm, ym, 650, 50, "  �摜�ύX", 9, fr);
		fr = rectangle_button(dx, xm, ym, 10, 160, " �ʏ�摜", 10, fr);
		fr = rectangle_button(dx, xm, ym, 10, 190, " ���m�N��", 11, fr);
		fr = rectangle_button(dx, xm, ym, 10, 220, " ���U�C�N", 12, fr);
		fr = rectangle_button(dx, xm, ym, 410, 75, "  �萔�v�Z", 13, fr);
		fr = rectangle_button(dx, xm, ym, 10, 250, " �G�b�W���o", 14, fr);
		fr = rectangle_button(dx, xm, ym, 10, 280, " �G���{�X", 15, fr);
		fr = rectangle_button(dx, xm, ym, 10, 310, "  �G�敗", 16, fr);
		fr = rectangle_button(dx, xm, ym, 10, 340, " ��ʌ��o", 17, fr);
		fr = rectangle_button(dx, xm, ym, 10, 370, "��ʃ��U�C�N", 18, fr);
		fr = rectangle_button(dx, xm, ym, 10, 400, " �l�K�|�W", 19, fr);
		fr = rectangle_button(dx, xm, ym, 10, 430, "�摜�G���{�X", 20, fr);
		fr = rectangle_button(dx, xm, ym, 10, 460, "�炷���ւ�", 21, fr);
		fr = rectangle_button(dx, xm, ym, 650, 75, " 3D���[�h", 22, fr);

		if (d3f == 1){//3D�摜��]

			d3r = square_button(dx, draw, xm, ym, 150, 200, "LF", d3r);
			d3r = square_button(dx, draw, xm, ym, 600, 200, "RF", d3r);
			draw->theta(d3r, 10);//�J�����p�x����
		}

		if (draw->d.mcf == 1){//����t�@�C�����̂ݎ��s
			//�V�[�N�o�[
			re[0].x = 200;
			re[0].y = 500;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (0 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 530;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (0 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 599;
			re[2].y = 530;
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

			dx->pD3DDevice->BeginScene();
			//�e�N�X�`���[
			dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
			dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
			// �`��
			dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &re, sizeof(MY_VERTEX2));
			dx->pD3DDevice->EndScene();

			re[0].x = 200;
			re[0].y = 500;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (200 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 530;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (200 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 200 + ((float)draw->d.time1 / (float)draw->d.time2) * 399.0f;
			re[2].y = 530;
			re[2].z = 0.0f;
			re[2].rhw = 0.0f;
			re[2].color = (200 << 16) + (0 << 8) + 255;
			re[2].tu = 1.0f;
			re[2].tv = 1.0f;

			re[3].x = 200 + ((float)draw->d.time1 / (float)draw->d.time2) * 399.0f;
			re[3].y = 500;
			re[3].z = 0.0f;
			re[3].rhw = 0.0f;
			re[3].color = (200 << 16) + (0 << 8) + 155;
			re[3].tu = 1.0f;
			re[3].tv = 0.0f;

			dx->pD3DDevice->BeginScene();
			//�e�N�X�`���[
			dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
			dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
			// �`��
			dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &re, sizeof(MY_VERTEX2));
			dx->pD3DDevice->EndScene();

			//�V�[�N�o�[���Ƀ}�E�X�J�[�\���L
			if (xm >= 200 && xm <= 599 && ym >= 500 && ym <= 530){

				if (clf)//���N���b�N
					//�Đ��ʒu�ύX
					draw->d.pMediaPosition->put_CurrentPosition(draw->d.time2 * (xm - 200) / 399);
			}
		}

		if (fin_g == 1)draw->finish = 1;

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
		square_button(dx, draw, xm, ym, 0, 50, "L", fr);   //draw->finish = 1���擾�̈�
		square_button(dx, draw, xm, ym, 100, 50, "R", fr);//draw->finish = 1���擾�̈�
		square_button(dx, draw, xm, ym, 50, 0, "U", fr);   //draw->finish = 1���擾�̈�
		square_button(dx, draw, xm, ym, 50, 100, "D", fr);//draw->finish = 1���擾�̈�
		fr = square_button(dx, draw, xm, ym, 150, 500, "LF", fr);
		fr = square_button(dx, draw, xm, ym, 600, 500, "RF", fr);

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