//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　Menuクラス内メソッド定義                                **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Menu.h"
#include "Sound_.h"

// 頂点データのＦＶＦフォーマットの設定
#define MY_VERTEX_FVF2 ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

int Menu::xm;
int Menu::ym;
int Menu::clf;

int Menu::rectangle_button(Dx9Init *dx, int xm, int ym, float x, float y, char *str, int f, int fr){//ボタン用関数
	
	re[0].x = x;
	re[0].y = y;
	re[0].z = 0.0f;
	re[0].rhw = 0.0f;
	re[0].color = (0 << 16) + (0 << 8) + 100;
	re[0].tu = 0.0f;
	re[0].tv = 0.0f;

	re[1].x = x;
	re[1].y = y + 20;
	re[1].z = 0.0f;
	re[1].rhw = 0.0f;
	re[1].color = (0 << 16) + (0 << 8) + 50;
	re[1].tu = 0.0f;
	re[1].tv = 0.0f;

	re[2].x = x + 100;
	re[2].y = y + 20;
	re[2].z = 0.0f;
	re[2].rhw = 0.0f;
	re[2].color = (0 << 16) + (0 << 8) + 50;
	re[2].tu = 0.0f;
	re[2].tv = 0.0f;

	re[3].x = x + 100;
	re[3].y = y;
	re[3].z = 0.0f;
	re[3].rhw = 0.0f;
	re[3].color = (0 << 16) + (0 << 8) + 100;
	re[3].tu = 0.0f;
	re[3].tv = 0.0f;

	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y){//マウス座標==ボタン範囲内か？

		re[0].color = (0 << 16) + (0 << 8) + 255;

		re[1].color = (0 << 16) + (0 << 8) + 200;

		re[2].color = (0 << 16) + (0 << 8) + 200;

		re[3].color = (0 << 16) + (0 << 8) + 255;
	}
	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y && strcmp(str, " 3Dモード") == 0){ //3D回転パネル出現可否処理
		if (d3f == 0 && d3b == 0){ d3f = 1; d3b = 1; }
		if (d3f == 1 && d3b == 0){ d3f = 0; d3b = 1; }
	}
	else if (strcmp(str, " 3Dモード") == 0)d3b = 0;

	dx->pD3DDevice->BeginScene();
	// 頂点データのＦＶＦフォーマットを設定
	dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// 描画
	dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &re, sizeof(MY_VERTEX2));
	//文字
	RECT r = { (long)x, (long)y + 2, (long)x, (long)y + 2 };
	dx->pD3DFont->DrawText(NULL, str, -1, &r, DT_LEFT | DT_SINGLELINE | DT_NOCLIP, 0xffffffff);
	//テクスチャー
	dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	dx->pD3DDevice->EndScene();
	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y && clf == 1)//範囲内かつ左クリック
		return f;//ボタン範囲内クリック
	return fr;  //何も押してない場合元の数値を返す
}

int Menu::square_button(Dx9Init *dx, ImageDraw *draw, int xm, int ym, float x, float y, char *str, int fr) {//矢印用関数

	sq[0].x = x;
	sq[0].y = y;
	sq[0].z = 0.0f;
	sq[0].rhw = 0.0f;
	sq[0].color = (0 << 16) + (0 << 8) + 150;
	sq[0].tu = 0.0f;
	sq[0].tv = 0.0f;

	sq[1].x = x;
	sq[1].y = y + 50;
	sq[1].z = 0.0f;
	sq[1].rhw = 0.0f;
	sq[1].color = (0 << 16) + (0 << 8) + 100;
	sq[1].tu = 0.0f;
	sq[1].tv = 0.0f;

	sq[2].x = x + 50;
	sq[2].y = y + 50;
	sq[2].z = 0.0f;
	sq[2].rhw = 0.0f;
	sq[2].color = (0 << 16) + (0 << 8) + 100;
	sq[2].tu = 0.0f;
	sq[2].tv = 0.0f;

	sq[3].x = x + 50;
	sq[3].y = y;
	sq[3].z = 0.0f;
	sq[3].rhw = 0.0f;
	sq[3].color = (0 << 16) + (0 << 8) + 150;
	sq[3].tu = 0.0f;
	sq[3].tv = 0.0f;

	if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){//マウス座標==ボタン範囲内か？
		
		sq[0].color = (0 << 16) + (0 << 8) + 255;
		
		sq[1].color = (0 << 16) + (0 << 8) + 200;
		
		sq[2].color = (0 << 16) + (0 << 8) + 200;
		
		sq[3].color = (0 << 16) + (0 << 8) + 255;
	}
	dx->pD3DDevice->BeginScene();
	//テクスチャー
	dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	// 頂点データのＦＶＦフォーマットを設定
	dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
	// 描画
	dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &sq, sizeof(MY_VERTEX2));
	dx->pD3DDevice->EndScene();
	if (str == "LF"){//File関数用完成画像表示無し

		tr[0].x = x + 5;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 30;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 30;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->pD3DDevice->BeginScene();
		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//範囲内かつ左クリック
			return 1;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "RF"){//File関数用完成画像表示無し

		tr[0].x = x + 45;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 20;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 20;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->pD3DDevice->BeginScene();
		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//範囲内かつ左クリック
			return 2;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}
	
	if (str == "L"){

		tr[0].x = x + 5;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 30;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 30;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->pD3DDevice->BeginScene();
		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;//マウス座標==ボタン範囲内か？
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//範囲内かつ左クリック
			return 1;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "R"){

		tr[0].x = x + 45;
		tr[0].y = y + 25;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 20;
		tr[1].y = y + 5;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 20;
		tr[2].y = y + 45;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->pD3DDevice->BeginScene();
		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;//マウス座標==ボタン範囲内か？
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)//範囲内かつ左クリック
			return 2;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "U"){

		tr[0].x = x + 25;
		tr[0].y = y + 5;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 5;
		tr[1].y = y + 30;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 45;
		tr[2].y = y + 30;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->pD3DDevice->BeginScene();
		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)
			return 4;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "D"){

		tr[0].x = x + 25;
		tr[0].y = y + 45;
		tr[0].z = 0.0f;
		tr[0].rhw = 0.0f;
		tr[0].color = (150 << 16) + (150 << 8) + 150;
		tr[0].tu = 0.0f;
		tr[0].tv = 0.0f;

		tr[1].x = x + 5;
		tr[1].y = y + 20;
		tr[1].z = 0.0f;
		tr[1].rhw = 0.0f;
		tr[1].color = (150 << 16) + (150 << 8) + 150;
		tr[1].tu = 0.0f;
		tr[1].tv = 0.0f;

		tr[2].x = x + 45;
		tr[2].y = y + 20;
		tr[2].z = 0.0f;
		tr[2].rhw = 0.0f;
		tr[2].color = (150 << 16) + (150 << 8) + 150;
		tr[2].tu = 0.0f;
		tr[2].tv = 0.0f;

		dx->pD3DDevice->BeginScene();
		//テクスチャー
		dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
		// 頂点データのＦＶＦフォーマットを設定
		dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
		// 描画
		dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 1, &tr, sizeof(MY_VERTEX2));
		dx->pD3DDevice->EndScene();

		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)fin_g = 1;
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && clf == 1)
			return 3;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	return fr;
}

int Menu::mouse(Dx9Init *dx, ImageDraw *draw, int f, int *offset){//fで処理操作,offsetで画像全体移動

	int fr = 0;   //キー決定
	int d3r = 0; //3D回転パネルキー決定
	static int mf = 0; //左クリック履歴

	if (f == 1){                       //マウス処理1 
		fr = square_button(dx, draw, xm, ym, 0, 0, "LF", fr);
		fr = square_button(dx, draw, xm, ym, 60, 0, "RF", fr);
		fr = rectangle_button(dx, xm, ym, 5, 60, "  画像決定", 3, fr);

	}

	if (f == 2){                  //マウス処理2
		fr = rectangle_button(dx, xm, ym, 0, 20, "   16個", 1, fr);
		fr = rectangle_button(dx, xm, ym, 110, 20, "   64個", 2, fr);
		fr = rectangle_button(dx, xm, ym, 220, 20, "   256個", 3, fr);
		fr = rectangle_button(dx, xm, ym, 330, 20, "  1600個", 4, fr);
		fr = rectangle_button(dx, xm, ym, 440, 20, "  10000個", 5, fr);
		fr = rectangle_button(dx, xm, ym, 550, 20, " 160000個", 6, fr);

	}

	if (f == 3){  //マウス処理3
		draw->finish = 0; //画像完成フラグ初期化
		fin_g = 0;       //画像完成フラグ初期化

		fr = square_button(dx, draw, xm, ym, 0, 50, "L", fr);
		fr = square_button(dx, draw, xm, ym, 100, 50, "R", fr);
		fr = square_button(dx, draw, xm, ym, 50, 0, "U", fr);
		fr = square_button(dx, draw, xm, ym, 50, 100, "D", fr);
		fr = rectangle_button(dx, xm, ym, 170, 50, "   元通り", 5, fr);
		fr = rectangle_button(dx, xm, ym, 290, 50, " シャッフル", 6, fr);
		fr = rectangle_button(dx, xm, ym, 410, 50, "   お手本", 7, fr);
		fr = rectangle_button(dx, xm, ym, 530, 50, "  個数変更", 8, fr);
		fr = rectangle_button(dx, xm, ym, 650, 50, "  画像変更", 9, fr);
		fr = rectangle_button(dx, xm, ym, 10, 160, " 通常画像", 10, fr);
		fr = rectangle_button(dx, xm, ym, 10, 190, " モノクロ", 11, fr);
		fr = rectangle_button(dx, xm, ym, 10, 220, " モザイク", 12, fr);
		fr = rectangle_button(dx, xm, ym, 410, 75, "  手数計算", 13, fr);
		fr = rectangle_button(dx, xm, ym, 10, 250, " エッジ検出", 14, fr);
		fr = rectangle_button(dx, xm, ym, 10, 280, " エンボス", 15, fr);
		fr = rectangle_button(dx, xm, ym, 10, 310, "  絵画風", 16, fr);
		fr = rectangle_button(dx, xm, ym, 10, 340, " 顔面検出", 17, fr);
		fr = rectangle_button(dx, xm, ym, 10, 370, "顔面モザイク", 18, fr);
		fr = rectangle_button(dx, xm, ym, 10, 400, " ネガポジ", 19, fr);
		fr = rectangle_button(dx, xm, ym, 10, 430, "画像エンボス", 20, fr);
		fr = rectangle_button(dx, xm, ym, 10, 460, "顔すげ替え", 21, fr);
		fr = rectangle_button(dx, xm, ym, 650, 75, " 3Dモード", 22, fr);

		if (d3f == 1){//3D画像回転

			d3r = square_button(dx, draw, xm, ym, 150, 200, "LF", d3r);
			d3r = square_button(dx, draw, xm, ym, 600, 200, "RF", d3r);
			draw->theta(d3r, 10);//カメラ角度操作
		}

		if (draw->d.mcf == 1){//動画ファイル時のみ実行
			//シークバー
			re[0].x = 200;
			re[0].y = 500;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (0 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 530;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (0 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 599;
			re[2].y = 530;
			re[2].z = 0.0f;
			re[2].rhw = 0.0f;
			re[2].color = (0 << 16) + (0 << 8) + 255;
			re[2].tu = 1.0f;
			re[2].tv = 1.0f;

			re[3].x = 599;
			re[3].y = 500;
			re[3].z = 0.0f;
			re[3].rhw = 0.0f;
			re[3].color = (0 << 16) + (0 << 8) + 155;
			re[3].tu = 1.0f;
			re[3].tv = 0.0f;

			dx->pD3DDevice->BeginScene();
			//テクスチャー
			dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			// 頂点データのＦＶＦフォーマットを設定
			dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
			// 描画
			dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &re, sizeof(MY_VERTEX2));
			dx->pD3DDevice->EndScene();

			re[0].x = 200;
			re[0].y = 500;
			re[0].z = 0.0f;
			re[0].rhw = 0.0f;
			re[0].color = (200 << 16) + (0 << 8) + 155;
			re[0].tu = 0.0f;
			re[0].tv = 0.0f;

			re[1].x = 200;
			re[1].y = 530;
			re[1].z = 0.0f;
			re[1].rhw = 0.0f;
			re[1].color = (200 << 16) + (0 << 8) + 255;
			re[1].tu = 0.0f;
			re[1].tv = 1.0f;

			re[2].x = 200 + ((float)draw->d.time1 / (float)draw->d.time2) * 399.0f;
			re[2].y = 530;
			re[2].z = 0.0f;
			re[2].rhw = 0.0f;
			re[2].color = (200 << 16) + (0 << 8) + 255;
			re[2].tu = 1.0f;
			re[2].tv = 1.0f;

			re[3].x = 200 + ((float)draw->d.time1 / (float)draw->d.time2) * 399.0f;
			re[3].y = 500;
			re[3].z = 0.0f;
			re[3].rhw = 0.0f;
			re[3].color = (200 << 16) + (0 << 8) + 155;
			re[3].tu = 1.0f;
			re[3].tv = 0.0f;

			dx->pD3DDevice->BeginScene();
			//テクスチャー
			dx->pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			// 頂点データのＦＶＦフォーマットを設定
			dx->pD3DDevice->SetFVF(MY_VERTEX_FVF2);
			// 描画
			dx->pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, &re, sizeof(MY_VERTEX2));
			dx->pD3DDevice->EndScene();

			//シークバー内にマウスカーソル有
			if (xm >= 200 && xm <= 599 && ym >= 500 && ym <= 530){

				if (clf)//左クリック
					//再生位置変更
					draw->d.pMediaPosition->put_CurrentPosition(draw->d.time2 * (xm - 200) / 399);
			}
		}

		if (fin_g == 1)draw->finish = 1;

	}

	if (f == 4){                 //マウス処理4
		fr = rectangle_button(dx, xm, ym, 100, 20, "ハイスピード", 1, fr);
		fr = rectangle_button(dx, xm, ym, 220, 20, "   中止", 2, fr);
		fr = rectangle_button(dx, xm, ym, 340, 20, " 猛スピード", 3, fr);

	}

	if (f == 5){   //マウス処理5,手数計算
		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && clf == 1)fr = 1;//画像内を差し,かつ左クリック
	}

	if (f == 6){//マウス処理6,画像エンボス
		square_button(dx, draw, xm, ym, 0, 50, "L", fr);   //draw->finish = 1を取得の為
		square_button(dx, draw, xm, ym, 100, 50, "R", fr);//draw->finish = 1を取得の為
		square_button(dx, draw, xm, ym, 50, 0, "U", fr);   //draw->finish = 1を取得の為
		square_button(dx, draw, xm, ym, 50, 100, "D", fr);//draw->finish = 1を取得の為
		fr = square_button(dx, draw, xm, ym, 150, 500, "LF", fr);
		fr = square_button(dx, draw, xm, ym, 600, 500, "RF", fr);

	}

	if (sf == 1 && fr != 0 && f != 1 && mf == 0)sound->sound();//ボタン押されたら鳴らす

	//ボタン押しっぱなしの場合の処理
	if (fr == 0){ mf = 0; return 0; }
	if (f == 1 && mf == 1)return 0;
	if (f == 3 && mf == 1 && fr > 4)return 0;
	mf = 1;

	return fr;
}

Menu::Menu(){

	d3f = 0;
	d3b = 0;
	sf = 1;//効果音有
	sound = new Sound_();
}

Menu::Menu(int s){
	sf = 0;//効果音無し,オブジェクト生成無し
}

Menu::~Menu(){
	if (sf){//効果音有の時のみ実行
		delete sound;
		sound = NULL;
	}
}