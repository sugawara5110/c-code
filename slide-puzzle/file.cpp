//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　ファイル処理                                    　　　**//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <string.h>
#include "struct_set.h"
#include "drawing_img.h"
#include "mouse.h"
#include "sound.h"

void file(alldata *p){   //画像ファイル選択関数
     
	HANDLE hFile;
	WIN32_FIND_DATA fd;
	
    char f_name_tbl[256][100];   //画像ファイル名文字列格納
	int stl;                    //画像ファイル名文字列長さ
	int x, i, frg, k;
	int mv;                  //移動量変更用
	
	i = 3;
	strcpy(f_name_tbl[1],"./dat/img/nekokin.dat");//最初のファイル名格納
	strcpy(f_name_tbl[2], "./dat/img/z_cam_ewc.dat");//カメラ用のファイル名格納
     
	hFile = FindFirstFile("*.*", &fd);//探索場所カレントデレクトリ

    do{
		//使える画像はBMP,JPEG,PNG,DDS,ARGB,TGA の６種類です。
	   
		//↓画像ファイル選別  
		if (strlen(fd.cFileName) > 100)continue;//文字の長さが100超えの場合スキップ
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
	
	} while (FindNextFile(hFile, &fd) && i < 256);//次のファイルのアドレス
	FindClose(hFile);                 //クローズ
    
    k=i-1;
    i=1;
    x=800;
    frg=1;
	while (1){
		mv = 10;//通常移動量に更新
		strcpy(p->g_name , f_name_tbl[i]);//ファイル名格納
		stl = strlen(p->g_name) - 4;
		p->mcf = 0;//選択ファイル判別初期化
		if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi") || //動画選択時
			!strcmp(p->g_name + stl, ".MPG") || !strcmp(p->g_name + stl, ".AVI")){
			mv = 20; p->mcf = 1;
		}
		if (!strcmp(p->g_name, "./dat/img/z_cam_ewc.dat")){mv = 20; p->mcf = 2;}//カメラ選択時
		PauseMovieToGraph(p->mof);       //動画再生停止
		p->cap.release();               //カメラ終了処理
		drawing_img(p, x, 0, 2);       //動画像描画関数
		 if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//強制終了
	 if(frg!=0){ 
		 sound(2);//サウンド関数ブロック移動音
		 while (ScreenFlip() == 0 && x != 0){
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//強制終了
			drawing_img(p,x,0, 3);
			mouse(p,1, 0); //マウス関数
			if (frg == 1)x -= mv;
			if (frg == 2)x += mv;
         } //while終わり
	 
	 } //if終わり
	
	 do{
		 if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(0); }//強制終了
         drawing_img(p, x, 0, 3);
		 frg = mouse(p,1, 0); //マウス関数
		 ScreenFlip();
		 if (frg == 3){
			 sound(1);        //サウンド関数ボタン音
			 InitSoftImage();//ソフトイメージ全開放
			 return;
		 }
	 } while (frg == 0);//do～while終了

	 if(frg!=0){ 
		 sound(2);//サウンド関数ブロック移動音
		 while (ScreenFlip() == 0 && x >= -800 && x<800){
			 if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); exit(1); }//強制終了
			 drawing_img(p, x, 0, 3);
			 mouse(p,1,0); //マウス関数
			 if (frg == 1)x -= mv;
			 if (frg == 2)x += mv;
		
            } //while終わり
	    if(frg==1)i=i%k+1;
	    if(frg==2){i=i-1;if(i==0)i=k;} 
	 
	 } //if終わり
         if(frg==1)x=800;
	 if(frg==2)x=-800;
    }//while終わり

 } //file()終わり