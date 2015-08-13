//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@File�N���X�����\�b�h��`                          �@�@�@**//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <string.h>
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

char* File::file(Dx9Init *dx, MSG *msg){ //�摜�t�@�C���I���֐�

	int x, flg;
	Menu menu;        //���j���[�I�u�W�F�N�g����
	ImageDraw *fdraw;//�t�@�C���֐��p

	x = 1600;
	flg = 1;
	while (1){
		fdraw = new ImageDraw(dx, f_name_tbl[i]);//�t�@�C���֐��p�I�u�W�F�N�g����

		if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";//���摜�`��֐�

		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {	// PostQuitMessage()���Ă΂ꂽ
				delete fdraw;
				return "stop";
			}
			else {
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		if (flg != 0){
			while (x != 0){

				dx->drawscreen();//�`��

				if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";
				menu.mouse(dx, NULL, 1, 0); //�}�E�X�֐�
				if (flg == 1)x -= 20;
				if (flg == 2)x += 20;
			} //while�I���

		} //if�I���

		do{
			if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg->message == WM_QUIT) {	// PostQuitMessage()���Ă΂ꂽ
					delete fdraw;
					return "stop";
				}
				else {
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(msg);
					DispatchMessage(msg);
				}
			}

			if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";
			flg = menu.mouse(dx, NULL, 1, 0); //�}�E�X�֐�

			dx->drawscreen();//�`��

			if (flg == 3){

				delete fdraw;         //�I������̂Ŕj��
				return f_name_tbl[i];//�t�@�C��������֐����� 
			}

		} while (flg == 0);//do�`while�I��

		if (flg != 0){
			while (x >= -1600 && x < 1600){

				dx->drawscreen();//�`��

				if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";
				menu.mouse(dx, NULL, 1, 0); //�}�E�X�֐�
				if (flg == 1)x -= 20;
				if (flg == 2)x += 20;

			} //while�I���
			if (flg == 1)i = i%k + 1;
			if (flg == 2){ i = i - 1; if (i == 0)i = k; }

		} //if�I���
		if (flg == 1)x = 1600;
		if (flg == 2)x = -1600;
		delete fdraw;//���̉摜�ɐ؂�ւ��̂Ŕj��
	}//while�I���

}//file()�I���

char* File::e_file(ImageDraw *draw, int f){

	if (f == 1)i1 = i1%k + 1;
	if (f == 2){ i1 = i1 - 1; if (i1 == 0)i1 = k; }

	if (draw->getmcf() == 2 && !strcmp(f_name_tbl[i1], "./dat/img/z_cam_ewc.dat")){//�J�����I����

		if (f == 1)i1 = i1%k + 1;
		if (f == 2){ i1 = i1 - 1; if (i1 == 0)i1 = k; }
	}

	return f_name_tbl[i1];

}