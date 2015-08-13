//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(�`��ǂݍ���)                 **//
//**                                    (ImageDraw�̊��N���X)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "Move.h"

void ImageRead::face_detect_after(){    //���o������̕`�揈���֐�

	cv::Mat dte;
	d.lock = 1;                                //�����X���b�h����̃f�[�^���샍�b�N
	if (d.lock_t == 1){ d.lock = 0; return; }//�����X���b�h���b�N�̏ꍇ, ���b�N������return
	dte = d.mt_temp.clone();                //�{�X���f�[�^�����n��.clone()���S�R�s�[
	d.lock = 0;                             //���b�N����

	int i, j;

	//dte.data[]�̗v�f���Ƀf�[�^�������Ă�v�f�̂݊g��`��
	for (j = 0; j < yrs; j++){
		for (i = 0; i < xrs; i++){

			if (dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 2] > 0 ||  //�f�[�^�̗L��
				dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 1] > 0 ||
				dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 0] > 0)
			{
				imgpi[yrs - j - 1][i] =
					(dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 2] << 16) +
					(dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 1] << 8) +
					dte.data[(j * dte.rows / yrs) * dte.step + (i * dte.cols / xrs) * dte.channels() + 0];
			}
		}
	}
}

int ImageRead::read_img(){ //�摜�ǂݍ���

	if ((d.gfl == 6 || d.gfl == 7 || d.gfl == 10) && face_detect_h == NULL){
		face_detect_h = (HANDLE)_beginthreadex(NULL, 0, face_detect, &d, 0, NULL);//�X���b�h�X�^�[�g
	}
	if (d.gfl != 6 && d.gfl != 7 && d.gfl != 10){ d.th_f = 0; d.th_st = 0; }//���o�������I�����t���O������

	if (d.mcf == 0)g2 = cv::imread(d.g_name, CV_LOAD_IMAGE_COLOR);//�摜�ǂݍ���

	if (d.mcf == 1){//����ǂݍ��ݏ����J�n

		//�Đ����x1.0�W�� �L���͈�0.1�`4.0 
		pMediaPosition->put_Rate(rate);

		//�X�g���[���̍��v���ԕ�����Ƃ���A���݂̈ʒu���擾����B
		pMediaPosition->get_CurrentPosition(&d.time1);

		//���ʒu�ƏI���ʒu�������ꍇ�X�^�[�g�ʒu�ɃZ�b�g
		if (d.time1 == time2)pMediaPosition->put_CurrentPosition(0);

		//�ŐV�摜�f�[�^��pBuffer�Ɋi�[
		pSampleGrabber->GetCurrentBuffer(&nBufferSize, (long *)(pBuffer));

	}//����ǂݍ��ݏ����I��

	if (d.mcf == 2){//�J�����ǂݍ��ݏ����J�n

		int cn = 0;//�J�E���g
		do{
			d.cap >> c_img;           //�摜�f�[�^�X�V
			if (cn == 3){
				d.mcf = 0; g2 = cv::imread("./dat/img/z_cam_ewc.dat", CV_LOAD_IMAGE_COLOR);//�摜�ǂݍ���
				break;
			}
			cn++;
		} while (c_img.data == NULL);//���g��NULL�̊Ԃ͎��̏����ɍs���Ȃ�

	}//�J�����ǂݍ��ݏ����I��

	if (FAILED(resize())) return -1;//�摜���T�C�Y����

	if (d.gfl == 6 || d.gfl == 7 || d.gfl == 10){
		d.th_f = 1;                 //���o�X���b�h���o�����[�vON
		if (d.th_st){            //���o�X���b�h1��ڂ̏����������Ă邩?
			face_detect_after();//���o������̕`�揈���֐��Ăяo��
		}
	}
	return 0;
}