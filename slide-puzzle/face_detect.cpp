//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@��ʌ��o�����֐�(����X���b�h)                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <process.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include "ImageRead.h"
#include "ImageDraw.h"

unsigned __stdcall face_detect(void *th){

	// �w�K�ς݌��o��̓ǂݍ���
	cv::string cascadeName = "dat\\xml\\haarcascade_frontalface_alt2.xml";
	cv::CascadeClassifier cascade;
	if (!cascade.load(cascadeName))
		return -1;

	ImageRead::drawdata_t d_t = (ImageRead::drawdata_t)th;//�{�X���b�h����f�[�^�󂯎��(drawdata��drawdata_t�͒��g�͓���)
	cv::vector<cv::Rect> faces;     //���o�ӏ�
	cv::VideoCapture video;        //���o�X���b�h����Đ��n���h��
	cv::Mat ori, ori2;
	cv::Mat dts;
	cv::Mat grayImage;         //���o�p�O���[�X�P�[���f�[�^�i�[�p
	REFTIME time1;            //�{�X���b�h�Ɠ������p�ϐ�
	int x, y, i, j, x1, y1;  //for

	typedef struct{
		int max_x;
		int max_y;
		int min_x;
		int min_y;
	}detect_range;
	detect_range dtr[200];//��ʌ��o�͈�
	int dtrp;            //���o��
	int dti;            //�Y����
	int col, row;      //for
	int xs, ys;       //���o�ӏ����T�C�Y
	int xrs, yrs;    //���o�ӏ����T�C�Y��T�C�Y

	d_t->th_st = 0;         //�X���b�h�i�s�󋵏�����

	while (1){            //���̊֐����Ă΂ꂽ�炸���ƃ��[�v�����ςȂ� 
		try{             //��O����,�@�\���Ă邩�m��񂪈ꉞ�����B
			if (video.isOpened() == true){//����t�@�C����open�̏ꍇ
				video.release();         //����t�@�C����
			}

			while (d_t->th_f == 1 && (d_t->gfr == 6 || d_t->gfr == 7 || d_t->gfr == 10)){//���o�����X�^�[�g�t���O,  ���o�����I��

				switch (d_t->mcf){  //0:�Î~��I�� 1:����I�� 2:�J�����I��

					//�Î~�揈���J�n
				case 0:
					ori = cv::imread(d_t->g_name, CV_LOAD_IMAGE_COLOR);//�摜�ǂݍ���

					break;
					//�Î~�揈���I��

					//���揈���J�n
				case 1:
					if (video.isOpened() == false){//����t�@�C��close�̏ꍇ
						video.open(d_t->g_name);  //����t�@�C��open
					}

					d_t->pMediaPosition->get_CurrentPosition(&time1);//�Đ��ʒu�擾(�b)
					video.set(CV_CAP_PROP_POS_MSEC, time1 * 1000);//�f������(�������̓~���b�P�ʂȂ̂�1000������)
					video >> ori;                         //�t���[���ǂݍ���
					if (ori.empty()){                    //�t���[�����󂩁H	
						video.release();
						video.open(d_t->g_name);
						video >> ori;
					}

					break;
					//���揈���I��

					//�J���������J�n
				case 2:
					d_t->cap >> ori;
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

				/*****************************************��ʃ��U�C�N,�炷���ւ������J�n***********************************************************/
				if (d_t->gfr == 7 || d_t->gfr == 10){
					//�����Ƀ��T�C�Y
					cv::resize(ori, ori2, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
					dtrp = 0;//���o��������
					dti = 0;//�Y����

					//���o�͈�,���T�������J�n
					for (row = 0; row < dts.rows; row++){
						for (col = 0; col < dts.cols; col++){

							//if255�������Ă��邩
							if (dts.data[row * dts.step + col * dts.channels() + 2] == 0 &&
								dts.data[row * dts.step + col * dts.channels() + 1] == 0 &&
								dts.data[row * dts.step + col * dts.channels() + 0] == 255)
							{
								dtr[dti].max_x = dtr[dti].max_y = 0;
								dtr[dti].min_x = dts.cols - 1;
								dtr[dti].min_y = dts.rows - 1;
								x1 = col;
								y1 = row;
								while (1){//while1�񔲂���1��`�I��
									//���U�C�N�`��͈͌��菈��
									if (dtr[dti].max_x < x1)dtr[dti].max_x = x1;
									if (dtr[dti].max_y < y1)dtr[dti].max_y = y1;
									if (dtr[dti].min_x > x1)dtr[dti].min_x = x1;
									if (dtr[dti].min_y > y1)dtr[dti].min_y = y1;

									//��`�̐}�`���C�������ǂ��Ĕ͈͂�T��,�T���ӏ���0�ɍX�V����255�������Ȃ�����I���
									if (y1 - 1 >= 0 &&
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 2] == 0 &&
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 1] == 0 &&
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 0] == 255)
									{
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 0] = 0;
										y1 -= 1;
										continue;
									}

									if (y1 + 1 < dts.rows &&
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 2] == 0 &&
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 1] == 0 &&
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 0] == 255)
									{
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 0] = 0;
										y1 += 1;
										continue;
									}

									if (x1 - 1 >= 0 &&
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 2] == 0 &&
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 1] == 0 &&
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 0] == 255)
									{
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 0] = 0;
										x1 -= 1;
										continue;
									}

									if (x1 + 1 < dts.cols &&
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 2] == 0 &&
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 1] == 0 &&
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 0] == 255)
									{
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 0] = 0;
										x1 += 1;
										continue;
									}
									//��`�̐}�`���C��1�s�N�Z�����I��

									break;

								}//while1�񔲂���1��`�I��

								//1��`�I�������̂œY����1�ǉ�
								dti++;
								if (dti >= 200)break;//�z��A�N�Z�X�ᔽ�h�~
							}//if255�������Ă��邩
						}//for cols
						if (dti >= 200){ dti = 200; break; }//�z��A�N�Z�X�ᔽ�h�~
					}//for rows
					dtrp = dti;//���o�����
					//���o�͈�,���T�������I��

					if (d_t->gfr == 7){
						//���o�ӏ����U�C�N�����J�n(1���[�v1�ӏ�)
						for (dti = 0; dti < dtrp; dti++){
							//if�A�N�Z�X�ᔽ�h�~
							if (dts.rows - dtr[dti].max_y >= 10 && dts.rows - dtr[dti].min_y >= 10 &&
								dts.cols - dtr[dti].max_x >= 10 && dts.cols - dtr[dti].min_x >= 10){
								for (y = dtr[dti].min_y; y <= dtr[dti].max_y; y += 10){
									for (x = dtr[dti].min_x; x <= dtr[dti].max_x; x += 10){
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
								}//for�����܂�
							}//if�A�N�Z�X�ᔽ�h�~
						}//for dti
						//���o�ӏ����U�C�N�����I��

					}//if gfr == 7

					if (d_t->gfr == 10){
						//���o�ӏ������ւ������J�n
						for (dti = 0; dti < dtrp; dti++){

							//����ւ����摜�T�C�Y����
							xs = dtr[dti].max_x - dtr[dti].min_x + 1;
							ys = dtr[dti].max_y - dtr[dti].min_y + 1;
							//����ւ���摜�T�C�Y����
							xrs = dtr[dtrp - dti - 1].max_x - dtr[dtrp - dti - 1].min_x + 1;
							yrs = dtr[dtrp - dti - 1].max_y - dtr[dtrp - dti - 1].min_y + 1;

							//if�A�N�Z�X�ᔽ�h�~
							if (dts.rows > dtr[dti].min_y + ys &&
								dts.cols > dtr[dti].min_x + xs &&
								dts.rows > dtr[dtrp - dti - 1].min_y + yrs &&
								dts.cols > dtr[dtrp - dti - 1].min_x + xrs){

								for (y = 0; y < yrs; y++){
									for (x = 0; x < xrs; x++){

										dts.data[(y + dtr[dtrp - dti - 1].min_y) * dts.step +
											(x + dtr[dtrp - dti - 1].min_x) * dts.channels() + 2] =
											ori2.data[(y * ys / yrs + dtr[dti].min_y) * ori2.step +
											(x * xs / xrs + dtr[dti].min_x) * ori2.channels() + 2];

										dts.data[(y + dtr[dtrp - dti - 1].min_y) * dts.step +
											(x + dtr[dtrp - dti - 1].min_x) * dts.channels() + 1] =
											ori2.data[(y * ys / yrs + dtr[dti].min_y) * ori2.step +
											(x * xs / xrs + dtr[dti].min_x) * ori2.channels() + 1];

										dts.data[(y + dtr[dtrp - dti - 1].min_y) * dts.step +
											(x + dtr[dtrp - dti - 1].min_x) * dts.channels() + 0] =
											ori2.data[(y * ys / yrs + dtr[dti].min_y) * ori2.step +
											(x * xs / xrs + dtr[dti].min_x) * ori2.channels() + 0];

									}
								}
							}//if�A�N�Z�X�ᔽ
						}//for dti
						//���o�ӏ������ւ������I��
					}//if gfr == 10
				}//gfr==7,10�����I��
				/*****************************************��ʃ��U�C�N,�炷���ւ������I��***********************************************************/
				d_t->lock_t = 1;                                //�����X���b�h���b�N
				if (d_t->lock == 1){ d_t->lock_t = 0; continue; }//�{�X�����b�N��,�����X���b�h���b�N������continue
				d_t->mt_temp = dts.clone();                   //�{�X���f�[�^�����n��.clone()���S�R�s�[
				d_t->lock_t = 0;                             //�����X���b�h���b�N����
				d_t->th_st = 1;//�X���b�h�i�s��(NULL�ɂ��G���[�h�~)
				if (d_t->th_fin == 1)return 0;//�X���b�h����
			}//while���o��
		}
		catch (...){
			// "..." ���w�肷��� try �u���b�N���Ŕ��������S�Ă̗�O��ߑ����܂��B���̂Ƃ��A��O��ϐ��Ŏ󂯂邱�Ƃ͂ł��܂���B
			continue;
		}
		if (d_t->th_fin == 1)return 0;//�X���b�h����
	}//while(�����ƃ��[�v�����ςȂ�)

	return 0;
}
