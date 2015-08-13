//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@ImageRead�N���X��`                                   **//
//**                              (ImageDraw�̊��N���X)                                **//
//*****************************************************************************************//

#ifndef Class_ImageRead_Header
#define Class_ImageRead_Header
// #ifndef ���� #endif �܂ł̊Ԃɒ�`���L�ڂ��܂��B

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <dshow.h>  //directshow
#include <qedit.h> // SampleGrabber�p

class Move; //�O���錾(�v���g�^�C�v�錾�݂����Ȃ���)

class ImageRead{//ImageDraw�̊�{�N���X

private://����J
	IGraphBuilder *pGraphBuilder;      //�C���^�[�t�F�[�X,�O���t�ւ̃t�B���^�̒ǉ��A2 �̃s���̐ڑ���
	IBaseFilter *pSampleGrabberFilter;//�C���^�[�t�F�[�X,�t�B���^�𐧌�
	ISampleGrabber *pSampleGrabber;  //�C���^�[�t�F�[�X,�t�B���^ �O���t����ʂ�X�̃��f�B�A �T���v�����擾
	IVideoWindow *pVideoWindow;     //�C���^�[�t�F�[�X,�r�f�I �E�B���h�E�̃v���p�e�B��ݒ�
	IMediaControl *pMediaControl;  //�C���^�[�t�F�[�X,�t�B���^ �O���t��ʂ�f�[�^ �t���[�𐧌�
	VIDEOINFOHEADER *pVideoInfoHeader;//�\����,�r�f�I �C���[�W�̃r�b�g�}�b�v�ƐF���
	AM_MEDIA_TYPE am_media_type;     //���f�B�A �T���v���� ���W���[ �^�C�v���w�肷��O���[�o����ӎ��ʎq (GUID)

	IMediaPosition *pMediaPosition; //�C���^�[�t�F�[�X,�X�g���[�����̈ʒu���V�[�N
	REFTIME time2;                   //����̑S�Đ�����
	IBasicAudio *pBasicAudio;      //�C���^�[�t�F�[�X,�I�[�f�B�I �X�g���[���̃{�����[���ƃo�����X�𐧌�

	long nBufferSize;//�o�b�t�@�T�C�Y�T�C�Y
	BYTE *pBuffer;  //�s�N�Z���f�[�^�o�b�t�@
	int linesize;   //1���C���T�C�Y

	cv::Mat g2;                 //�Î~��n���h��
	HANDLE face_detect_h;    //���o�X���b�h�n���h��
	cv::Mat c_img;           //�J�����摜�i�[
	int xs, ys;  //�摜���T�C�Y�O�̃T�C�Y
	float rate; //�Đ����x

	void face_detect_after(); //��ʌ��o�̌㏈��
	int resize();             //�摜���T�C�Y����

protected://�h���N���X�ȊO����J
	typedef struct{
		REFTIME time1;          //����̌��Đ��ʒu
		cv::VideoCapture cap;  //USB�J�����n���h��
		cv::Mat mt_temp;             //���o�摜�i�[
		int th_f;                    //���o�X���b�h����
		volatile int th_st;         //���o�X���b�h�i�s�� volatile �R���p�C���œK���h�~
		volatile int lock, lock_t;//�f�[�^���쎞���b�N(�r������)
		int th_fin;               //�X���b�hreturn�t���O
		char g_name[100];        //�摜�t�@�C�����i�[
		int gfl;                //�摜���[�h 0:�ʏ� 1:���m�N�� 2:���U�C�N 3:�G�b�W���o 4:�G���{�X���� 5:�G�敗���� 6:��ʌ��o���� 7:��ʃ��U�C�N 8:�l�K�|�W 9:�摜�G���{�X 10:�炷���ւ� 11:�X���b�g�X�L���� 12:�m�C�Y����
		int mcf;               //0:�Î~��I�� 1:����I�� 2:�J�����I��
	}drawdata;               //���o�X���b�h�Ԃł̃f�[�^        
	drawdata d;

public://���J
	int **imgpi;   //�摜�s�N�Z�����
	int xrs, yrs; //�摜���T�C�Y��̃T�C�Y
	
	ImageRead();            //�K��R���X�g���N�^
	ImageRead(char *name); //�����L�R���X�g���N�^(�t�@�C����)
	int read_img();       //�摜�ǂݍ���
	~ImageRead();        //�f�X�g���N�^

	//�ϐ�����
	int getgfl();               //gfl�o��
	int getmcf();              //mcf�o��
	REFTIME gettime1();      //���Đ��ʒu�o��
	REFTIME gettime2();     //�S�Đ����ԏo��
	float getrate();        //�Đ����x�o��
	void putmediapos(REFTIME p);//�Đ��ʒu����
	void putrate(float ra);      //�Đ����x����
	void putVol(int v);         //���ʓ���
};

#endif