//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�T�E���h����                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"

void sound(int f){
	static int bt; //�{�^�����n���h��
	static int br;//�u���b�N�ړ����n���h��

	if (f == 0){//��������
		bt = LoadSoundMem("./ogg/botan.ogg");
		br = LoadSoundMem("./ogg/mov.ogg");
		return;
	}

	if (f == 1){//�{�^�����Đ�
		PlaySoundMem(bt, DX_PLAYTYPE_BACK, TRUE);
		return;
	}

	if (f == 2){//�u���b�N�ړ����Đ�
		StopSoundMem(br);//�n���h��br�Đ��X�g�b�v
		PlaySoundMem(br, DX_PLAYTYPE_BACK, TRUE);
		return;
	}

}