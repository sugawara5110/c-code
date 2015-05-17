//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(�`��ǂݍ���)                 **//
//**                                    (ImageDraw�̊��N���X)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "Move.h"
#include "Filter.h"

void ImageRead::face_detect_after(){    //���o������̕`�揈���֐�

	cv::Mat dte;
	d.lock = 1;                               //�����X���b�h����̃f�[�^���샍�b�N
	if (d.lock_t == 1){ d.lock = 0; return; }//�����X���b�h���b�N�̏ꍇ, ���b�N������return
	dte = d.mt_temp.clone();                //�{�X���f�[�^�����n��.clone()���S�R�s�[
	d.lock = 0;                            //���b�N����

	int i, j;

	//dte.data[]�̗v�f���Ƀf�[�^�������Ă�v�f�̂݊g��`��
	switch (d.mcf){

	case 0://�Î~��
		for (j = 0; j < g2.rows; j++){
			for (i = 0; i < g2.cols; i++){
				
				if (dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2] > 0 ||   //�f�[�^�̗L��
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1] > 0 ||
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0] > 0)
				{
					g2.data[j * g2.step + i * g2.channels() + 2] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2];
					g2.data[j * g2.step + i * g2.channels() + 1] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1];
					g2.data[j * g2.step + i * g2.channels() + 0] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0];
				}
			}
		}
		break;

	case 1://����
		for (j = 0; j < pVideoInfoHeader->bmiHeader.biHeight; j++){
			for (i = 0; i < pVideoInfoHeader->bmiHeader.biWidth; i++){
				int offset = j * linesize + i * 3;
				int hei = pVideoInfoHeader->bmiHeader.biHeight - j - 1;//directshow�͋t����Ȃ̂Ŕ��] 

				if (dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 2] > 0 ||   //�f�[�^�̗L��
					dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 1] > 0 ||
					dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 0] > 0)
				{
					pBuffer[offset + 2] = dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 2];
					pBuffer[offset + 1] = dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 1];
					pBuffer[offset + 0] = dte.data[hei / 2 * dte.step + i / 2 * dte.channels() + 0];
				}
			}
		}
		break;

	case 2://�J����
		for (j = 0; j < c_img.rows; j++){
			for (i = 0; i < c_img.cols; i++){
				if (dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2] > 0 ||   //�f�[�^�̗L��
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1] > 0 ||
					dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0] > 0)
				{
					c_img.data[j * c_img.step + i * c_img.channels() + 2] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 2];
					c_img.data[j * c_img.step + i * c_img.channels() + 1] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 1];
					c_img.data[j * c_img.step + i * c_img.channels() + 0] =
						dte.data[j / 2 * dte.step + i / 2 * dte.channels() + 0];
				}
			}
		}
		break;

	}//switch

}

int ImageRead::drawing_img(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z, int f){ //move==NULL:�����H�摜�\��, x,y�͕`�掞�̃I�t�Z�b�g��, f == 0:�Î~��X�V��, 1:�Î~��X�V�L(����͏�ɍX�V)

	int i, j;//for

	if ((d.gfr == 6 || d.gfr == 7 || d.gfr == 10) && face_detect_h == NULL){
		face_detect_h = (HANDLE)_beginthreadex(NULL, 0, face_detect, &d, 0, NULL);//�X���b�h�X�^�[�g
	}
	if (d.gfr != 6 && d.gfr != 7 && d.gfr != 10){ d.th_f = 0; d.th_st = 0; }//���o�������I�����t���O������

	/********************************************************************************************************************************/
	/********************************************���� �J�������������J�n*************************************************************/
	/********************************************************************************************************************************/

	if (d.mcf == 1 || d.mcf == 2){//����,�J�����ǂݍ��ݏ����J�n
		if (d.mcf == 1){//����ǂݍ��ݏ����J�n

			//�X�g���[���̍��v���ԕ�����Ƃ���A���݂̈ʒu���擾����B
			d.pMediaPosition->get_CurrentPosition(&d.time1);

			//���ʒu�ƏI���ʒu�������ꍇ�X�^�[�g�ʒu�ɃZ�b�g
			if (d.time1 == d.time2)d.pMediaPosition->put_CurrentPosition(0);

			//�ŐV�摜�f�[�^��pBuffer�Ɋi�[
			pSampleGrabber->GetCurrentBuffer(&nBufferSize, (long *)(pBuffer));

			d.th_f = 1;//���o�X���b�h�X�^�[�g

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

			d.th_f = 1;//���o�X���b�h�ғ��@   

		}//�J�����ǂݍ��ݏ����I��

		//�J����,����I�����̌��o�㏈��
		if (d.gfr == 6 || d.gfr == 7 || d.gfr == 10){
			if (d.th_st){            //���o�X���b�h1��ڂ̏����������Ă邩?
				face_detect_after();//���o������̕`�揈���֐��Ăяo��
			}
		}

		if (FAILED(pic_resize(dx))) return -1;//�摜���T�C�Y����

	}//����,�J�����ǂݍ��ݏ����I��

	/********************************************************************************************************************************/
	/********************************************���� �J�������������I��*************************************************************/
	/********************************************************************************************************************************/

	/********************************************************************************************************************************/
	/*********************************************�Î~�揉�������J�n*****************************************************************/
	/********************************************************************************************************************************/
	if (d.mcf == 0){
		if (f == 1){   //�X�V���K�v�Ȏ��̂ݎ��s����
			g2 = cv::imread(d.g_name, CV_LOAD_IMAGE_COLOR);//�摜�ǂݍ���

			if (d.gfr == 6 || d.gfr == 7 || d.gfr == 10){//��ʌ��o�I����
				d.th_f = 1;                 //���o�X���b�h���o�����[�vON
				while (d.th_st != 1);      //���o�X���b�h�������҂�(volatile�t���Ȃ��ƍœK���Ŗ������[�v�ɂȂ�)
				face_detect_after();      //���o������̕`�揈���֐��Ăяo��
			}

			if (FAILED(pic_resize(dx))) return -1;    //�摜���T�C�Y����
		}

		if (f == 0){  //�X�V�����̏ꍇ���s����
			for (j = 0; j < yrs; j++){
				for (i = 0; i < xrs; i++){
					imgpi[j][i] = imcpy1[j][i];
				}
			}
		}
	}

	/********************************************************************************************************************************/
	/*********************************************�Î~�揉�������I��*****************************************************************/
	/********************************************************************************************************************************/


	if (FAILED(draw(dx, filter, move, x, y, z))) return -1;//�e��摜����,�`�揈��
	return 0;
}//drawing_img()�I��

HRESULT ImageRead::draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z){ return S_OK; }//�I�[�o�[���C�h�Ŏg��