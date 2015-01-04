//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　ファイル処理                                    　　　**//
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

void file(alldata *p){   //画像ファイル選択関数
     
    DIR *dir;
    struct dirent *ds;          /* ディレクトリストリーム構造体 */
    char f_name_tbl[256][25];   //画像ファイル名文字列格納
	int stl;                   //画像ファイル名文字列長さ
	int x,i,Color,frg,k; 
	int mv;                  //移動量変更用
	
     i=1;
     Color = GetColor( 255 , 255 , 255 ) ;       
 
    dir=opendir(".\\");  //探索場所カレントデレクトリ

    for(ds = readdir(dir); ds != NULL; ds = readdir(dir)){
        //使える画像はBMP,JPEG,PNG,DDS,ARGB,TGA の６種類です。
	   
		//↓画像ファイル選別  
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
	
    }//for終わり
    closedir(dir);
    
    k=i-1;
    i=1;
    x=800;
    frg=1;
	while (1){
		mv = 10;//通常移動量に更新
		strcpy(p->g_name , f_name_tbl[i]);//ファイル名格納
		stl = strlen(p->g_name) - 4;
		if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi") ||
			!strcmp(p->g_name, "z_cam_ewc.bmp")){//動画時移動量更新
			mv = 20;
		}
		PauseMovieToGraph(p->mof);       //動画再生停止
		drawing_img(p, x, 0, 4);        //動画像描画関数(カメラ終了処理)
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