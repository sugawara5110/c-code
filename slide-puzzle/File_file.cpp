//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@File�N���X�����\�b�h��`                          �@�@�@**//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <string.h>
#include "sound.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Menu.h"
#include "File.h"

File::File(){

	i = 3;
	strcpy(f_name_tbl[1], "./dat/img/nekokin.dat");//�ŏ��̃t�@�C�����i�[
	strcpy(f_name_tbl[2], "./dat/img/z_cam_ewc.dat");//�J�����p�̃t�@�C�����i�[

	hFile = FindFirstFile("*.*", &fd);//�T���ꏊ�J�����g�f���N�g��

	do{
		//�g����摜��BMP,JPEG,PNG,DDS,ARGB,TGA �̂U��ނł��B

		//���摜�t�@�C���I��  
		if (strlen(fd.cFileName) > 100)continue;//�����̒�����100�����̏ꍇ�X�L�b�v
		int stl = strlen(fd.cFileName) - 4;
		if (!strcmp(fd.cFileName + stl, ".mpg") || !strcmp(fd.cFileName + stl, ".avi") ||
			!strcmp(fd.cFileName + stl, ".MPG") || !strcmp(fd.cFileName + stl, ".AVI") ||
			!strcmp(fd.cFileName + stl, ".OGG") || !strcmp(fd.cFileName + stl, ".ogg") ||
			!strcmp(fd.cFileName + stl, ".WMV") || !strcmp(fd.cFileName + stl, ".wmv") ||
			!strcmp(fd.cFileName + stl, ".FLV") || !strcmp(fd.cFileName + stl, ".flv") ||
			!strcmp(fd.cFileName + stl, ".MP4") || !strcmp(fd.cFileName + stl, ".mp4")){
			strcpy(f_name_tbl[i++], fd.cFileName); continue;
		}

		if (!strcmp(fd.cFileName + stl, ".bmp") || !strcmp(fd.cFileName + stl, ".png") || !strcmp(fd.cFileName + stl, ".dds") ||
			!strcmp(fd.cFileName + stl, ".tga") || !strcmp(fd.cFileName + stl, ".jpg") || !strcmp(fd.cFileName + stl, ".BMP") ||
			!strcmp(fd.cFileName + stl, ".PNG") || !strcmp(fd.cFileName + stl, ".DDS") || !strcmp(fd.cFileName + stl, ".TGA") ||
			!strcmp(fd.cFileName + stl, ".JPG")){
			strcpy(f_name_tbl[i++], fd.cFileName); continue;
		}

		stl = strlen(fd.cFileName) - 5;
		if (!strcmp(fd.cFileName + stl, ".jpeg") || !strcmp(fd.cFileName + stl, ".argb") ||
			!strcmp(fd.cFileName + stl, ".JPEG") || !strcmp(fd.cFileName + stl, ".ARGB")){
			strcpy(f_name_tbl[i++], fd.cFileName); continue;
		}

	} while (FindNextFile(hFile, &fd) && i < 256);//���̃t�@�C���̃A�h���X
	FindClose(hFile);                 //�N���[�Y

	k = i - 1;
	i = i1 = 1;

}

char* File::file(){ //�摜�t�@�C���I���֐�

	int x, frg;
	Menu menu;        //���j���[�I�u�W�F�N�g����
	ImageDraw *fdraw;//�t�@�C���֐��p

	x = 800;
	frg = 1;
	while (1){
		fdraw = new ImageDraw(f_name_tbl[i]);//�t�@�C���֐��p�I�u�W�F�N�g����

		fdraw->drawing_img(NULL, NULL, x, 0, 1);//���摜�`��֐�
		if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//�����I��
		if (frg != 0){
			sound(2);//�T�E���h�֐��u���b�N�ړ���
			while (ScreenFlip() == 0 && x != 0){
				if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//�����I��
				fdraw->drawing_img(NULL, NULL, x, 0, 0);
				menu.mouse(NULL, 1, 0); //�}�E�X�֐�
				if (frg == 1)x -= 10;
				if (frg == 2)x += 10;
			} //while�I���

		} //if�I���

		do{
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//�����I��
			fdraw->drawing_img(NULL, NULL, x, 0, 0);
			frg = menu.mouse(NULL, 1, 0); //�}�E�X�֐�
			ScreenFlip();
			if (frg == 3){
				sound(1);        //�T�E���h�֐��{�^����
				InitSoftImage();//�\�t�g�C���[�W�S�J��
				delete fdraw;         //�I������̂Ŕj��
				return f_name_tbl[i];//�t�@�C��������֐����� 
			}

		} while (frg == 0);//do�`while�I��

		if (frg != 0){
			sound(2);//�T�E���h�֐��u���b�N�ړ���
			while (ScreenFlip() == 0 && x >= -800 && x < 800){
				if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(1); }//�����I��
				fdraw->drawing_img(NULL, NULL, x, 0, 0);
				menu.mouse(NULL, 1, 0); //�}�E�X�֐�
				if (frg == 1)x -= 10;
				if (frg == 2)x += 10;

			} //while�I���
			if (frg == 1)i = i%k + 1;
			if (frg == 2){ i = i - 1; if (i == 0)i = k; }

		} //if�I���
		if (frg == 1)x = 800;
		if (frg == 2)x = -800;
		delete fdraw;//���̉摜�ɐ؂�ւ��̂Ŕj��
	}//while�I���

} //file()�I���

char* File::e_file(ImageDraw *draw, int f){

	if (f == 1)i1 = i1%k + 1;
	if (f == 2){ i1 = i1 - 1; if (i1 == 0)i1 = k; }

	if (draw->d.mcf == 2 && !strcmp(f_name_tbl[i1], "./dat/img/z_cam_ewc.dat")){//�J�����I����

		if (f == 1)i1 = i1%k + 1;
		if (f == 2){ i1 = i1 - 1; if (i1 == 0)i1 = k; }
	}

	return f_name_tbl[i1];

}