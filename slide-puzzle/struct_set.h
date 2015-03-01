//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�\���̌^��`.h                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

typedef struct {          
	int cx;              //���摜x���W
	int cy;             //���摜y���W
	int fx;            //�����摜x���W
	int fy;           //�����摜y���W
	int chx;         //change�֐��px���W
	int chy;        //change�֐��py���W
} imxy;            //�摜���W�\����img


typedef struct {             
	int idx;                //img �Y����
	int pcs;               //�u���b�N�c,��������
	int bsize;            //�u���b�N�T�C�Y
	int lastposx;        //�E����x���W
	int lastposy;       //�E����y���W
	int rand_i;        //�����_��������
	int rand_s;       //�����_���͈�
	int count;       //change�֐��̈ړ������u���b�N����
	int move;       //�u���b�N�ړ���
	int movem;     //����p�u���b�N�ړ���
}para;            //�u���b�N�����̃p�����[�^�[�\����paras

typedef struct {
	cv::VideoCapture cap;         //USB�J�����n���h��
	cv::Mat mt_temp;             //���o�摜�i�[
	int th_f;                   //���o�X���b�h����
	volatile int th_st;        //���o�X���b�h�i�s�� volatile �R���p�C���œK���h�~
	volatile int lock, lock_t;//�f�[�^���쎞���b�N(�r������)
	imxy *img;                         //�摜���W 
	int **imgpi;                      //�摜�s�N�Z�����
	int **imcpy;                     //�摜�s�N�Z�����R�s�[
	int **block;                    //�u���b�N�摜�t�B���^�[
	int xrs, yrs;                  //�摜���T�C�Y��̃T�C�Y
	int finish;                   //�����摜�\���t���O 0:��\�� 1:�\�� (�}�E�X�֐��ő���,�ړ��{�^���͈͓��ɃJ�[�\�����\��)
	int mc;                      //pic_resize���̃������m�ۃ`�F�b�N
	char g_name[100];           //�摜�t�@�C�����i�[
    int size;                  //paras�Y����
	int space[2];             //�X�y�[�X���W0:x 1:y
    para paras[6];           //�u���b�N���̃p�����[�^�[
	int gfr;                //�摜���[�h 0:�ʏ� 1:���m�N�� 2:���U�C�N 3:�G�b�W���o 4:�G���{�X���� 5:�G�敗���� 6:��ʌ��o���� 7:��ʃ��U�C�N 8:�l�K�|�W
	int mof;               //����t�@�C���I�[�v���p�n���h��(�Đ�,��~���ʗp)
	int tkf;              //�萔�v�Z���s�t���O
	int tkc;             //�萔�v�Z�J�E���g
	int cnt_results;    //auto_matic�֐��̃J�E���g����
	int mcf;           //0:�Î~��I�� 1:����I�� 2:�J�����I�� 
}alldata, *alldata_t; //�قڑS�֐��Ŏg���f�[�^�Q�\����p   ���o�X���b�h�f�[�^ p_t