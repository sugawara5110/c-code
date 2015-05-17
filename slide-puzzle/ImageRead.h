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

class Filter;//�O���錾(�v���g�^�C�v�錾�݂����Ȃ���)
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
	long nBufferSize;
	BYTE *pBuffer;
	int linesize;

	cv::Mat g2;                 //�Î~��n���h��
	HANDLE face_detect_h;      //���o�X���b�h�n���h��
	cv::Mat c_img;            //�J�����摜�i�[
	int mc;                  //pic_resize���̃������m�ۃ`�F�b�N

	void face_detect_after();    //��ʌ��o�̌㏈��
	int pic_resize(Dx9Init *dx);//�摜���T�C�Y����

protected://�h���N���X�ȊO����J
	LPDIRECT3DVERTEXBUFFER9 pMyVB;//���_�o�b�t�@�[
	int **imcpy1; //�摜�s�N�Z�����R�s�[(�Î~��p)
	int xs, ys;  //����`��O�����p�̉摜�T�C�Y
	int d3;     //3D�؂�ւ�

public://���J
	typedef struct{
		IMediaPosition *pMediaPosition; //�C���^�[�t�F�[�X,�X�g���[�����̈ʒu���V�[�N
		REFTIME time1, time2;          //����̍Đ��ʒu,�Đ�����

		cv::VideoCapture cap;         //USB�J�����n���h��
		cv::Mat mt_temp;             //���o�摜�i�[
		int th_f;                   //���o�X���b�h����
		volatile int th_st;        //���o�X���b�h�i�s�� volatile �R���p�C���œK���h�~
		volatile int lock, lock_t;//�f�[�^���쎞���b�N(�r������)
		int th_fin;              //�X���b�hreturn�t���O
		char g_name[100];      //�摜�t�@�C�����i�[
		int gfr;              //�摜���[�h 0:�ʏ� 1:���m�N�� 2:���U�C�N 3:�G�b�W���o 4:�G���{�X���� 5:�G�敗���� 6:��ʌ��o���� 7:��ʃ��U�C�N 8:�l�K�|�W 9:�摜�G���{�X 10:�炷���ւ�
		int mcf;             //0:�Î~��I�� 1:����I�� 2:�J�����I��
	}drawdata, *drawdata_t; //���o�X���b�h�Ԃł̃f�[�^        

	drawdata d;
	IBasicAudio *pBasicAudio;//�C���^�[�t�F�[�X,�I�[�f�B�I �X�g���[���̃{�����[���ƃo�����X�𐧌�

	int **imgpi;    //�摜�s�N�Z�����
	int **imcpy;   //�摜�s�N�Z�����R�s�[
	int xrs, yrs; //�摜���T�C�Y��̃T�C�Y
	int finish;  //�����摜�\���t���O 0:��\�� 1:�\�� (�}�E�X�֐��ő���,�ړ��{�^���͈͓��ɃJ�[�\�����\��)

	ImageRead();                         //�K��R���X�g���N�^
	ImageRead(Dx9Init *dx, char *name); //�����L�R���X�g���N�^(�t�@�C����)
	int drawing_img(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z, int f); //�摜�ǂݍ��ݕ`�揈��
	virtual HRESULT draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z);  //�摜�`�揈��,virtual �I�[�o�[���C�h�����֐�
	~ImageRead();                                                   //�f�X�g���N�^

};

#endif