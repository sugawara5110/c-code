//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@Move�N���X��`                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Move_Header
#define Class_Move_Header
// #ifndef ���� #endif �܂ł̊Ԃɒ�`���L�ڂ��܂��B

class Filter;    //�O���錾(�v���g�^�C�v�錾�݂����Ȃ���)
class ImageDraw;//�O���錾(�v���g�^�C�v�錾�݂����Ȃ���)

class Move{

private://����J��
	int br; //�A�v���I���p

	int auto_space_mov(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw, int *cnt, int *x, int *y);             //�u���b�N�����ړ�
	int auto_round(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw, int *cnt, int *x, int *y, int cx, int cy);//�u���b�N�����ړ�
	int change(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw);                                             //�u���b�N����

public://���J��
	typedef struct {
		int cx;               //���摜x���W
		int cy;              //���摜y���W
		int cz;             //���摜z���W
		int fx;            //�����摜x���W
		int fy;           //�����摜y���W
		int chx;         //change�֐��px���W
		int chy;        //change�֐��py���W
		int chz;       //change�֐��pz���W 
	} imxy;           //�摜���W�\����img

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

	imxy *img;                //�摜���W 
	int size;                //paras�Y����
	int space[2];           //�X�y�[�X���W0:x 1:y
	para paras[6];         //�u���b�N���̃p�����[�^�[

    int tkf;              //�萔�v�Z���s�t���O
	int tkc;             //�萔�v�Z�J�E���g
	int cnt_results;    //auto_matic�֐��̃J�E���g����

	Move();//�R���X�g���N�^
	void coordinates(int si);//���W������
	int mov(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw, int *cnt, int flg, int autof);//�u���b�N�蓮�ړ�
	int auto_matic(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw, int *cnt, int pcs, int range, int j);//�u���b�N�����ړ�
	int count(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw, int *cnt, int pcs, int range, int j);    //�萔�v�Z
	int shuffle(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw);  //�V���b�t��
	int recovery(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw);//���ʂ�
	~Move();//�f�X�g���N�^

};

#endif