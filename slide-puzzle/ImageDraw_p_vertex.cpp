//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageDraw�N���X�����\�b�h��`(�p�Y���摜���_)               **//
//**                                    (ImageRead�̔h���N���X)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <math.h>
#include "back_image.h"
#include "Move.h"
#include "ImageRead.h"
#include "ImageDraw.h" 

HRESULT ImageDraw::p_vertex(Dx9Init *dx, Move *move, int x, int y, int z){  //���_����

	if (pMyVB == NULL)dx->getVBarray(this, d3);

	int i, j, k, k1, k2;//for
	int r, g, b;       //�F
	typedef struct{//�q�X�g�O�����쐬�p
		int rp;   //�Ԃ̐�
		int gp;  //�΂̐�
		int bp; //�̐�
	}his;
	static his hist[128]; //�q�X�g�O�����e�F�̊e�K����(2�K������1��)
	int xx, yy, zz;      //�����O,������̃p�Y���摜�I�t�Z�b�g
	static int ff = 0;  //�������  0:����  1:������
	int ffr = 1;       //������ԗ��� 0:�X�V���� 1:�X�V����
	DWORD cl;         //1�s�N�Z���F�ꎞ�ۊ�
	float i2, j2, z2;//1�s�N�Z�����W�ꎞ�ۊ�
	float pz;       //1�s�N�Z����Z���W
	int bf;        //�c�o�b�t�@����

	//�X�N���[���N���A 
	if (FAILED(dx->sclear()))return E_FAIL;

	//�J�����ʒu�X�V
	cpx = xrs * 0.5f + sin(theta_lr * 3.14f / 180.0f) * 750.0f;
	cpz = -cos(theta_lr * 3.14f / 180.0f) * 750.0f;

	// �J�����̈ʒu�ƕ�����ݒ�
	dx->cameraset(xrs, yrs, cpx, cpz);

	//�q�X�g�O�����f�[�^����
	for (i = 0; i < 128; i++){  //������
		hist[i].rp = 0;
		hist[i].gp = 0;
		hist[i].bp = 0;
	}
	for (j = 0; j < yrs; j++){//�e�F,�e�K�����Ɍ�����
		for (i = 0; i < xrs; i++){
			r = imgpi[j][i] >> 16 & 0xff;
			g = imgpi[j][i] >> 8 & 0xff;
			b = imgpi[j][i] & 0xff;
			hist[r / 2].rp++;
			hist[g / 2].gp++;
			hist[b / 2].bp++;
		}
	}
	//�q�X�g�O�����f�[�^�����I��

	/********************************************************************************************************************************/
	/********************************************�ŏI�摜�`�揈���J�n****************************************************************/
	/********************************************************************************************************************************/

	Move::para *prs = &move->paras[move->size];
	Move::imxy *img = move->img;
	int df;
	if (d3 != 1)df = 8; else df = 4;
	//��������s���鏈��(�t�@�C���֐��ȊO)
	back_image(dx, 1);     //�w�i�`��֐�,����1==�摜�`��
	k1 = 0;

	//�p�Y�����ȊO�̉摜�`�捶��
	int yr = (int)((yrs - 400) * 0.5);
	int xr = (int)((xrs - 400) * 0.5);
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = 0; j < 400; j++){
			for (i = 0; i < xr; i++){
				//�G�b�W���o��
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z�����F���ɂ��
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//�s�N�Z���Ԍ��Ԗ���
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = 0; j < 400; j++){
			for (i = 0; i < xr; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//�p�Y�����ȊO�̉摜���_�E��
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = 0; j < 400; j++){
			for (i = xrs - xr; i < xrs; i++){
				//�G�b�W���o��
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z�����F���ɂ��
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//�s�N�Z���Ԍ��Ԗ���
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = 0; j < 400; j++){
			for (i = xrs - xr; i < xrs; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//�p�Y�����ȊO�̉摜���_�㑤
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = 0; j < yr; j++){
			for (i = 0; i < 400; i++){
				//�G�b�W���o��
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z�����F���ɂ��
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//�s�N�Z���Ԍ��Ԗ���
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = 0; j < yr; j++){
			for (i = 0; i < 400; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//�p�Y�����ȊO�̉摜���_����
	if (ver == 0 || d.gfl == 3 || d3 == 3){
		for (j = yrs - yr; j < yrs; j++){
			for (i = 0; i < 400; i++){
				//�G�b�W���o��
				if (d.gfl == 3 && imgpi[j][i] == 0){
					for (int k2 = 0; k2 < 4; k2++){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				cl = imgpi[j][i];
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z�����F���ɂ��
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				//�s�N�Z���Ԍ��Ԗ���
				img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j + 0.5f, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f, (float)j, -pz - 15.0f);
					img_a[k1++].color = cl;
				}
			}
		}
	}
	else{
		for (j = yrs - yr; j < yrs; j++){
			for (i = 0; i < 400; i++){
				cl = imgpi[j][i];

				for (k2 = 0; k2 < df; k2++){
					img_a[k1++].color = cl;
				}
			}
		}
	}

	//�p�Y���Ԃ̉摜���_
	if (move->size != 5){//size5�͏������Ȃ�
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				//�G�b�W���o��
				if (d.gfl == 3 && imgpi[j + yr][i + xr] == 0){
					img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					img_a[k1++].color = 0;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
					}
					continue;
				}

				if (block[j][i] == 150 || block[j][i] == 1){//�t�B���^�[�l150,1�̂ݕ`��
					if (d3 == 1 || d3 == 2)pz = 20.0f;
					if (d3 == 3)pz = ((imgpi[j + yr][i + xr] >> 16 & 0xff) +
						(imgpi[j + yr][i + xr] >> 8 & 0xff) +
						(imgpi[j + yr][i + xr] & 0xff)) / 9.0f;//Z�����F���ɂ��
					img_a[k1].p = D3DXVECTOR3((float)i + xr, (float)j + yr, pz);
					img_a[k1++].color = imgpi[j + yr][i + xr];
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i + xr, (float)j + yr, -pz - 15.0f);
						img_a[k1++].color = imgpi[j + yr][i + xr];
					}
				}
			}
		}
	}

	//�p�Y���������菈��,�����O������̃I�t�Z�b�g����
	for (i = 0; i < prs->idx; i++){
		if (img[i].cx != img[i].fx || img[i].cy != img[i].fy || img[i].cz != 0)break;
	}
	if (i == prs->idx && ff == 0)ffr = 0;               //����,�X�V����
	if (i == prs->idx && ff == 1){ ffr = 1; ff = 0; }  //����,�X�V�L��
	if (i != prs->idx && ff == 1)ffr = 0;             //������,�X�V����
	if (i != prs->idx && ff == 0){ ffr = 1; ff = 1; }//������,�X�V�L��

	//�u���b�N�摜����
	if (ff == 1 && move->size < 5){
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (block[j][i] == 0)continue;   //�F�ϊ������X�L�b�v
				//���F�擾�ϊ�
				r = imgpi[j + yr][i + xr] >> 16 & 0xff;
				g = imgpi[j + yr][i + xr] >> 8 & 0xff;
				b = imgpi[j + yr][i + xr] & 0xff;
				r -= block[j][i];
				g -= block[j][i];
				b -= block[j][i];
				if (r <= 0)r = 1;
				if (g <= 0)g = 1;
				if (b <= 0)b = 1;
				imgpi[j + yr][i + xr] = (r << 16) + (g << 8) + b;//�F���
			}
		}
	}//�u���b�N�摜�����I���

	if (ff == 0){ xx = 0; yy = 0, zz = 0; }//������ԂŃI�t�Z�b�g�ʍX�V
	else{ xx = 19; yy = 19, zz = -57; }

	//�p�Y�����摜���_�����J�n
	for (k = 0; k < prs->idx; k++){
		for (j = 0; j < prs->bsize; j++){     //1�u���b�N��y����
			for (i = 0; i < prs->bsize; i++){//1�u���b�N��x����

				//�G�b�W���o����
				if (d.gfl == 3 && imgpi[j + img[k].fy + yr]
					[i + img[k].fx + xr] == 0){
					img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					img_a[k1++].color = 0;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
					}

					//�s�N�Z���Ԍ��Ԗ���
					if (i + 1 == prs->bsize && j + 1 != prs->bsize){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}

					if (i + 1 != prs->bsize && j + 1 != prs->bsize){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}

					if (i + 1 != prs->bsize && j + 1 == prs->bsize){
						img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						img_a[k1++].color = 0;
						if (d3 != 1){
							img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							img_a[k1++].color = 0;
						}
					}
					continue;
				}

				//�u���b�N�Ԃ͕`�悵�Ȃ�(�t�B���^�[�l150,1 size==5)
				if ((block[j + img[k].fy][i + img[k].fx] == 150 ||
					block[j + img[k].fy][i + img[k].fx] == 1) &&
					move->size != 5)continue;

				//�ʏ�\��,�\�����W�ʒu�{�I�t�Z�b�g, �\���F���(�������W�ʒu�|�I�t�Z�b�g)
				cl = imgpi[j + img[k].fy + yr][i + img[k].fx + xr];

				j2 = (float)j + img[k].cy + yr + yy + y;
				i2 = (float)i + img[k].cx + xr + xx + x;
				z2 = (float)img[k].cz + zz + z;

				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j + img[k].fy + yr][i + img[k].fx + xr] >> 16 & 0xff) +
					(imgpi[j + img[k].fy + yr][i + img[k].fx + xr] >> 8 & 0xff) +
					(imgpi[j + img[k].fy + yr][i + img[k].fx + xr] & 0xff)) / 9.0f;//Z�����F���ɂ��

				img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2, pz + z2);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2, -pz - 15.0f + z2);
					img_a[k1++].color = cl;
				}

				//�s�N�Z���Ԍ��Ԗ���
				if (i + 1 == prs->bsize && j + 1 != prs->bsize){
					img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2 + 0.5f, pz + z2);
					img_a[k1++].color = cl;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i2, (float)j2 + 0.5f, -pz - 15.0f + z2);
						img_a[k1++].color = cl;
					}
				}

				if (i + 1 != prs->bsize && j + 1 != prs->bsize){
					img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2 + 0.5f, pz + z2);
					img_a[k1++].color = cl;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2 + 0.5f, -pz - 15.0f + z2);
						img_a[k1++].color = cl;
					}
				}

				if (i + 1 != prs->bsize && j + 1 == prs->bsize){
					img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2, pz + z2);
					img_a[k1++].color = cl;
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i2 + 0.5f, (float)j2, -pz - 15.0f + z2);
						img_a[k1++].color = cl;
					}
				}
			}
		}
	}//�p�Y�����摜���_�����I���

	//�c���_�f�[�^����(���ύX�̍ێg�p���_�f�[�^���ɍ��ق��o��̂Ŏc�肪�\��������(�L-��-`))
	if (d3 == 1)bf = yrs * xrs * 4;//���_���Z�o
	else bf = yrs * xrs * 8;
	do{
		img_a[k1].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�c���_�f�[�^0����
		img_a[k1++].color = 0;
	} while (k1 < bf);

	dx->d3primitive(this, d3);//3D�`��

	//����,���������㏈��(�ċA��1�x�������������)
	if (ffr == 1){
		if (ff == 0){//�������㏈��
			for (i = 0; i < 19; i++){
				draw_img(dx, move, 19 - i, 19 - i, -57 + i * 3);//�ċA
				dx->drawscreen();//�`��
			}
		}
		if (ff == 1){//���������㏈��
			for (i = 0; i < 19; i++){
				draw_img(dx, move, i - 19, i - 19, 57 - i * 3);//�ċA(19��xx,yy�̕��̕␳)
				dx->drawscreen();//�`��
			}
		}
	}//����,���������㏈���I���

	//�q�X�g�O�������_
	pLine->Begin();
	for (i = 0; i < 128; i++){//�q�X�g�O�������_

		hi[0] = D3DXVECTOR2(640.0f + i, 200.0f - hist[i].rp / 80.0f);
		hi[1] = D3DXVECTOR2(640.0f + i, 200.0f);
		pLine->Draw(hi, 2, D3DCOLOR_ARGB(255, i * 2, 0, 0));

		hi[0] = D3DXVECTOR2(640.0f + i, 300.0f - hist[i].gp / 80.0f);
		hi[1] = D3DXVECTOR2(640.0f + i, 300.0f);
		pLine->Draw(hi, 2, D3DCOLOR_ARGB(255, 0, i * 2, 0));

		hi[0] = D3DXVECTOR2(640.0f + i, 400.0f - hist[i].bp / 80.0f);
		hi[1] = D3DXVECTOR2(640.0f + i, 400.0f);
		pLine->Draw(hi, 2, D3DCOLOR_ARGB(255, 0, 0, i * 2));
	}
	pLine->End();
	//�q�X�g�O�������_�I��

	//������摜���_
	if (finish == true){
		k1 = 0;
		fimg = (Dx9Init::MY_VERTEX2*)malloc(sizeof(Dx9Init::MY_VERTEX2) * 200 * 200);
		for (j = 0; j < 200; j++){
			for (i = 0; i < 200; i++){
				if (d.gfl == 3 && imgpi[yr + j * 2][xr + i * 2] == 0)continue;
				fimg[k1].x = 590.0f + i;
				fimg[k1].y = 360.0f + (199.0f - j);
				fimg[k1].z = 0.0f;
				fimg[k1].rhw = 0.0f;
				fimg[k1].color = imgpi[yr + j * 2][xr + i * 2];
				fimg[k1].tu = 0.0f;
				fimg[k1++].tv = 0.0f;
			}
		}
		dx->d2primitive(fimg);//2D�`��
		free(fimg);
	}
	ver = 1;//�����`�抮��
	/********************************************************************************************************************************/
	/********************************************�ŏI�摜�`�揈���I��****************************************************************/
	/********************************************************************************************************************************/
	return S_OK;
}

