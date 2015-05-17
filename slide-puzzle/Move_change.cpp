//*****************************************************************************************//
//**                                                                                     **//
//**                   �@Move�N���X�����\�b�h��`(�u���b�N�V���b�t��)                    **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Filter.h"

int Move::change(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw){ //�u���b�N�����֐��錾

	int i;           //for�p
	int cnt;        //�ړ������u���b�N���J�E���g 
	para *prs = &paras[size];
	int *sp = space;
	int mv;       //�ړ��ʕύX�p

	mv = prs->move;//�ʏ�u���b�N�ړ��ʂɏ�����

	if (draw->d.mcf == 1 || draw->d.mcf == 2){//����u���b�N��,�ړ��ʂɍX�V
		mv = prs->movem;           //����u���b�N���ړ��ʍX�V
	}

	for (int i = 0; i < prs->idx; i++){//z�����ړ���i�[
		img[i].chz = (rand() % 600) - 300;
	}

	cnt = 0;
	while (cnt < prs->idx){//z������яo������
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		cnt = 0;   //for��idx��I�����_��cnt�K��l���ۂ����肷���for���J�n�O�͏�����
		for (i = 0; i < prs->idx; i++){ //�u���b�N�����J��Ԃ�

			//z�ړ���������,cnt�K��l�ɂȂ�܂ŏ���
			if (img[i].cz == img[i].chz){ cnt++; continue; }    //���B�㏈��

			if ((img[i].chz - img[i].cz) < 0){                 //-�����ړ��㏈��
				img[i].cz -= mv; if ((img[i].chz - img[i].cz) >= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}

			if ((img[i].chz - img[i].cz) > 0){                //+�����ړ��㏈��
				img[i].cz += mv; if ((img[i].chz - img[i].cz) <= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}
		}

		draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//�摜�`��
		dx->drawscreen();//�`��
	}//z������яo�������I��

	cnt = 0;
	while (cnt < prs->count){//xy�ړ�����

		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		cnt = 0;   //for��(idx-1)��I�����_��cnt�K��l���ۂ����肷���for���J�n�O�͏�����
		for (i = 0; i < prs->idx - 1; i++){                   //�u���b�N�����J��Ԃ�

			if (img[i].cx == img[i].chx){ cnt++; }
			else if ((img[i].chx - img[i].cx) < 0){ img[i].cx -= mv; }  //x�ړ���������,cnt�K��l�ɂȂ�܂ŏ���
			else if ((img[i].chx - img[i].cx) > 0){ img[i].cx += mv; }

			if (img[i].cy == img[i].chy){ cnt++; }
			else if ((img[i].chy - img[i].cy) < 0){ img[i].cy -= mv; }  //y�ړ���������,cnt�K��l�ɂȂ�܂ŏ���
			else if ((img[i].chy - img[i].cy) > 0){ img[i].cy += mv; }

		}//for�I���

		if (img[prs->idx - 1].cx == prs->lastposx){ cnt++; }  //�E���摜�E�ړ�
		else { img[prs->idx - 1].cx -= mv; }

		draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//�摜�`��
		dx->drawscreen();//�`��
	}//xy�ړ��I��

	sp[0] = 400;                  //�X�y�[�Xx���W������
	sp[1] = prs->lastposy;       //�X�y�[�Xy���W������

	for (i = 0; i < prs->idx; i++){//z�����߂�
		img[i].chz = 0;
	}

	cnt = 0;
	while (cnt < prs->idx){//z�����߂�
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		cnt = 0;   //for��idx��I�����_��cnt�K��l���ۂ����肷���for���J�n�O�͏�����
		for (i = 0; i < prs->idx; i++){ //�u���b�N�����J��Ԃ�

			//z�ړ���������,cnt�K��l�ɂȂ�܂ŏ���
			if (img[i].cz == img[i].chz){ cnt++; continue; }    //���B�㏈��

			if ((img[i].chz - img[i].cz) < 0){                 //-�����ړ��㏈��
				img[i].cz -= mv; if ((img[i].chz - img[i].cz) >= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}

			if ((img[i].chz - img[i].cz) > 0){                //+�����ړ��㏈��
				img[i].cz += mv; if ((img[i].chz - img[i].cz) <= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}
		}

		draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//�摜�`��
		dx->drawscreen();//�`��
	}//z�����߂�I���

	while (1){
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//���[�v�̏I��
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}
		int the = draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//�摜�`��
		dx->drawscreen();//�`��
		if (the == 0)break;
	}

	return 0;
} //change()�I���	


int Move::shuffle(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw){   //�V���b�t���֐��錾 

	int i, j, k, tempx, tempy;
	para *prs = &paras[size];

	k = 0;
	for (j = 0; j <= prs->lastposy; j += prs->bsize){    //���W�f�[�^������
		for (i = 0; i <= prs->lastposx; i += prs->bsize){
			img[k].chx = i;
			img[k++].chy = j;
		}
	}

	for (i = 0; i < prs->rand_i; i++){  //img[]����chx,chy�������_���Ńf�[�^����ւ�
		//size==5�̏ꍇ�̏���(rand�֐�max�l������Ȃ���)
		if (size == 5){
			j = (rand() % 10000) * 15 + rand() % 10014; k = (rand() % 10000) * 15 + rand() % 10014;
		}
		else{                 //size==0�`4�̏ꍇ�̏���
			j = rand() % prs->rand_s;   //�������Y����j
			k = rand() % prs->rand_s;   //������Y����k
			if (j == k){ i--; continue; }
		}
		tempx = img[j].chx;    //����
		tempy = img[j].chy;
		img[j].chx = img[k].chx;
		img[j].chy = img[k].chy;
		img[k].chx = tempx;
		img[k].chy = tempy;
	} //for�I���  		

	return change(dx, msg, filter, draw);//change�Ԃ�l�����̂܂ܕԂ�
}

int Move::recovery(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw){//���ʂ�

	for (int i = 0; i < paras[size].idx; i++){
		img[i].chx = img[i].fx; img[i].chy = img[i].fy;
	}

	return change(dx, msg, filter, draw);//change�Ԃ�l�����̂܂ܕԂ�
}