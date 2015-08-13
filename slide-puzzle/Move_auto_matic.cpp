//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@Move�N���X�����\�b�h��`(����)                        **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Menu.h"

int Move::auto_space_mov(Dx9Init *dx, MSG *msg, ImageDraw *draw, int *x, int *y){//�X�y�[�X��ړI�u���b�N�ɗאڂ���܂Ŏ����ړ�,cnt �ړ��֎~�t���O,x,y�ڕW�u���b�N

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
		
		if (sp[0] - *x > 0)f = mov(dx, msg, draw, 2, 1);//�X�y�[�X�E�ʒu�֌W�ʂɈ�����ς�mov�֐����Ăяo��
		if (sp[0] - *x > 0 && f == 0){ mov(dx, msg, draw, 3, 1); continue; }
		if (sp[0] - *x < 0)f = mov(dx, msg, draw, 1, 1);//�X�y�[�X��
		if (sp[1] - *y > 0)f = mov(dx, msg, draw, 4, 1);//�X�y�[�X�� 
		if (sp[1] - *y < 0)f = mov(dx, msg, draw, 3, 1);//�X�y�[�X��
		if (br)return 0;//�A�v���I��
		if (f == 2){ ret = 1; break; }
	}//while�I���
	if (ret == 1)return 1;//auto_matic�֐������p
	return 0;
}//auto_space_mov�I���




int Move::auto_round(Dx9Init *dx, MSG *msg, ImageDraw *draw, int *x, int *y, int cx, int cy){ //�X�y�[�X��],�u���b�N�ړ��֐�,cnt�ړ��֎~�t���O,*x*y�ړ��u���b�N,cx,cy�ړ���

	para *prs = &paras[size];
	int *sp = space;
	int s = prs->bsize;
	int fr = 0;   //��]���� 0:���v 0�ȊO:�����v movp==0(�ړ������{)�̓x�؂�ւ��
	int movp = 0;//��]�����؊��t���O
	int ret = 0;//return 1 ���ʗp

	if (auto_space_mov(dx, msg, draw, x, y))return 1;
	if (br)return 0;//�A�v���I��
	while (*x != cx || *y != cy){
		if (sp[0] == *x && sp[1] == *y - s){       //�X�y�[�X��
			if (*y - cy > 0){ mov(dx, msg, draw, 3, 1); continue; }//�ړI�n�����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 1, 1); else movp = mov(dx, msg, draw, 2, 1); //��]��������,�E,��
		}
		if (sp[0] == *x + s && sp[1] == *y - s){     //�X�y�[�X�E��
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 3, 1); else movp = mov(dx, msg, draw, 2, 1); //��]��������,��,��
		}
		if (sp[0] == *x + s && sp[1] == *y){       //�X�y�[�X�E
			if (*x - cx < 0){ mov(dx, msg, draw, 2, 1); continue; }//�ړI�n�E����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 3, 1); else movp = mov(dx, msg, draw, 4, 1); //��]��������,��,��
		}
		if (sp[0] == *x + s && sp[1] == *y + s){     //�X�y�[�X�E��
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 2, 1); else movp = mov(dx, msg, draw, 4, 1); //��]��������,��,��
		}
		if (sp[0] == *x && sp[1] == *y + s){       //�X�y�[�X��
			if (*y - cy < 0){ mov(dx, msg, draw, 4, 1); continue; }//�ړI�n������
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 2, 1); else movp = mov(dx, msg, draw, 1, 1); //��]��������,��,�E
		}
		if (sp[0] == *x - s && sp[1] == *y + s){     //�X�y�[�X����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 4, 1); else movp = mov(dx, msg, draw, 1, 1); //��]��������,��,�E
		}
		if (sp[0] == *x - s && sp[1] == *y){       //�X�y�[�X��
			if (*x - cx > 0){ mov(dx, msg, draw, 1, 1); continue; }//�ړI�n������
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 4, 1); else movp = mov(dx, msg, draw, 3, 1); //��]��������,��,��
		}
		if (sp[0] == *x - s && sp[1] == *y - s){     //�X�y�[�X����
			if (movp == 0)fr = !fr;  //�ړ������{��,��]�����؊�
			if (fr == 0)movp = mov(dx, msg, draw, 1, 1); else movp = mov(dx, msg, draw, 3, 1); //��]��������,�E,��
		}
		if (br)return 0;//�A�v���I��
		if (movp == 2){ ret = 1; break; }

	} //while�I���

	if (ret == 1)return 1;//auto_matic�֐������p
	return 0;

}//auto_round()�I���


int Move::auto_matic(Dx9Init *dx, MSG *msg, ImageDraw *draw, int range, int j){    //�I�[�g�֐��錾 cnt�ړ��֎~�t���O,pcs�c����,range�Ώ۔͈�,jfor���͈�

	para *prs = &paras[size];
	int pcs = paras[size].pcs;
	int *sp = space;
	int i; //for�p
	int ret = 0;//return ���ʗp

	if (range == NULL){ //������
		range = pcs;
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
			if (range == 2){ mov(dx, msg, draw, 1, 1); mov(dx, msg, draw, 1, 1); cnt_results = 0; ret = 1; break; }//��v�̏ꍇ���̏����ŏI��
			break; //��v�����ꍇwhile����
		}
		if (br)return -1;//�A�v���I��
		//���͈͓���1��1��ڏ��� 
		for (i = j + 1; i < range + j; i++){
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i - 1].fx, img[i - 1].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;	//�ړ��֎~ON
			if (br)return -1;//�A�v���I��
		} //for�I���
		if (ret == 1)break;//���for���u���[�N�̏ꍇwhile����

		//���ړ��s��ԏ���
		if ((img[j].cx == img[range + j - 1].fx && img[j].cy == img[range + j - 1].fy) ||
			(img[j].cx == img[range + j - 1 + pcs].fx && img[j].cy == img[range + j - 1 + pcs].fy &&
			sp[0] == img[range + j - 1].fx && sp[1] == img[range + j - 1].fy)){
			for (i = j + 1; i < range + j; i++){
				cnt[i] = 0;
			}
			if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[range + j - 1 + pcs * 2].fx, img[range + j - 1 + pcs * 2].fy)){
				ret = 1; break;//���~�̏ꍇwhile����
			}
			if (br)return -1;//�A�v���I��
			continue;//�ړ��s��ԏ�����ŏ��ɖ߂�
		}

		//���͈͓���1��1����
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j + pcs].fx, img[j + pcs].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;  //�ړ��֎~ON
		if (br)return -1;//�A�v���I��
		//���͈͓���1��2��ڏ���
		for (i = range + j - 1; i >= j + 1; i--){
			cnt[i] = 0;        //�ړ��O�ړ��֎~OFF
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;        //�ړ���ړ��֎~ON
			if (br)return -1;//�A�v���I��
		}//for�I���
		if (ret == 1)break;//���for��break�̏ꍇwhile����

		if (range == 2){ cnt[j] = 0; mov(dx, msg, draw, 3, 1); mov(dx, msg, draw, 1, 1); mov(dx, msg, draw, 1, 1); cnt_results = 0; ret = 1; break; }//����

		//���͈͓���1��1����
		cnt[j] = 0;            //�ړ��O�ړ��֎~OFF
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;            //�ړ���ړ��֎~ON
		if (br)return -1;//�A�v���I��
		break;//while����
	}//��1�񏈗����[�v�I���
	if (ret == 1)return 0;//while��break�̏ꍇ�֐��I��

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
		if (br)return -1;//�A�v���I��
		//���͈͓���1��1��ڏ���
		for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i - pcs].fx, img[i - pcs].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;  //�ړ��֎~ON
			if (br)return -1;//�A�v���I��
		} //for�I���
		if (ret == 1)break;//��for��break�̏ꍇwhile����

		//���ړ��s��ԏ���
		if ((img[j].cx == img[j + pcs*(range - 1)].fx && img[j].cy == img[j + pcs*(range - 1)].fy) ||
			(img[j].cx == img[j + pcs*(range - 1) + 1].fx && img[j].cy == img[j + pcs*(range - 1) + 1].fy &&
			sp[0] == img[j + pcs*(range - 1)].fx && sp[1] == img[j + pcs*(range - 1)].fy)){
			for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
				cnt[i] = 0;
			}
			if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j + pcs*(range - 1) + 2].fx, img[j + pcs*(range - 1) + 2].fy)){
				ret = 1; break;//���~�̏ꍇwhile����
			}
			if (br)return -1;//�A�v���I��
			continue;//�ړ��s��ԏ�����ŏ��ɖ߂�
		}

		//���͈͓���1���1����
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j + 1].fx, img[j + 1].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;         //�ړ��֎~ON
		if (br)return -1;//�A�v���I��
		//���͈͓���1��2��ڏ���
		for (i = j + pcs*(range - 1); i >= j + pcs; i -= pcs){
			cnt[i] = 0;   //�ړ��֎~OFF
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//���~�̏ꍇfor����
			cnt[i] = 1;   //�ړ��֎~ON
			if (br)return -1;//�A�v���I��
		}//for�I���
		if (ret == 1)break;//��for��break�̏ꍇwhile����

		//���͈͓���1���1����
		cnt[j] = 0;  //�ړ��֎~OFF
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//���~�̏ꍇwhile����
		cnt[j] = 1;  //�ړ��֎~ON
		if (br)return -1;//�A�v���I��
		break;//while����
	}//��1�񏈗����[�v�I���
	if (br)return -1;//�A�v���I��
	if (ret == 1)return 0;//while��break�̏ꍇ�֐��I��

	j++; //for�������l�ύX
	//���ċA�Ăяo��  
	auto_matic(dx, msg, draw, range, j);
	return 0;
}//auto_matic()�I���

int Move::count(Dx9Init *dx, MSG *msg, ImageDraw *draw){//�萔�v�Z

	Menu menu;//�ꎞ�I�Ɏg��
	int i;
	int sm[2];//�萔�v�Z�����O�X�y�[�X���W�ޔ�p
	int msgf = 0;   //�A�v���I�����[�v�����t���O

	tkf = 1; tkc = 0;//�萔�v�Z���s�t���O,�J�E���g������
	for (i = 0; i < paras[size].idx; i++){
		img[i].chx = img[i].cx;//�萔�v�Z�O���W�l�ޔ�
		img[i].chy = img[i].cy;//�萔�v�Z�O���W�l�ޔ�
	}
	sm[0] = space[0];//�萔�v�Z�O���W�l�ޔ�
	sm[1] = space[1];//�萔�v�Z�O���W�l�ޔ�
	dx->text("�摜���N���b�N�ŏI��", 305, 522);//�e�L�X�g�`��
	dx->drawscreen();//�`��
	if (auto_matic(dx, msg, draw, NULL, 0) == -1)return -1;//auto_matic�֐�����

	for (i = 0; i < paras[size].idx; i++){
		img[i].cx = img[i].chx;//�萔�v�Z����W�l�߂�
		img[i].cy = img[i].chy;//�萔�v�Z����W�l�߂�
	}
	space[0] = sm[0];//�萔�v�Z����W�l�߂�
	space[1] = sm[1];//�萔�v�Z����W�l�߂�
	if (cnt_results != -1){//-1���ƒ��~���Ă�
		char str[30];
		sprintf(str, "%d��Ŋ������܂�", tkc);
		dx->text(str, 305, 502);//�e�L�X�g�`��
		dx->text("�摜���N���b�N�ŏI��", 305, 522);//�e�L�X�g�`��
		dx->drawscreen();//�`��

		while (menu.mouse(dx, draw, 5, 0) != 1){//�N���b�N�҂�
			if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg->message == WM_QUIT) {	// PostQuitMessage()���Ă΂ꂽ
					msgf = 1;
					break;	//���[�v�̏I��
				}
				else {
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(msg);
					DispatchMessage(msg);
				}
			}
		}
		if (msgf == 1)return -1;
	}//if�I���
	tkf = 0; tkc = 0;//�t���O,�J�E���g������
	return 0;
}