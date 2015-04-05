//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@Move�N���X�����\�b�h��`(����)                        **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Filter.h"

Move::Move(){//�R���X�g���N�^

	tkf = 0;              //�萔�v�Z���s�t���O
	tkc = 0;             //�萔�v�Z�J�E���g
	cnt_results = 0;    //auto_matic�֐��̃J�E���g����
	size = 0;          //�G���[�h�~ 

	paras[0] = { 16, 4, 100, 500, 400, 400, 15, 31, 5, 10 };    //�u���b�N�����̃p�����[�^�[
	paras[1] = { 64, 8, 50, 550, 450, 800, 63, 127, 5, 10 };
	paras[2] = { 256, 16, 25, 575, 475, 1000, 255, 511, 5, 5 };
	paras[3] = { 1600, 40, 10, 590, 490, 2000, 1599, 3199, 2, 5 };
	paras[4] = { 10000, 100, 4, 596, 496, 9000, 9999, 19999, 2, 4 };
	paras[5] = { 160000, 400, 1, 599, 499, 300000, 159999, 319999, 1, 1 };

	img = (Move::imxy*)malloc(sizeof(Move::imxy)*paras[size].idx);//�摜���W

	int k = 0;
	for (int j = 100; j <= paras[size].lastposy; j += paras[size].bsize){    //���W�f�[�^������
		for (int i = 200; i <= paras[size].lastposx; i += paras[size].bsize){
			img[k].cx = img[k].fx = img[k].chx = i;
			img[k].cy = img[k].fy = img[k++].chy = j;
		}
	}
}

int Move::auto_space_mov(Filter *filter, ImageDraw *draw, int *cnt, int *x, int *y){//�X�y�[�X��ړI�u���b�N�ɗאڂ���܂Ŏ����ړ�,cnt �ړ��֎~�t���O,x,y�ڕW�u���b�N

	para *prs = &paras[size];
	int *sp = space;
	int f;//mov()����
	int s = prs->bsize;
	int ret = 0;//return 1 ���ʗp

	while (1){
		if ((sp[0] == *x && sp[1] == *y - s) || (sp[0] == *x + s && sp[1] == *y - s) || //�ړI�n�ɗא�
			(sp[0] == *x + s && sp[1] == *y) || (sp[0] == *x + s && sp[1] == *y + s) ||
			(sp[0] == *x && sp[1] == *y + s) || (sp[0] == *x - s && sp[1] == *y + s) ||
			(sp[0] == *x - s && sp[1] == *y) || (sp[0] == *x - s && sp[1] == *y - s))break;

		if (sp[0] - *x > 0)f = mov(filter, draw, cnt, 2, 1);//�X�y�[�X�E�ʒu�֌W�ʂɈ�����ς�mov�֐����Ăяo��
		if (sp[0] - *x > 0 && f == 0){ mov(filter, draw, cnt, 3, 1); continue; }
		if (sp[0] - *x < 0)f = mov(filter, draw, cnt, 1, 1);//�X�y�[�X��
		if (sp[1] - *y > 0)f = mov(filter, draw, cnt, 4, 1);//�X�y�[�X�� 
		if (sp[1] - *y < 0)f = mov(filter, draw, cnt, 3, 1);//�X�y�[�X��
		if (f == 2){ ret = 1; break; }
	}//while�I���
	if (ret == 1)return 1;//auto_matic�֐������p
	return 0;
}//auto_space_mov�I���




int Move::auto_round(Filter *filter, ImageDraw *draw, int *cnt, int *x, int *y, int cx, int cy){ //�X�y�[�X��],�u���b�N�ړ��֐�,cnt�ړ��֎~�t���O,*x*y�ړ��u���b�N,cx,cy�ړ���

	para *prs = &paras[size];
	int *sp = space;
	int s = prs->bsize;
	int fr = 0;   //��]���� 0:���v 0�ȊO:�����v movp==0(�ړ������{)�̓x�؂�ւ��
	int movp = 0;//��]�����؊��t���O
	int ret = 0;//return 1 ���ʗp

	if (auto_space_mov(filter, draw, cnt, x, y))return 1;

	while (*x != cx || *y != cy){
		if (sp[0] == *x && sp[1] == *y - s){       //�X�y�[�X��
			if (*y - cy > 0){ mov(filter, draw, cnt, 3, 1); continue; }//�ړI�n�����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 1, 1); else movp = mov(filter, draw, cnt, 2, 1); //��]��������,�E,��
		}
		if (sp[0] == *x + s && sp[1] == *y - s){     //�X�y�[�X�E��
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 3, 1); else movp = mov(filter, draw, cnt, 2, 1); //��]��������,��,��
		}
		if (sp[0] == *x + s && sp[1] == *y){       //�X�y�[�X�E
			if (*x - cx < 0){ mov(filter, draw, cnt, 2, 1); continue; }//�ړI�n�E����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 3, 1); else movp = mov(filter, draw, cnt, 4, 1); //��]��������,��,��
		}
		if (sp[0] == *x + s && sp[1] == *y + s){     //�X�y�[�X�E��
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 2, 1); else movp = mov(filter, draw, cnt, 4, 1); //��]��������,��,��
		}
		if (sp[0] == *x && sp[1] == *y + s){       //�X�y�[�X��
			if (*y - cy < 0){ mov(filter, draw, cnt, 4, 1); continue; }//�ړI�n������
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 2, 1); else movp = mov(filter, draw, cnt, 1, 1); //��]��������,��,�E
		}
		if (sp[0] == *x - s && sp[1] == *y + s){     //�X�y�[�X����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 4, 1); else movp = mov(filter, draw, cnt, 1, 1); //��]��������,��,�E
		}
		if (sp[0] == *x - s && sp[1] == *y){       //�X�y�[�X��
			if (*x - cx > 0){ mov(filter, draw, cnt, 1, 1); continue; }//�ړI�n������
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 4, 1); else movp = mov(filter, draw, cnt, 3, 1); //��]��������,��,��
		}
		if (sp[0] == *x - s && sp[1] == *y - s){     //�X�y�[�X����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(filter, draw, cnt, 1, 1); else movp = mov(filter, draw, cnt, 3, 1); //��]��������,�E,��
		}
		if (movp == 2){ ret = 1; break; }

	} //while�I���

	if (ret == 1)return 1;//auto_matic�֐������p
	return 0;

}//auto_round()�I���



void Move::auto_matic(Filter *filter, ImageDraw *draw, int *cnt, int pcs, int range, int j){    //�I�[�g�֐��錾 cnt�ړ��֎~�t���O,pcs�c����,range�Ώ۔͈�,jfor���͈�

	para *prs = &paras[size];
	int *sp = space;
	int i; //for�p
	int ret = 0;//return ���ʗp

	if (range == pcs){ //������
		cnt = (int*)malloc(sizeof(int)*prs->idx);
		for (i = 0; i < prs->idx; i++){
			cnt[i] = 0; //mov()�֐����̈ړ��֎~�t���O
		}
	}

	while (1){//��1�񏈗����[�v

		//��1�񊮐�����
		for (i = j; i < range + j; i++){//��1��`�F�b�N
			if (img[i].cx != img[i].fx || img[i].cy != img[i].fy)break;//�l����v�̊ԌJ��Ԃ�
		}
		if (i == range + j){//�S����v������ړ��֎~����,break
			for (i = j; i < range + j; i++){ cnt[i] = 1; }
			if (range == 2){ mov(filter, draw, cnt, 1, 1); mov(filter, draw, cnt, 1, 1); cnt_results = 0; ret = 1; break; }//��v�̏ꍇ���̏����ŏI��
			break; //��v�����ꍇwhile����
		}

		//���͈͓���1��1��ڏ��� 
		for (i = j + 1; i < range + j; i++){
			if (auto_round(filter, draw, cnt, &img[i].cx, &img[i].cy, img[i - 1].fx, img[i - 1].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;	//�ړ��֎~ON
		} //for�I���
		if (ret == 1)break;//���for���u���[�N�̏ꍇwhile����

		//���ړ��s��ԏ���
		if ((img[j].cx == img[range + j - 1].fx && img[j].cy == img[range + j - 1].fy) ||
			(img[j].cx == img[range + j - 1 + pcs].fx && img[j].cy == img[range + j - 1 + pcs].fy &&
			sp[0] == img[range + j - 1].fx && sp[1] == img[range + j - 1].fy)){
			for (i = j + 1; i < range + j; i++){
				cnt[i] = 0;
			}
			if (auto_round(filter, draw, cnt, &img[j].cx, &img[j].cy, img[range + j - 1 + pcs * 2].fx, img[range + j - 1 + pcs * 2].fy)){
				ret = 1; break;//���~�̏ꍇwhile����
			}
			continue;//�ړ��s��ԏ�����ŏ��ɖ߂�
		}

		//���͈͓���1��1����
		if (auto_round(filter, draw, cnt, &img[j].cx, &img[j].cy, img[j + pcs].fx, img[j + pcs].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;  //�ړ��֎~ON

		//���͈͓���1��2��ڏ���
		for (i = range + j - 1; i >= j + 1; i--){
			cnt[i] = 0;        //�ړ��O�ړ��֎~OFF
			if (auto_round(filter, draw, cnt, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;        //�ړ���ړ��֎~ON
		}//for�I���
		if (ret == 1)break;//���for��break�̏ꍇwhile����

		if (range == 2){ cnt[j] = 0; mov(filter, draw, cnt, 3, 1); mov(filter, draw, cnt, 1, 1); mov(filter, draw, cnt, 1, 1); cnt_results = 0; ret = 1; break; }//����

		//���͈͓���1��1����
		cnt[j] = 0;            //�ړ��O�ړ��֎~OFF
		if (auto_round(filter, draw, cnt, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;            //�ړ���ړ��֎~ON

		break;//while����
	}//��1�񏈗����[�v�I���
	if (ret == 1)return;//while��break�̏ꍇ�֐��I��

	j += pcs;         //for�������l�X�V
	range--;         //�͈͍X�V

	while (1){//��1�񏈗����[�v

		//��1�񊮐�����
		for (i = j; i <= j + pcs*(range - 1); i += pcs){
			if (img[i].cx != img[i].fx || img[i].cy != img[i].fy)break;//�l����v�̊ԌJ��Ԃ�
		}
		if (i > j + pcs*(range - 1)){ //�S����v������ړ��֎~����,break
			for (i = j; i <= j + pcs*(range - 1); i += pcs){ cnt[i] = 1; }
			break;//��v�����ꍇwhile����
		}

		//���͈͓���1��1��ڏ���
		for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
			if (auto_round(filter, draw, cnt, &img[i].cx, &img[i].cy, img[i - pcs].fx, img[i - pcs].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;  //�ړ��֎~ON
		} //for�I���
		if (ret == 1)break;//��for��break�̏ꍇwhile����

		//���ړ��s��ԏ���
		if ((img[j].cx == img[j + pcs*(range - 1)].fx && img[j].cy == img[j + pcs*(range - 1)].fy) ||
			(img[j].cx == img[j + pcs*(range - 1) + 1].fx && img[j].cy == img[j + pcs*(range - 1) + 1].fy &&
			sp[0] == img[j + pcs*(range - 1)].fx && sp[1] == img[j + pcs*(range - 1)].fy)){
			for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
				cnt[i] = 0;
			}
			if (auto_round(filter, draw, cnt, &img[j].cx, &img[j].cy, img[j + pcs*(range - 1) + 2].fx, img[j + pcs*(range - 1) + 2].fy)){
				ret = 1; break;//���~�̏ꍇwhile����
			}
			continue;//�ړ��s��ԏ�����ŏ��ɖ߂�
		}

		//���͈͓���1���1����
		if (auto_round(filter, draw, cnt, &img[j].cx, &img[j].cy, img[j + 1].fx, img[j + 1].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;         //�ړ��֎~ON

		//���͈͓���1��2��ڏ���
		for (i = j + pcs*(range - 1); i >= j + pcs; i -= pcs){
			cnt[i] = 0;   //�ړ��֎~OFF
			if (auto_round(filter, draw, cnt, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;   //�ړ��֎~ON
		}//for�I���
		if (ret == 1)break;//��for��break�̏ꍇwhile����

		//���͈͓���1���1����
		cnt[j] = 0;  //�ړ��֎~OFF
		if (auto_round(filter, draw, cnt, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;  //�ړ��֎~ON

		break;//while����
	}//��1�񏈗����[�v�I���
	if (ret == 1)return;//while��break�̏ꍇ�֐��I��

	j++; //for�������l�ύX
	//���ċA�Ăяo��  
	auto_matic(filter, draw, cnt, pcs, range, j);

}//auto_matic()�I���