//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�\���̌^��`.h                                        **//
//**                                                                                     **//
//*****************************************************************************************//

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
} para;           //�u���b�N�����̃p�����[�^�[�\����paras

typedef struct {                   
	imxy *img;                        //�摜���W 
	int **imgpi;                     //�摜�s�N�Z�����
	int **imcpy;                    //�摜�s�N�Z�����R�s�[
	int **block;                   //�u���b�N�摜�t�B���^�[
	int xrs, yrs;                 //�摜���T�C�Y��̃T�C�Y
	int mc;                      //pic_resize���̃������m�ۃ`�F�b�N
	char g_name[25];            //�摜�t�@�C�����i�[
    int size;                  //paras�Y����
	int space[2];             //�X�y�[�X���W0:x 1:y
    para paras[6];           //�u���b�N���̃p�����[�^�[
	int gfr;                //�摜���[�h 0:�ʏ� 1:���m�N�� 2:���U�C�N 3:�G�b�W���o 4:�G���{�X���� 5:���ʉ敗����
	int mof;               //����t�@�C���I�[�v���p�n���h��(�Đ�,��~���ʗp)
	int tkf;              //�萔�v�Z���s�t���O
	int tkc;             //�萔�v�Z�J�E���g
	int cnt_results;    //auto_matic�֐��̃J�E���g����
}alldata;              //�قڑS�֐��Ŏg���f�[�^�Q�\����p