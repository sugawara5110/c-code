//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageDraw�N���X�����\�b�h��`(�摜�`��)                 �@�@**//
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
#include "Filter.h"

// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g�̐ݒ�
#define MY_VERTEX_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE )
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

HRESULT ImageDraw::draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z){//�摜�ϊ�,�`�揈��  drawing_img������Ăяo�����(�I�[�o�[���C�h)

	int i, j, k, k1, i1, j1;//for
	int r, g, b;        //�F
	int **pi16;        //�G�b�W,�G���{�X�����ꎞ�ۊǗp
	int mono;         //���m�N���p
	int pi;          //�G�b�W���o�p
	double ghs, gvs;//�G�b�W���o�p
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

	// �r���[�|�[�g�Ɛ[�x�o�b�t�@�̃N���A�ƃX�e���V���o�b�t�@�̍폜
	if (FAILED(dx->pD3DDevice->Clear(0, NULL,   // �N���A����̈�͑S��
		D3DCLEAR_TARGET |			// �o�b�N�o�b�t�@���w��
		D3DCLEAR_ZBUFFER, 		   // �[�x�o�b�t�@�iZ�o�b�t�@�j���w��
		D3DCOLOR_XRGB(0, 0, 255), // �N���A����F
		1.0f, 					 // �N���A����[�x�o�b�t�@�iZ�o�b�t�@�j�̒l
		0)))				    // �N���A����X�e���V���o�b�t�@�̒l
	{
		return E_FAIL;
	}

	//�J�����ʒu�X�V
	cpx = xrs * 0.5f + sin(theta_lr * 3.14f / 180.0f) * 750.0f;
	cpz = -cos(theta_lr * 3.14f / 180.0f) * 750.0f;

	// �J�����̈ʒu�ƕ�����ݒ�
	D3DXMatrixLookAtLH(&m_view,
		&D3DXVECTOR3(cpx, yrs * 0.5f, cpz),          //�J�����̈ʒu
		&D3DXVECTOR3(xrs * 0.5f, yrs * 0.5f, 0.0f),//�J�����̕�����������_
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));          //�J�����̏�̕���

	if (d3 == 1)img_a = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * yrs * xrs * 4);//�p�Y���摜���_�z��m��(�ʏ�)
	else img_a = (MY_VERTEX*)malloc(sizeof(MY_VERTEX) * yrs * xrs * 8);      //�p�Y���摜���_�z��m��(Z��������*2)
	if (move == NULL) {//NULL�̏ꍇ�摜���������`��
		back_image(dx, 1);
		k1 = 0;
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z�����F���ɂ��
				img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
				//�s�N�Z���Ԍ��Ԗ���
				img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + x, (float)j + 0.5f, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j + 0.5f, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j + 0.5f, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
				img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j, pz);
				img_a[k1++].color = cl;
				if (d3 != 1){
					img_a[k1].p = D3DXVECTOR3((float)i + 0.5f + x, (float)j, -pz - 30.0f);
					img_a[k1++].color = cl;
				}
			}
		}
		pMyVB->Lock(0, 0, (void**)&img_p, 0);
		if (d3 == 1)memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 4);
		else memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 8);
		pMyVB->Unlock();
		dx->pD3DDevice->BeginScene();// �`��J�n�錾
		// ���_�o�b�t�@�[�������_�����O�p�C�v���C���ɐݒ�
		dx->pD3DDevice->SetStreamSource(0, pMyVB, 0, sizeof(MY_VERTEX));

		// �}�g���b�N�X�������_�����O�p�C�v���C���ɐݒ�
		dx->pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

		//�e�N�X�`���[
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF);

		// �`��
		if (d3 == 1)dx->pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, yrs * xrs * 4);
		else dx->pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, yrs * xrs * 8);
		dx->pD3DDevice->EndScene();// �`��I���錾
		free(img_a);     //���_�z����
		return S_OK;
	}//�t�@�C���֐����s���̂ݏ����I��

	/********************************************************************************************************************************/
	/********************************************�e��摜�����J�n********************************************************************/
	/********************************************************************************************************************************/

	//���m�N���摜����,�G�b�W���o,�G���{�X,�摜�G���{�X����,�G�敗����(�Î~��,����,�ǂ���ł��g�p����)gfr�̒l��puzzle_main���瑀�삵�Ă���
	if (d.gfr == 1 || d.gfr == 3 || d.gfr == 4 || d.gfr == 5 || d.gfr == 8 || d.gfr == 9) {
		pi16 = (int**)malloc(sizeof(int*) * yrs);//�G�b�W,�G���{�X,�摜�G���{�X�����ꎞ�ۊǗp�������m��
		for (i = 0; i < yrs; i++){ pi16[i] = (int*)malloc(sizeof(int) * xrs); }

		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){

				if (d.gfr == 1){//���m�N���摜����
					r = imgpi[j][i] >> 16 & 0xff;//0x��16�i�� 0xff�ŉ�8�r�b�g &�_����
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					mono = (r + g + b) / 3;//(�ԁ{�΁{��)��3
					imcpy[j][i] = imgpi[j][i] = (mono << 16) + (mono << 8) + mono;//�F���𕽋ω�����ƃ��m�N���ɂȂ�
				}//���m�N�������I���

				if (d.gfr == 5){//�G�敗����
					r = (imgpi[j][i] >> 16 & 0xff) / 60 * 60;
					g = (imgpi[j][i] >> 8 & 0xff) / 60 * 60;
					b = (imgpi[j][i] & 0xff) / 60 * 60;
					imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfr == 8){//�l�K�|�W����
					r = 255 - (imgpi[j][i] >> 16 & 0xff);
					g = 255 - (imgpi[j][i] >> 8 & 0xff);
					b = 255 - (imgpi[j][i] & 0xff);
					imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;
				}

				if (d.gfr == 3 || d.gfr == 4 || d.gfr == 9){//�G�b�W���o,�G���{�X,�摜�G���{�X�O����
					r = imgpi[j][i] >> 16 & 0xff;
					g = imgpi[j][i] >> 8 & 0xff;
					b = imgpi[j][i] & 0xff;
					pi16[j][i] = (r + g + b) / 3;
				}//�G�b�W���o,�G���{�X�O�����I���

			}   //for�I���
		}      //for�I���

		if (d.gfr == 3 || d.gfr == 4 || d.gfr == 9) {//�G�b�W���o,�G���{�X,�摜�G���{�X�㏈��

			if (d.gfr == 9){    //�摜�G���{�X�p�摜�X�V,���ʒ���
				read->drawing_img(dx, NULL, NULL, 0, 0, 0, 0); //�摜�X�V
				if (read->d.mcf == 1)read->pBasicAudio->put_Volume(-2000);//���ʒ���,����ȊO���Ƃ܂��O���t������
			}

			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					if (i == 0 || i == xrs - 1 || j == 0 || j == yrs - 1){//�z��͈͊O�܂œǂ�ł��܂��̂ŊO���͌v�Z�Ɋ܂܂Ȃ��悤�ɂ���
						imgpi[j][i] = (1 << 16) + (1 << 8) + 1; continue;
					}
					//��������ghs 
					ghs = pi16[j - 1][i - 1] * -1;     //����,�~-1���W��,�v�Z���ĂȂ��s�N�Z���͌W��0
					ghs += pi16[j][i - 1] * -2;       //��
					ghs += pi16[j + 1][i - 1] * -1;  //����
					ghs += pi16[j - 1][i + 1] * 1;  //�E��
					ghs += pi16[j][i + 1] * 2;     //�E
					ghs += pi16[j + 1][i + 1] * 1;//�E��
					//��������gvs
					gvs = pi16[j - 1][i - 1] * -1;     //����
					gvs += pi16[j - 1][i] * -2;       //��
					gvs += pi16[j - 1][i + 1] * -1;  //�E��
					gvs += pi16[j + 1][i - 1] * 1;  //����
					gvs += pi16[j + 1][i] * 2;     //��
					gvs += pi16[j + 1][i + 1] * 1;//�E��

					if (d.gfr == 3){                  //�G�b�W���o�v�Z
						pi = (int)(sqrt(ghs*ghs + gvs*gvs));//sqrt������(pow�֐��ł����肾���������܂������Ȃ�)
						if (pi < 50)pi = 0;         //�w�i�F(��)��0�ɂ���
						if (pi > 255)pi = 255;
						imcpy[j][i] = imgpi[j][i] = (pi << 16) + (pi << 8) + pi;
					}

					if (d.gfr == 4 || d.gfr == 9){  //�G���{�X,�摜�G���{�X�v�Z
						pi = (int)((ghs + gvs) * 0.5);//�Ƃ肠�������̌v�Z���@�ɂ���ƃG���{�X�ɂȂ�݂���
						pi += 128;          //�D�F���ۂ�������������炵��������
						if (pi < 1)pi = 1;
						if (pi > 255)pi = 255;
						imcpy[j][i] = imgpi[j][i] = (pi << 16) + (pi << 8) + pi;

						if (d.gfr == 9){//�摜�G���{�X
							pi -= 128;
							if (xrs == read->xrs && yrs == read->yrs){//�T�C�Y����
								r = (read->imgpi[j][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i] & 0xff) - pi;
							}

							if (xrs > read->xrs && yrs == read->yrs){//���摜�̕���x�傫��,y����
								if (i <= (xrs - read->xrs) * 0.5 || i >= (xrs - read->xrs) * 0.5 + read->xrs)continue;
								r = (read->imgpi[j][i - (xrs - read->xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i - (xrs - read->xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i - (xrs - read->xrs) / 2] & 0xff) - pi;
							}

							if (xrs < read->xrs && yrs == read->yrs){//���摜�̕���x������,y����
								r = (read->imgpi[j][i + (read->xrs - xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j][i + (read->xrs - xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j][i + (read->xrs - xrs) / 2] & 0xff) - pi;
							}

							if (yrs > read->yrs && xrs == read->xrs){//���摜�̕���y�傫��,x����
								if (j <= (yrs - read->yrs) * 0.5 || j >= (yrs - read->yrs) * 0.5 + read->yrs)continue;
								r = (read->imgpi[j - (yrs - read->yrs) / 2][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j - (yrs - read->yrs) / 2][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j - (yrs - read->yrs) / 2][i] & 0xff) - pi;
							}

							if (yrs < read->yrs && xrs == read->xrs){//���摜�̕���y������,x����
								r = (read->imgpi[j + (read->yrs - yrs) / 2][i] >> 16 & 0xff) - pi;
								g = (read->imgpi[j + (read->yrs - yrs) / 2][i] >> 8 & 0xff) - pi;
								b = (read->imgpi[j + (read->yrs - yrs) / 2][i] & 0xff) - pi;
							}

							if (yrs < read->yrs && xrs > read->xrs){//���摜�̕���y������,���摜�̕���x�傫��
								if (i <= (xrs - read->xrs) * 0.5 || i >= (xrs - read->xrs) * 0.5 + read->xrs)continue;
								r = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j + (read->yrs - yrs) / 2][i - (xrs - read->xrs) / 2] & 0xff) - pi;
							}

							if (yrs > read->yrs && xrs < read->xrs){//���摜�̕���y�傫��,���摜�̕���x������
								if (j <= (yrs - read->yrs) * 0.5 || j >= (yrs - read->yrs) * 0.5 + read->yrs)continue;
								r = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] >> 16 & 0xff) - pi;
								g = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] >> 8 & 0xff) - pi;
								b = (read->imgpi[j - (yrs - read->yrs) / 2][i + (read->xrs - xrs) / 2] & 0xff) - pi;
							}

							if (r < 1)r = 1;
							if (r > 255)r = 255;
							if (g < 1)g = 1;
							if (g > 255)g = 255;
							if (b < 1)b = 1;
							if (b > 255)b = 255;
							imcpy[j][i] = imgpi[j][i] = (r << 16) + (g << 8) + b;//�摜�G���{�X�v�Z�I��
						}
					}

				}//for�I���
			}   //for�I���
		}      //if �G�b�W,�G���{�X�㏈���I���
		for (i = 0; i < yrs; i++)free(pi16[i]);//�G�b�W,�G���{�X�����ꎞ�ۊǗp���������
		free(pi16);
	}//���m�N��,�G�b�W���o,�G���{�X�����I���

	//���U�C�N����(����,�Î~��ǂ���ł��g��)
	//gfr��v��mf==1����ON�Ŏ��s  gfr��v��f==1�Î~��X�V�L��Ŏ��s
	if (d.gfr == 2) {
		for (j = 0; j < yrs - 10; j += 10){
			for (i = 0; i < xrs - 10; i += 10){
				for (j1 = j; j1 < j + 10; j1++){
					for (i1 = i; i1 < i + 10; i1++){
						imcpy[j1][i1] = imgpi[j1][i1] = imgpi[j][i];//10pixel�A���œ��F�`�ʂŃ��U�C�N�ɂȂ�
					}
				}
			}
		}
	}//���U�C�N�����I���

	/********************************************************************************************************************************/
	/********************************************�e��摜�����I��********************************************************************/
	/********************************************************************************************************************************/

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

	//��������s���鏈��(�t�@�C���֐��ȊO)
	back_image(dx, 1);     //�w�i�`��֐�,����1==�摜�`��
	k1 = 0;
	//�p�Y�����ȊO�̉摜�`�捶��
	for (j = 0; j < 400; j++){
		for (i = 0; i < (xrs - 400) * 0.5; i++){
			//if�G�b�W���o���̔w�i�F�`��X�L�b�v�����ȉ����������L��
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//�p�Y�����ȊO�̉摜���_�E��
	for (j = 0; j < 400; j++){
		for (i = xrs - (int)((xrs - 400) * 0.5); i < xrs; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//�p�Y�����ȊO�̉摜���_�㑤
	for (j = 0; j < (yrs - 400) * 0.5; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//�p�Y�����ȊO�̉摜���_����
	for (j = yrs - (int)((yrs - 400) * 0.5); j < yrs; j++){
		for (i = 0; i < 400; i++){
			if (d.gfr == 3 && imgpi[j][i] == 0)continue;
			cl = D3DXCOLOR((float)(imgpi[j][i] >> 16 & 0xff) / 255, (float)(imgpi[j][i] >> 8 & 0xff) / 255, (float)(imgpi[j][i] & 0xff) / 255, 1.0f);
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
	//�p�Y���Ԃ̉摜���_
	if (move->size != 5){//size5�͏������Ȃ�
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (d.gfr == 3 && imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] == 0)continue;
				if (filter->block[j][i] == 150 || filter->block[j][i] == 1){//�t�B���^�[�l150,1�̂ݕ`��
					if (d3 == 1 || d3 == 2)pz = 20.0f;
					if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z�����F���ɂ��
					img_a[k1].p = D3DXVECTOR3((float)i + (xrs - 400) * 0.5f, (float)j + (yrs - 400) * 0.5f, pz);
					img_a[k1++].color = D3DXCOLOR((float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff) / 255,
						(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff) / 255,
						(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff) / 255, 1.0f);
					if (d3 != 1){
						img_a[k1].p = D3DXVECTOR3((float)i + (xrs - 400) * 0.5f, (float)j + (yrs - 400) * 0.5f, -pz - 15.0f);
						img_a[k1++].color = D3DXCOLOR((float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff) / 255,
							(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff) / 255,
							(float)(imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff) / 255, 1.0f);
					}
				}
			}
		}
	}

	//�p�Y���������菈��,�����O������̃I�t�Z�b�g����
	for (i = 0; i < prs->idx; i++){
		if (img[i].cx != img[i].fx || img[i].cy != img[i].fy)break;
	}
	if (i == prs->idx && ff == 0)ffr = 0;               //����,�X�V����
	if (i == prs->idx && ff == 1){ ffr = 1; ff = 0; }  //����,�X�V�L��
	if (i != prs->idx && ff == 1)ffr = 0;             //������,�X�V����
	if (i != prs->idx && ff == 0){ ffr = 1; ff = 1; }//������,�X�V�L��

	//�u���b�N�摜����
	if (ff == 1 && move->size < 5){
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (filter->block[j][i] == 0)continue;   //�F�ϊ������X�L�b�v
				//�����̂܂ܕϊ�����ƐF���ς���Ă����Ă��܂��̂ň�U���ɖ߂�
				imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] = imcpy[j + (yrs - 400) / 2][i + (xrs - 400) / 2];
				//���F�擾�ϊ�
				r = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff;
				g = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff;
				b = imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff;
				r -= filter->block[j][i];
				g -= filter->block[j][i];
				b -= filter->block[j][i];
				if (r <= 0)r = 1;
				if (g <= 0)g = 1;
				if (b <= 0)b = 1;
				imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] = (r << 16) + (g << 8) + b;//�F���
			}
		}
	}//�u���b�N�摜�����I���

	//�����u���b�N����
	if (ff == 0 && d.mcf == 0){    //�����ςݐÎ~�揈��
		for (j = 0; j < yrs; j++){
			for (i = 0; i < xrs; i++){
				if (d.gfr == 3 && imgpi[j][i] == 0)continue;
				imgpi[j][i] = imcpy[j][i];//�u���b�N�摜�����O�f�[�^���R�s�[
			}
		}
	}

	if (ff == 0){ xx = 0; yy = 0, zz = 0; }//������ԂŃI�t�Z�b�g�ʍX�V
	else{ xx = 19; yy = 19, zz = -57; }

	//�p�Y�����摜���_�����J�n
	for (k = 0; k < prs->idx; k++){
		for (j = 0; j < prs->bsize; j++){     //1�u���b�N��y����
			for (i = 0; i < prs->bsize; i++){//1�u���b�N��x����

				//�G�b�W���o����
				if (d.gfr == 3 && imgpi[j + img[k].fy + (yrs - 400) / 2]
					[i + img[k].fx + (xrs - 400) / 2] == 0)continue;

				//�u���b�N�Ԃ͕`�悵�Ȃ�(�t�B���^�[�l150,1 size==5)
				if ((filter->block[j + img[k].fy][i + img[k].fx] == 150 ||
					filter->block[j + img[k].fy][i + img[k].fx] == 1) &&
					move->size != 5)continue;

				//�ʏ�\��,�\�����W�ʒu�{�I�t�Z�b�g, �\���F���(�������W�ʒu�|�I�t�Z�b�g)
				cl = D3DXCOLOR((float)(imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 16 & 0xff) / 255,
					(float)(imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 8 & 0xff) / 255,
					(float)(imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] & 0xff) / 255, 1.0f);

				j2 = (float)j + img[k].cy + (yrs - 400) * 0.5f + yy + y, 0.0f;
				i2 = (float)i + img[k].cx + (xrs - 400) * 0.5f + xx + x;
				z2 = (float)img[k].cz + zz + z;

				if (d3 == 1 || d3 == 2)pz = 20.0f;
				if (d3 == 3)pz = ((imgpi[j][i] >> 16 & 0xff) + (imgpi[j][i] >> 8 & 0xff) + (imgpi[j][i] & 0xff)) / 9.0f;//Z�����F���ɂ��

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
	pMyVB->Lock(0, 0, (void**)&img_p, 0);
	if (d3 == 1)memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 4);
	else memcpy(img_p, img_a, sizeof(MY_VERTEX) * yrs * xrs * 8);
	pMyVB->Unlock();
	dx->pD3DDevice->BeginScene();// �`��J�n�錾
	// ���_�o�b�t�@�[�������_�����O�p�C�v���C���ɐݒ�
	dx->pD3DDevice->SetStreamSource(0, pMyVB, 0, sizeof(MY_VERTEX));

	// �}�g���b�N�X�������_�����O�p�C�v���C���ɐݒ�
	dx->pD3DDevice->SetTransform(D3DTS_VIEW, &m_view);

	//�e�N�X�`���[
	dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);

	// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
	dx->pD3DDevice->SetFVF(MY_VERTEX_FVF);

	// �`��
	if (d3 == 1)dx->pD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0, yrs * xrs * 4);
	else dx->pD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, yrs * xrs * 8);
	dx->pD3DDevice->EndScene();// �`��I���錾
	free(img_a);    //���_�z����

	//����,���������㏈��(�ċA��1�x�������������)
	if (ffr == 1){
		if (ff == 0){//�������㏈��
			for (i = 0; i < 19; i++){
				drawing_img(dx, filter, move, 19 - i, 19 - i, -57 + i * 3, 0);//�ċA
				dx->drawscreen();//�`��
			}
		}
		if (ff == 1){//���������㏈��
			for (i = 0; i < 19; i++){
				drawing_img(dx, filter, move, i - 19, i - 19, 57 - i * 3, 0);//�ċA(19��xx,yy�̕��̕␳)
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
	if (finish == 1){
		k1 = 0;
		fimg = (MY_VERTEX2*)malloc(sizeof(MY_VERTEX2) * 200 * 200);
		for (j = 0; j < 200; j++){
			for (i = 0; i < 200; i++){
				if (d.gfr == 3 && imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2] == 0)continue;
				fimg[k1].x = 590.0f + i;
				fimg[k1].y = 360.0f + (199.0f - j);
				fimg[k1].z = 0.0f;
				fimg[k1].rhw = 0.0f;
				fimg[k1].color = imgpi[(yrs - 400) / 2 + j * 2][(xrs - 400) / 2 + i * 2];
				fimg[k1].tu = 0.0f;
				fimg[k1++].tv = 0.0f;
			}
		}
		dx->pD3DDevice->BeginScene();// �`��J�n�錾
		// ���_�f�[�^�̂e�u�e�t�H�[�}�b�g��ݒ�
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// �`��
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST, 200 * 200, fimg, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();// �`��I���錾
		free(fimg);
	}

	/********************************************************************************************************************************/
	/********************************************�ŏI�摜�`�揈���I��****************************************************************/
	/********************************************************************************************************************************/
	return S_OK;
}

