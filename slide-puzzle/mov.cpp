//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�ړ������@�@�@�@�@                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "struct_set.h"
#include "mouse.h"
#include "sound.h"
#include "drawing_img.h"

int mov(alldata *p,int *cnt,int frg, int autof){  //�ړ������֐��錾 cnt�ړ��֎~�t���O,frg�ړ�����,autof=1:auto_matic�֐����s��
	
	int i;             //for
	int k = -1;       //������
	int mv;          //�ړ��ʕύX�p
	int ms = 0;     //�҃X�s�[�h���[�h�t���O
	int mov_p = 0; //�ړ����{����p
	int offset[2];//�摜���W�I�t�Z�b�g
	para *prs = &p->paras[p->size];
	imxy *img = p->img;
	int *sp = p->space;
	int stl; //�t�@�C����������

	offset[0] = 0, offset[1] = 0;//������
		
	for (i = 0; i<prs->idx; i++){
		//��auto_matic()�֐��̃u���b�N�Փ˔��菈��
		if (autof == 1 && cnt[i] == 1){ k = -1; continue; }
		//���ړ��L�[���ɑS�u���b�N�ƃX�y�[�X�̍��W��r,�ړ��Ώۃu���b�N����(�Y����k)
		if (frg == 1 && img[i].cx == sp[0] + prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (frg == 2 && img[i].cx == sp[0] - prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (frg == 3 && img[i].cx == sp[0] && img[i].cy == sp[1] + prs->bsize){ k = i; break; }
		if (frg == 4 && img[i].cx == sp[0] && img[i].cy == sp[1] - prs->bsize){ k = i; break; }

	}  //for�I���


	if (k >= 0){             //�ړ��Ώۃu���b�N���莞�̂ݎ��s
		if (p->tkf != 1){   //�萔�v�Z�t���O1�ȊO�Ɏ��s
			mv = prs->move;//�ʏ�ړ��ʏ�����
			stl = strlen(p->g_name) - 4;
			if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi") ||
				!strcmp(p->g_name, "z_cam_ewc.bmp")){//����u���b�N��,�ړ��ʂɍX�V
				mv = prs->movem;                    //����u���b�N���ړ��ʍX�V
			}

			if (autof == 1 && mouse(p,4, offset) == 1)mv = prs->bsize;//automatic�֐����s���n�C�X�s�[�h
			if (autof == 1 && mouse(p,4, offset) == 2)return 2;      //automatic�֐����~
			if (autof == 1 && mouse(p,4, offset) == 3){ mv = prs->bsize; ms = 1; }//�҃X�s�[�h�I��
			
			if (strcmp(p->g_name + stl, ".mpg") && strcmp(p->g_name + stl, ".avi"))sound(2);//�T�E���h�֐��u���b�N�ړ�����(�Î~��̎��̂ݎ��s)
		}
		else{//�萔�v�Z�t���O1�̎����s
			p->tkc++;//�萔�J�E���g
			mv = prs->bsize;//�҃X�s�[�h
			if (mouse(p,5, 0) == 1){ p->cnt_results = -1; return 2; }//�萔�v�Z����,�萔�v�Z���~
		}

		while (img[k].cx != sp[0] || img[k].cy != sp[1]){ //�ړ������W�X�V����,�X�y�[�X���W�ɓ��B������I��
			
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(cnt); free(img); return-1; }//�����I��
								
			if (frg == 1)img[k].cx -= mv;//���W�l�X�V �u���b�N���ړ�
            if (frg == 2)img[k].cx += mv;//���W�l�X�V �u���b�N�E�ړ�
			if (frg == 3)img[k].cy -= mv;//���W�l�X�V �u���b�N��ړ�
			if (frg == 4)img[k].cy += mv;//���W�l�X�V �u���b�N���ړ�

			if (ms == 0 && p->tkf == 0) {//�҃X�s�[�hoff���`��
				drawing_img(p, offset[0], offset[1], 0);//�摜�`��
				if (autof == 0)mouse(p,3, 0); else mouse(p,4, offset);//�}�E�X�����`��̂�
				ScreenFlip(); //�\��ʕ`��
			}
         }//while�I��
		
		//���ړ�������X�y�[�X���W�X�V
		if (frg == 1)sp[0] += prs->bsize;
		if (frg == 2)sp[0] -= prs->bsize;
		if (frg == 3)sp[1] += prs->bsize;
		if (frg == 4)sp[1] -= prs->bsize;
		
	} //if�I���
	if (k >= 0)mov_p = 1;
	
	return mov_p;
}  //mov()�I���
