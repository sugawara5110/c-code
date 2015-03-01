//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@��ʌ��o�����֐�(����X���b�h)                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <process.h>
#include "struct_set.h"

unsigned __stdcall face_detect(void *th){


	// �w�K�ς݌��o��̓ǂݍ���
	cv::string cascadeName = "dat\\xml\\haarcascade_frontalface_alt2.xml";
	cv::CascadeClassifier cascade;
	if (!cascade.load(cascadeName))
		return -1;

	alldata_t p_t = (alldata_t)th;//�{�X���b�h����f�[�^�󂯎��(alldata��alldata_t�͒��g�͓���)
	cv::vector<cv::Rect> faces;     //���o�ӏ�
	cv::VideoCapture video;        //���o�X���b�h����Đ��n���h��
	cv::Mat ori, ori2;
    cv::Mat dts;
    cv::Mat grayImage;           //���o�p�O���[�X�P�[���f�[�^�i�[�p
    int time1;                  //�{�X���b�h�Ɠ������p�ϐ�
	int x, y, i, j;            //for
	int max_x, max_y,         //��ʃ��U�C�N�͈�
		min_x, min_y;
	p_t->th_st = 0;         //�X���b�h�i�s�󋵏�����

	while (1){            //���̊֐����Ă΂ꂽ�炸���ƃ��[�v�����ςȂ� 
		try{             //��O����,�@�\���Ă邩�m��񂪈ꉞ�����B
			if (video.isOpened() == true){//����t�@�C����open�̏ꍇ
				video.release();         //����t�@�C����
			}

			while (p_t->th_f == 1 && (p_t->gfr == 6 || p_t->gfr == 7)){//���o�����X�^�[�g�t���O,  ���o�����I��

				switch (p_t->mcf){  //0:�Î~��I�� 1:����I�� 2:�J�����I��

					//�Î~�揈���J�n
				case 0:
					ori = cv::imread(p_t->g_name, CV_LOAD_IMAGE_COLOR);//�摜�ǂݍ���

					break;
					//�Î~�揈���I��

					//���揈���J�n
				case 1:
					if (video.isOpened() == false){//����t�@�C��close�̏ꍇ
						video.open(p_t->g_name);  //����t�@�C��open
					}
					time1 = TellMovieToGraph(p_t->mof);     //�Đ��ʒu�擾(�~���b)
					video.set(CV_CAP_PROP_POS_MSEC, time1);//�f����������
					video >> ori;                         //�t���[���ǂݍ���
					if (ori.empty()){                    //�t���[�����󂩁H	
						video.release();
						video.open(p_t->g_name);
						video >> ori;
					}

					break;
					//���揈���I��

					//�J���������J�n
				case 2:
					p_t->cap >> ori;
					break;
					//�J���������I��

				}//switch�I���

				//�����Ƀ��T�C�Y
				cv::resize(ori, dts, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
				//�O���[�X�P�[���ϊ�
				cvtColor(dts, grayImage, CV_RGB2GRAY);
				//���o����
				cascade.detectMultiScale(    //���o����
					grayImage,              // �摜
					faces,                 // �o�͂�����`
					1.1,                  // �k���p�̃X�P�[��
					2,                   // �ŏ��̓��[��
					CV_HAAR_SCALE_IMAGE,// �t���O
					cv::Size(30, 30)   // �ŏ��̋�`
					);

				//���o�ӏ��̋�`�`��p�̃f�[�^�i�[�\���̂�0�ŏ�����
				for (y = 0; y < dts.rows; y++){
					for (x = 0; x < dts.cols; x++){
						dts.data[y * dts.step + x * dts.channels() + 2] = 0;
						dts.data[y * dts.step + x * dts.channels() + 1] = 0;
						dts.data[y * dts.step + x * dts.channels() + 0] = 0;
					}
				}

				// ��`�f�[�^��p_t->dts�Ɋi�[  (.begin �s��̍ŏ��̗v�f���w������Ԃ́C�s��C�e���[�^��Ԃ�)
				for (cv::vector<cv::Rect>::iterator iter = faces.begin(); iter != faces.end(); iter++) {

					rectangle(dts, *iter, cv::Scalar(255, 0, 0), 1);//���o���ɕ`�悷����e
					//void rectangle(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
				}

/*****************************************��ʃ��U�C�N�����J�n***********************************************************/
				if (p_t->gfr == 7){
					//�����Ƀ��T�C�Y
					cv::resize(ori, ori2, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
					max_x = max_y = 0;      //���U�C�N�͈͏�����
					min_x = ori2.cols - 1; //���U�C�N�͈͏�����
					min_y = ori2.rows - 1;//���U�C�N�͈͏�����

					for (y = 0; y < dts.rows; y++){
						for (x = 0; x < dts.cols; x++){
							if (dts.data[y * dts.step + x * dts.channels() + 2] > 0 ||
								dts.data[y * dts.step + x * dts.channels() + 1] > 0 ||
								dts.data[y * dts.step + x * dts.channels() + 0] > 0)
							{
								//���U�C�N�`��͈͌��菈��
								if (max_x < x)max_x = x;
								if (max_y < y)max_y = y;
								if (min_x > x)min_x = x;
								if (min_y > y)min_y = y;

							}
						}
					}
					//if�A�N�Z�X�ᔽ�h�~
					if (dts.rows - max_y >= 10 && dts.rows - min_y >= 10 && dts.cols - max_x >= 10 && dts.cols - min_x >= 10){
						for (y = min_y; y <= max_y; y += 10){
							for (x = min_x; x <= max_x; x += 10){
								for (j = y; j < y + 10; j++){
									for (i = x; i < x + 10; i++){
										//���U�C�N�`�揈��
										dts.data[j * dts.step + i * dts.channels() + 2] =
											ori2.data[y * ori2.step + x * ori2.channels() + 2];
										dts.data[j * dts.step + i * dts.channels() + 1] =
											ori2.data[y * ori2.step + x * ori2.channels() + 1];
										dts.data[j * dts.step + i * dts.channels() + 0] =
											ori2.data[y * ori2.step + x * ori2.channels() + 0];

									}
								}
							}
						}
					}
				}
/*****************************************��ʃ��U�C�N�����I��***********************************************************/
				p_t->lock_t = 1;                                //�����X���b�h���b�N
				if (p_t->lock == 1){p_t->lock_t = 0; continue;}//�{�X�����b�N��,�����X���b�h���b�N������continue
				p_t->mt_temp = dts.clone();                   //�{�X���f�[�^�����n��.clone()���S�R�s�[
				p_t->lock_t = 0;                             //�����X���b�h���b�N����
				p_t->th_st = 1;//�X���b�h�i�s��(NULL�ɂ��G���[�h�~)
			}//while���o��
		}
		catch (...){
			// "..." ���w�肷��� try �u���b�N���Ŕ��������S�Ă̗�O��ߑ����܂��B���̂Ƃ��A��O��ϐ��Ŏ󂯂邱�Ƃ͂ł��܂���B
			continue;
		}

	}//while(�����ƃ��[�v�����ςȂ�)

	return 0;
}
