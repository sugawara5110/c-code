//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�w�i�摜����                                            **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <stdlib.h>
#include <time.h>

void back_image(int f){//f����p

	static int hk;   //�摜�n���h��
	int r;    //�����_��
	
	srand((unsigned)time(NULL));
	
	if (f == 0){
        r = rand() % 5;
		if (r == 0)hk = LoadGraph("./dat/hk/hk0.dat");//�w�i�摜�ǂݍ���
		if (r == 1)hk = LoadGraph("./dat/hk/hk1.dat");
		if (r == 2)hk = LoadGraph("./dat/hk/hk2.dat");
		if (r == 3)hk = LoadGraph("./dat/hk/hk3.dat");
		if (r == 4)hk = LoadGraph("./dat/hk/hk4.dat");

		return ;
	}

	if (f == 1){

		DrawGraph(0, 0, hk, TRUE);//�w�i�摜�`��
		
		return;
	}

}