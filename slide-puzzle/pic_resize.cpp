//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�摜���T�C�Y����                                    �@**//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "struct_set.h"

void pic_resize(alldata *p, int g2){//g2:���摜�\�t�g�C���[�W

	int j, i;            //for
	int r, g, b, a;     //�F���
	int xs, ys;             //���摜�T�C�Y
	static int xrsc, yrsc; //�������m�ۃ`�F�b�N
		
	GetSoftImageSize(g2, &xs, &ys);          //�ϊ����摜�T�C�Y�擾
	
		if (xs == ys){//���摜��xy�������T�C�Y
			xrsc = yrsc = 400;
		}

		if (xs > ys){//���摜��x�̕����T�C�Y�傫��(����������400�Œ�)
			xrsc = (double)xs / ys * 400;
			yrsc = 400;
		}

		if (xs < ys){ //���摜��y�̕����T�C�Y�傫��(����������400�Œ�)
			xrsc = 400;
			yrsc = (double)ys / xs * 400;
		}
		if (p->xrs != xrsc || p->yrs != yrsc){  //�ǂ��炩���U�̏ꍇ�摜�ύX���ꂽ�Ɣ��f,���������,�������Ċm��
			if (p->mc == 1){                   //mc==0�̏ꍇ�̓������m�ۂ��ĂȂ��׉���������Ȃ�(�A�N�Z�X�ᔽ�h�~)
				                              //���X�V�O�摜�T�C�Y���̉���Ȃ̂�xrsc,yrsc��p->xrs,yrs�X�V�O�ɏ���
				for (i = 0; i < p->yrs; i++){ free(p->imgpi[i]); free(p->imcpy[i]); }
				free(p->imgpi); free(p->imcpy);
			}
			p->xrs = xrsc; //�\���̂ɃR�s�[
			p->yrs = yrsc;//�\���̂ɃR�s�[
			p->imgpi = (int**)malloc(sizeof(int*) * p->xrs);//�X�V��摜�T�C�Y���̊m�ۂȂ̂�xrsc,yrsc��p->xrs,yrs�X�V��ɏ���
			p->imcpy = (int**)malloc(sizeof(int*) * p->xrs);
			for (i = 0; i < p->xrs; i++){ p->imgpi[i] = (int*)malloc(sizeof(int) * p->yrs); p->imcpy[i] = (int*)malloc(sizeof(int) * p->yrs); }
			p->mc = 1;//�m�ۃt���OON
		}//�������m�ۉ�������I���

	for (j = 0; j < p->yrs; j++){
		for (i = 0; i < p->xrs; i++){
			GetPixelSoftImage(g2, i*xs / p->xrs, j*ys / p->yrs, &r, &g, &b, &a);//���摜����F���擾
			p->imcpy[i][j] = p->imgpi[i][j] = (r << 16) + (g << 8) + b;//�摜�F���i�[�ϐ��ɐF���i�[
		}
	}
	InitSoftImage();//�\�t�g�C���[�W�S�J��
}
	
