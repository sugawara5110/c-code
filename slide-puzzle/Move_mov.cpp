//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@Move�N���X�����\�b�h��`(�ړ�����)�@�@�@�@�@          **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Menu.h"

int Move::mov(Dx9Init *dx, MSG *msg, ImageDraw *draw, int flg, int autof){  //�ړ������֐��錾 cnt�ړ��֎~�t���O,flg�ړ�����,autof=1:auto_matic�֐����s��

	int i;             //for
	int k = -1;       //������
	int mv;          //�ړ��ʕύX�p
	int ms = 0;     //�҃X�s�[�h���[�h�t���O
	int offset[2]; //�摜���W�I�t�Z�b�g
	para *prs = &paras[size];
	int *sp = space;
	Menu menu(0);//���j���[�I�u�W�F�N�g����

	offset[0] = 0, offset[1] = 0;//������

	for (i = 0; i < prs->idx; i++){
		//��auto_matic()�֐��̃u���b�N�Փ˔��菈��
		if (autof == 1 && cnt[i] == 1){ k = -1; continue; }
		//���ړ��L�[���ɑS�u���b�N�ƃX�y�[�X�̍��W��r,�ړ��Ώۃu���b�N����(�Y����k)
		if (flg == 1 && img[i].cx == sp[0] + prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (flg == 2 && img[i].cx == sp[0] - prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (flg == 3 && img[i].cx == sp[0] && img[i].cy == sp[1] + prs->bsize){ k = i; break; }
		if (flg == 4 && img[i].cx == sp[0] && img[i].cy == sp[1] - prs->bsize){ k = i; break; }

	}  //for�I���

	if (k >= 0){             //�ړ��Ώۃu���b�N���莞�̂ݎ��s
		if (tkf != 1){      //�萔�v�Z�t���O1�ȊO�Ɏ��s
			mv = prs->move;//�ʏ�ړ��ʏ�����

			if (autof == 1 && menu.mouse(dx, draw, 4, offset) == 1)mv = prs->bsize;//automatic�֐����s���n�C�X�s�[�h
			if (autof == 1 && menu.mouse(dx, draw, 4, offset) == 2)return 2;      //automatic�֐����~
			if (autof == 1 && menu.mouse(dx, draw, 4, offset) == 3){ mv = prs->bsize; ms = 1; }//�҃X�s�[�h�I��
		}
		else{//�萔�v�Z�t���O1�̎����s
			tkc++;//�萔�J�E���g
			mv = prs->bsize;//�҃X�s�[�h
			if (menu.mouse(dx, draw, 5, 0) == 1){ cnt_results = -1; return 2; }//�萔�v�Z����,�萔�v�Z���~
		}

		while (img[k].cx != sp[0] || img[k].cy != sp[1]){ //�ړ������W�X�V����,�X�y�[�X���W�ɓ��B������I��

			if (flg == 1)img[k].cx -= mv;//���W�l�X�V �u���b�N���ړ�
			if (flg == 2)img[k].cx += mv;//���W�l�X�V �u���b�N�E�ړ�
			if (flg == 3)img[k].cy -= mv;//���W�l�X�V �u���b�N���ړ�
			if (flg == 4)img[k].cy += mv;//���W�l�X�V �u���b�N��ړ�

			if (ms == 0 && tkf == 0) {//�҃X�s�[�hoff��,�萔�v�Z�ȊO���`��
				if (draw->draw_img(dx, this, offset[0], offset[1], 0) == -1)return -1;//�摜�`��
				if (autof == 0){ menu.mouse(dx, draw, 3, 0); if (draw->getgfl() == 9)menu.mouse(dx, draw, 6, 0); }
				else menu.mouse(dx, draw, 4, offset);//�}�E�X�����`��̂�

				dx->drawscreen();//�`��

			}
		}//while�I��

		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				br = 1;//�A�v���I��
				return -1;//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		//���ړ�������X�y�[�X���W�X�V
		if (flg == 1)sp[0] += prs->bsize;
		if (flg == 2)sp[0] -= prs->bsize;
		if (flg == 3)sp[1] += prs->bsize;
		if (flg == 4)sp[1] -= prs->bsize;

	} //if�I���
	if (k >= 0)return 1;

	return 0;
} //mov()�I���

