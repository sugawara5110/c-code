//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　Fileクラス内メソッド定義                          　　　**//
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
	strcpy(f_name_tbl[1], "./dat/img/nekokin.dat");//最初のファイル名格納
	strcpy(f_name_tbl[2], "./dat/img/z_cam_ewc.dat");//カメラ用のファイル名格納

	hFile = FindFirstFile("*.*", &fd);//探索場所カレントデレクトリ

	do{
		//使える画像はBMP,JPEG,PNG,DDS,ARGB,TGA の６種類です。

		//↓画像ファイル選別  
		if (strlen(fd.cFileName) > 100)continue;//文字の長さが100超えの場合スキップ
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

	} while (FindNextFile(hFile, &fd) && i < 256);//次のファイルのアドレス
	FindClose(hFile);                 //クローズ
	
	k = i - 1;
	i = i1 = 1;

}

char* File::file(Dx9Init *dx, MSG *msg){ //画像ファイル選択関数

	int x, flg;
	Menu menu;        //メニューオブジェクト生成
	ImageDraw *fdraw;//ファイル関数用

	x = 1600;
	flg = 1;
	while (1){
		fdraw = new ImageDraw(dx, f_name_tbl[i]);//ファイル関数用オブジェクト生成

		if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";//動画像描画関数

		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {	// PostQuitMessage()が呼ばれた
				delete fdraw;
				return "stop";
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		if (flg != 0){
			while (x != 0){

				dx->drawscreen();//描画

				if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";
				menu.mouse(dx, NULL, 1, 0); //マウス関数
				if (flg == 1)x -= 20;
				if (flg == 2)x += 20;
			} //while終わり

		} //if終わり

		do{
			if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg->message == WM_QUIT) {	// PostQuitMessage()が呼ばれた
					delete fdraw;
					return "stop";
				}
				else {
					// メッセージの翻訳とディスパッチ
					TranslateMessage(msg);
					DispatchMessage(msg);
				}
			}

			if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";
			flg = menu.mouse(dx, NULL, 1, 0); //マウス関数

			dx->drawscreen();//描画

			if (flg == 3){

				delete fdraw;         //終わったので破棄
				return f_name_tbl[i];//ファイル名決定関数抜け 
			}

		} while (flg == 0);//do～while終了

		if (flg != 0){
			while (x >= -1600 && x < 1600){

				dx->drawscreen();//描画

				if (fdraw->draw_img(dx, x, 0, 0) == -1) return "stop";
				menu.mouse(dx, NULL, 1, 0); //マウス関数
				if (flg == 1)x -= 20;
				if (flg == 2)x += 20;

			} //while終わり
			if (flg == 1)i = i%k + 1;
			if (flg == 2){ i = i - 1; if (i == 0)i = k; }

		} //if終わり
		if (flg == 1)x = 1600;
		if (flg == 2)x = -1600;
		delete fdraw;//次の画像に切り替わるので破棄
	}//while終わり

}//file()終わり

char* File::e_file(ImageDraw *draw, int f){

	if (f == 1)i1 = i1%k + 1;
	if (f == 2){ i1 = i1 - 1; if (i1 == 0)i1 = k; }

	if (draw->getmcf() == 2 && !strcmp(f_name_tbl[i1], "./dat/img/z_cam_ewc.dat")){//カメラ選択時

		if (f == 1)i1 = i1%k + 1;
		if (f == 2){ i1 = i1 - 1; if (i1 == 0)i1 = k; }
	}

	return f_name_tbl[i1];

}