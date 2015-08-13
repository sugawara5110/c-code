//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageDraw�N���X�����\�b�h��`(�摜�ۑ�)                 �@�@**//
//**                                    (ImageRead�̔h���N���X)                          **//
//*****************************************************************************************//

#include "ImageRead.h"
#include "ImageDraw.h" 
#include "Move.h"

void ImageDraw::capture(Move *move){

	int i, j, k;
	cv::Mat cap(cv::Size(xrs, yrs), CV_8UC3, cv::Scalar(0, 0, 0));//�J���摜����
	cv::Mat capflip;
	Move::para *prs = &move->paras[move->size];
	Move::imxy *img = move->img;

	//���s�N�Z���f�[�^��Mat�ɏ�������
	//�p�Y�����ȊO�̉摜�`�捶��
	for (j = 0; j < 400; j++){
		for (i = 0; i < (xrs - 400) * 0.5; i++){
			//if�G�b�W���o���̔w�i�F�`��X�L�b�v�����ȉ����������L��
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
			
		}
	}
	//�p�Y�����ȊO�̉摜���_�E��
	for (j = 0; j < 400; j++){
		for (i = xrs - (int)((xrs - 400) * 0.5); i < xrs; i++){
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
		}
	}
	//�p�Y�����ȊO�̉摜���_�㑤
	for (j = 0; j < (yrs - 400) * 0.5; j++){
		for (i = 0; i < 400; i++){
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
		}
	}
	//�p�Y�����ȊO�̉摜���_����
	for (j = yrs - (int)((yrs - 400) * 0.5); j < yrs; j++){
		for (i = 0; i < 400; i++){
			if (d.gfl == 3 && imgpi[j][i] == 0)continue;
			cap.data[j * cap.step + i * cap.channels() + 2] = imgpi[j][i] >> 16 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 1] = imgpi[j][i] >> 8 & 0xff;
			cap.data[j * cap.step + i * cap.channels() + 0] = imgpi[j][i] & 0xff;
		}
	}
	//�p�Y���Ԃ̉摜���_
	if (move->size != 5){//size5�͏������Ȃ�
		for (j = 0; j < 400; j++){
			for (i = 0; i < 400; i++){
				if (d.gfl == 3 && imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] == 0)continue;
				if (block[j][i] == 150 || block[j][i] == 1){//�t�B���^�[�l150,1�̂ݕ`��
					cap.data[(j + (yrs - 400) / 2) * cap.step + (i + (xrs - 400) / 2) * cap.channels() + 2] = 
						imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 16 & 0xff;
					cap.data[(j + (yrs - 400) / 2) * cap.step + (i + (xrs - 400) / 2) * cap.channels() + 1] = 
						imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] >> 8 & 0xff;
					cap.data[(j + (yrs - 400) / 2) * cap.step + (i + (xrs - 400) / 2) * cap.channels() + 0] = 
						imgpi[j + (yrs - 400) / 2][i + (xrs - 400) / 2] & 0xff;
				}
			}
		}
	}
	//�p�Y�����摜���_�����J�n
	for (k = 0; k < prs->idx; k++){
		for (j = 0; j < prs->bsize; j++){     //1�u���b�N��y����
			for (i = 0; i < prs->bsize; i++){//1�u���b�N��x����

				//�G�b�W���o����
				if (d.gfl == 3 && imgpi[j + img[k].fy + (yrs - 400) / 2]
					[i + img[k].fx + (xrs - 400) / 2] == 0)continue;

				//�u���b�N�Ԃ͕`�悵�Ȃ�(�t�B���^�[�l150,1 size==5)
				if ((block[j + img[k].fy][i + img[k].fx] == 150 ||
					block[j + img[k].fy][i + img[k].fx] == 1) &&
					move->size != 5)continue;
				cap.data[(j + img[k].cy + (yrs - 400) / 2) * cap.step + (i + img[k].cx + (xrs - 400) / 2) * cap.channels() + 2] = 
					imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 16 & 0xff;
				cap.data[(j + img[k].cy + (yrs - 400) / 2) * cap.step + (i + img[k].cx + (xrs - 400) / 2) * cap.channels() + 1] = 
					imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] >> 8 & 0xff;
				cap.data[(j + img[k].cy + (yrs - 400) / 2) * cap.step + (i + img[k].cx + (xrs - 400) / 2) * cap.channels() + 0] = 
					imgpi[j + img[k].fy + (yrs - 400) / 2][i + img[k].fx + (xrs - 400) / 2] & 0xff;

			}
		}
	}

	//�摜�������]
	cv::flip(cap, capflip, 0);

	//�摜�ۑ�
	cv::imwrite("capture.jpg", capflip);

}