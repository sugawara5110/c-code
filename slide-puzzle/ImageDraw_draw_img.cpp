//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(描画)                         **//
//**                                    (ImageDrawの基底クラス)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"

int ImageDraw::draw_img(Dx9Init *dx, int x, int y, int z){//ファイル選択用画像描画
	if (read_img() == -1)return -1;                        //画像読み込み
	if (FAILED(f_vertex(dx, x, y, z)))return -1;          //ファイル選択用画像頂点処理
	return 0;
}

int ImageDraw::draw_img(Dx9Init *dx, Move *move, int x, int y, int z){//画像描画
	if (read_img() == -1)return -1;                               //画像読み込み
	if (FAILED(processing_img()))return -1;                     //画像変換
	if (FAILED(p_vertex(dx, move, x, y, z)))return -1;  //頂点処理
	return 0;
}
