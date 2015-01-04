//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�t�@�C������                                    �@�@�@**//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <string.h>
#include <dirent.h>
#include "struct_set.h"
#include "drawing_img.h"
#include "mouse.h"
#include "sound.h"

void file(alldata *p){   //�摜�t�@�C���I���֐�
     
    DIR *dir;
    struct dirent *ds;          /* �f�B���N�g���X�g���[���\���� */
    char f_name_tbl[256][25];   //�摜�t�@�C����������i�[
	int stl;                   //�摜�t�@�C���������񒷂�
	int x,i,Color,frg,k; 
	int mv;                  //�ړ��ʕύX�p
	
     i=1;
     Color = GetColor( 255 , 255 , 255 ) ;       
 
    dir=opendir(".\\");  //�T���ꏊ�J�����g�f���N�g��

    for(ds = readdir(dir); ds != NULL; ds = readdir(dir)){
        //�g����摜��BMP,JPEG,PNG,DDS,ARGB,TGA �̂U��ނł��B
	   
		//���摜�t�@�C���I��  
		stl = strlen(ds->d_name) - 4;
		if (!strcmp(ds->d_name + stl, ".mpg") || !strcmp(ds->d_name + stl, ".avi")){ strcpy(f_name_tbl[i++], ds->d_name); continue; }

		if (!strcmp(ds->d_name + stl, ".bmp") || !strcmp(ds->d_name + stl, ".png") || !strcmp(ds->d_name + stl, ".dds") ||
			!strcmp(ds->d_name + stl, ".tga") || !strcmp(ds->d_name + stl, ".jpg")){
			strcpy(f_name_tbl[i++], ds->d_name); continue;
		}
	
	    stl=strlen(ds->d_name)-5;
	    if (!strcmp(ds->d_name + stl, ".jpeg") || !strcmp(ds->d_name + stl, ".argb")){ 
		    strcpy(f_name_tbl[i++], ds->d_name); continue;
	    }
	
    }//for�I���
    closedir(dir);
    
    k=i-1;
    i=1;
    x=800;
    frg=1;
	while (1){
		mv = 10;//�ʏ�ړ��ʂɍX�V
		strcpy(p->g_name , f_name_tbl[i]);//�t�@�C�����i�[
		stl = strlen(p->g_name) - 4;
		if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi") ||
			!strcmp(p->g_name, "z_cam_ewc.bmp")){//���掞�ړ��ʍX�V
			mv = 20;
		}
		PauseMovieToGraph(p->mof);       //����Đ���~
		drawing_img(p, x, 0, 4);        //���摜�`��֐�(�J�����I������)
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