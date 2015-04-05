//*****************************************************************************************//
//**                                                                                     **//
//**                   �@Move�N���X�����\�b�h��`(�u���b�N�V���b�t��)                    **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Filter.h"

int Move::change(Filter *filter, ImageDraw *draw){              //�u���b�N�����֐��錾

	int i; //for�p
	int cnt = 0;   //�ړ������u���b�N���J�E���g 
	para *prs = &paras[size];
	int *sp = space;
	int mv;       //�ړ��ʕύX�p

	mv = prs->move;//�ʏ�u���b�N�ړ��ʂɏ�����

	if (draw->d.mcf == 1 || draw->d.mcf == 2){//����u���b�N��,�ړ��ʂɍX�V
		mv = prs->movem;           //����u���b�N���ړ��ʍX�V
	}

	while (ScreenFlip() == 0 && cnt < prs->count){
		if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End();  free(img); return -1; }
		cnt = 0;   //for��(idx-1)��I�����_��cnt�K��l���ۂ����肷���for���J�n�O�͏�����

		for (i = 0; i < prs->idx - 1; i++){                   //�u���b�N�����J��Ԃ�

			if (img[i].cx == img[i].chx){ cnt++; }
			else if ((img[i].chx - img[i].cx) < 0){ img[i].cx -= mv; }  //x�ړ���������,cnt�K��l�ɂȂ�܂ŏ���
			else if ((img[i].chx - img[i].cx) > 0){ img[i].cx += mv; }

			if (img[i].cy == img[i].chy){ cnt++; }
			else if ((img[i].chy - img[i].cy) < 0){ img[i].cy -= mv; }  //y�ړ���������,cnt�K��l�ɂȂ�܂ŏ���
			else if ((img[i].chy - img[i].cy) > 0){ img[i].cy += mv; }

		}   //for�I���

		if (img[prs->idx - 1].cx == prs->lastposx){ cnt++; }                 //�E���摜�E�ړ�
		else { img[prs->idx - 1].cx -= mv; }

		draw->drawing_img(filter, this, 0, 0, 0);//�摜�`��


	}    //while�I���
	sp[0] = 600;                  //�X�y�[�Xx���W������
	sp[1] = prs->lastposy;       //�X�y�[�Xy���W������
	return 0;
}  //change()�I���	


void Move::shuffle(){   //�V���b�t���֐��錾 

	int i, j, k, tempx, tempy;
	para *prs = &paras[size];

	k = 0;
	for (j = 100; j <= prs->lastposy; j += prs->bsize){    //���W�f�[�^������
		for (i = 200; i <= prs->lastposx; i += prs->bsize){
			img[k].chx = i;
			img[k++].chy = j;
		}
	}

	for (i = 0; i < prs->rand_i; i++){         // img[]����chx,chy�������_���Ńf�[�^����ւ�
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

}   //shuffle()�I��� 