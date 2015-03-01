//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�t�@�C������                                    �@�@�@**//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <string.h>
#include "struct_set.h"
#include "drawing_img.h"
#include "mouse.h"
#include "sound.h"

void file(alldata *p){   //�摜�t�@�C���I���֐�
     
	HANDLE hFile;
	WIN32_FIND_DATA fd;
	
    char f_name_tbl[256][100];   //�摜�t�@�C����������i�[
	int stl;                    //�摜�t�@�C���������񒷂�
	int x, i, frg, k;
	int mv;                  //�ړ��ʕύX�p
	
	i = 3;
	strcpy(f_name_tbl[1],"./dat/img/nekokin.dat");//�ŏ��̃t�@�C�����i�[
	strcpy(f_name_tbl[2], "./dat/img/z_cam_ewc.dat");//�J�����p�̃t�@�C�����i�[
     
	hFile = FindFirstFile("*.*", &fd);//�T���ꏊ�J�����g�f���N�g��

    do{
		//�g����摜��BMP,JPEG,PNG,DDS,ARGB,TGA �̂U��ނł��B
	   
		//���摜�t�@�C���I��  
		if (strlen(fd.cFileName) > 100)continue;//�����̒�����100�����̏ꍇ�X�L�b�v
		stl = strlen(fd.cFileName) - 4;
		if (!strcmp(fd.cFileName + stl, ".mpg") || !strcmp(fd.cFileName + stl, ".avi") ||
			!strcmp(fd.cFileName + stl, ".MPG") || !strcmp(fd.cFileName + stl, ".AVI")){
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
    
    k=i-1;
    i=1;
    x=800;
    frg=1;
	while (1){
		mv = 10;//�ʏ�ړ��ʂɍX�V
		strcpy(p->g_name , f_name_tbl[i]);//�t�@�C�����i�[
		stl = strlen(p->g_name) - 4;
		p->mcf = 0;//�I���t�@�C�����ʏ�����
		if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi") || //����I����
			!strcmp(p->g_name + stl, ".MPG") || !strcmp(p->g_name + stl, ".AVI")){
			mv = 20; p->mcf = 1;
		}
		if (!strcmp(p->g_name, "./dat/img/z_cam_ewc.dat")){mv = 20; p->mcf = 2;}//�J�����I����
		PauseMovieToGraph(p->mof);       //����Đ���~
		p->cap.release();               //�J�����I������
		drawing_img(p, x, 0, 2);       //���摜�`��֐�
		 if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//�����I��
	 if(frg!=0){ 
		 sound(2);//�T�E���h�֐��u���b�N�ړ���
		 while (ScreenFlip() == 0 && x != 0){
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//�����I��
			drawing_img(p,x,0, 3);
			mouse(p,1, 0); //�}�E�X�֐�
			if (frg == 1)x -= mv;
			if (frg == 2)x += mv;
         } //while�I���
	 
	 } //if�I���
	
	 do{
		 if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//�����I��
         drawing_img(p, x, 0, 3);
		 frg = mouse(p,1, 0); //�}�E�X�֐�
		 ScreenFlip();
		 if (frg == 3){
			 sound(1);        //�T�E���h�֐��{�^����
			 InitSoftImage();//�\�t�g�C���[�W�S�J��
			 return;
		 }
	 } while (frg == 0);//do�`while�I��

	 if(frg!=0){ 
		 sound(2);//�T�E���h�֐��u���b�N�ړ���
		 while (ScreenFlip() == 0 && x >= -800 && x<800){
			 if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(1); }//�����I��
			 drawing_img(p, x, 0, 3);
			 mouse(p,1,0); //�}�E�X�֐�
			 if (frg == 1)x -= mv;
			 if (frg == 2)x += mv;
		
            } //while�I���
	    if(frg==1)i=i%k+1;
	    if(frg==2){i=i-1;if(i==0)i=k;} 
	 
	 } //if�I���
         if(frg==1)x=800;
	 if(frg==2)x=-800;
    }//while�I���

 } //file()�I���