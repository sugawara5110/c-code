//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　Menuクラス内メソッド定義                                **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Menu.h"
                    
int Menu::rectangle_button(int xm, int ym, int bt2on, int bt2of, int x, int y, char *str, int f, int fr){//ボタン用関数
	DrawGraph(x, y, bt2on, TRUE);//押してないボタン描画
	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y)DrawGraph(x, y, bt2of, TRUE);//マウス座標==ボタン範囲内か？
	DrawFormatString(x, y + 2, GetColor(10, 10, 10), str);
	if (xm >= x && xm < 100 + x && ym >= y && ym < 20 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
		return f;//ボタン範囲内クリック
	return fr;  //何も押してない場合元の数値を返す
}

int Menu::square_button(ImageDraw *draw, int xm, int ym, int bt1on, int bt1of, int x, int y, char *str, int fr) {//矢印用関数

	int cr;//記号色
	cr = GetColor(0, 0, 255);
	
	if (str == "LF"){//File関数用完成画像表示無し

		DrawGraph(x, y, bt1on, TRUE);//押してないボタン描画,矢印左
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)DrawGraph(x, y, bt1of, TRUE);//マウス座標==ボタン範囲内か？
		DrawTriangle(5 + x, 25 + y, 30 + x, 5 + y, 30 + x, 45 + y, cr, TRUE);   //矢印図形描画,左
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
			return 1;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "RF"){//File関数用完成画像表示無し

		DrawGraph(x, y, bt1on, TRUE);//押してないボタン描画,矢印右
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y)DrawGraph(x, y, bt1of, TRUE);//マウス座標==ボタン範囲内か？
		DrawTriangle(45 + x, 25 + y, 20 + x, 5 + y, 20 + x, 45 + y, cr, TRUE); //矢印図形描画,右
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
			return 2;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "L"){

		DrawGraph(x, y, bt1on, TRUE);//押してないボタン描画,矢印左
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }//マウス座標==ボタン範囲内か？
		DrawTriangle(5 + x, 25 + y, 30 + x, 5 + y, 30 + x, 45 + y, cr, TRUE);   //矢印図形描画,左
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
			return 1;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "R"){

		DrawGraph(x, y, bt1on, TRUE);//押してないボタン描画,矢印右
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }//マウス座標==ボタン範囲内か？
		DrawTriangle(45 + x, 25 + y, 20 + x, 5 + y, 20 + x, 45 + y, cr, TRUE); //矢印図形描画,右
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
			return 2;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "U"){

		DrawGraph(x, y, bt1on, TRUE);//矢印上
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }
		DrawTriangle(25 + x, 5 + y, 5 + x, 30 + y, 45 + x, 30 + y, cr, TRUE);       //矢印上図形描画
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			return 3;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	if (str == "D"){

		DrawGraph(x, y, bt1on, TRUE);//矢印下
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y){ DrawGraph(x, y, bt1of, TRUE); fin_g = 1; }
		DrawTriangle(25 + x, 45 + y, 5 + x, 20 + y, 45 + x, 20 + y, cr, TRUE);   //矢印下図形描画
		if (xm >= x && xm < 50 + x && ym >= y && ym < 50 + y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			return 4;//ボタン範囲内クリック
		return fr;//非クリック時元の数値を返す
	}

	return fr;
}

int Menu::mouse(ImageDraw *draw, int f, int *offset){//fで処理操作,offsetで画像全体移動

	static int bt1on, bt1of, bt2on, bt2of;   //ボタン画像ハンドル
	int i, j;   //for用
	int fr = 0;//キー決定
	int cr;    //記号色
	int xm, ym;   //マウス座標
	static int xof, yof;//左クリック時座標,マウス処理4で使用
	static int mf;     //左クリック履歴,マウス処理4で使用
	cr = GetColor(0, 0, 255);

	if (f != 4)mf = 0;//画像オフセット処理初期化

	if (f == 0){//初期処理

		bt1on = MakeXRGB8ColorSoftImage(50, 50);//ハンドルにカラ画像設定
		bt1of = MakeXRGB8ColorSoftImage(50, 50);
		bt2on = MakeXRGB8ColorSoftImage(100, 20);
		bt2of = MakeXRGB8ColorSoftImage(100, 20);

		for (i = 0; i < 50; i++){                    //ボタン生成1
			for (j = 0; j < 50; j++){

				DrawPixelSoftImage(bt1on, j, i, 185, 185, 185, 0);  //押さない状態の色設定
				DrawPixelSoftImage(bt1of, j, i, 250, 250, 250, 0); //押してる状態の色設定
				if (j == 0 || j == 49 || i == 0 || i == 49){      //外周色設定1
					DrawPixelSoftImage(bt1on, j, i, 20, 20, 20, 0);
					DrawPixelSoftImage(bt1of, j, i, 90, 90, 90, 0);
				}
				if ((i != 0 && i != 49 && j != 0 && j != 49) && (j == 1 || j == 48 || i == 1 || i == 48)){//外周色設定2
					DrawPixelSoftImage(bt1on, j, i, 70, 70, 70, 0);
					DrawPixelSoftImage(bt1of, j, i, 140, 140, 140, 0);
				}
				if ((i >= 2 && i <= 47 && j >= 2 && j <= 47) && (j == 2 || j == 47 || i == 2 || i == 47)){//外周色設定3
					DrawPixelSoftImage(bt1on, j, i, 140, 140, 140, 0);
					DrawPixelSoftImage(bt1of, j, i, 210, 210, 210, 0);
				}
			}
		}

		for (i = 0; i < 20; i++){                        //ボタン生成2
			for (j = 0; j < 100; j++){

				DrawPixelSoftImage(bt2on, j, i, 180, 185, 185, 0);  //押さない状態の色設定
				DrawPixelSoftImage(bt2of, j, i, 250, 250, 250, 0); //押した状態の色設定
				if (j == 0 || j == 99 || i == 0 || i == 19){      //外周色設定1
					DrawPixelSoftImage(bt2on, j, i, 20, 20, 20, 0);
					DrawPixelSoftImage(bt2of, j, i, 90, 90, 90, 0);
				}
				if ((i != 0 && i != 19 && j != 0 && j != 99) && (j == 1 || j == 98 || i == 1 || i == 18)){//外周色設定2
					DrawPixelSoftImage(bt2on, j, i, 70, 70, 70, 0);
					DrawPixelSoftImage(bt2of, j, i, 140, 140, 140, 0);
				}
				if ((i >= 2 && i <= 17 && j >= 2 && j <= 97) && (j == 2 || j == 97 || i == 2 || i == 17)){//外周色設定3
					DrawPixelSoftImage(bt2on, j, i, 140, 140, 140, 0);
					DrawPixelSoftImage(bt2of, j, i, 210, 210, 210, 0);
				}
			}
		}

		bt1on = CreateGraphFromSoftImage(bt1on);//ボタン生成後ハンドルに格納
		bt1of = CreateGraphFromSoftImage(bt1of);
		bt2on = CreateGraphFromSoftImage(bt2on);
		bt2of = CreateGraphFromSoftImage(bt2of);

	}

	if (f == 1){                       //マウス処理1 
		GetMousePoint(&xm, &ym);      //マウス座標取得                
		fr = square_button(draw, xm, ym, bt1on, bt1of, 0, 0, "LF", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 60, 0, "RF", fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 5, 60, "  画像決定", 3, fr);

	}

	if (f == 2){                  //マウス処理2
		GetMousePoint(&xm, &ym);  //マウス座標取得
		fr = rectangle_button(xm, ym, bt2on, bt2of, 0, 20, "   16個", 1, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 110, 20, "   64個", 2, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 220, 20, "   256個", 3, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 330, 20, "  1600個", 4, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 440, 20, "  10000個", 5, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 550, 20, " 160000個", 6, fr);

	}

	if (f == 3){                 //マウス処理3
		draw->finish = 0;//画像完成フラグ初期化
		fin_g = 0;      //画像完成フラグ初期化

		GetMousePoint(&xm, &ym);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 0, 50, "L", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 100, 50, "R", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 50, 0, "U", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 50, 100, "D", fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 170, 50, "   元通り", 5, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 290, 50, " シャッフル", 6, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 410, 50, "   お手本", 7, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 530, 50, "  個数変更", 8, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 650, 50, "  画像変更", 9, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 160, " 通常画像", 10, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 200, " モノクロ", 11, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 240, " モザイク", 12, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 410, 75, "  手数計算", 13, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 280, " エッジ検出", 14, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 320, " エンボス", 15, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 360, "  絵画風", 16, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 400, " 顔面検出", 17, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 440, "顔面モザイク", 18, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 480, " ネガポジ", 19, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 520, "画像エンボス", 20, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 560, "顔すげ替え", 21, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 650, 75, " 3Dモード", 22, fr);

		if (fin_g == 1)draw->finish = 1;

	}

	if (f == 4){                 //マウス処理4
		GetMousePoint(&xm, &ym);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 100, 20, "ハイスピード", 1, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 220, 20, "   中止", 2, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 340, 20, " 猛スピード", 3, fr);

		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && mf == 0 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0){ //画像オフセット処理
			GetMousePoint(&xof, &yof); mf = 1;
		}
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) == 0){
			xof = yof = 0; mf = 0;
		}
		if (mf == 1 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0){ offset[0] = xm - xof; offset[1] = ym - yof; }//画像オフセット処理

	}

	if (f == 5){   //マウス処理5,手数計算
		GetMousePoint(&xm, &ym);
		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)fr = 1;//画像内を差し,かつ左クリック

	}

	if (f == 6){//マウス処理6,画像エンボス
		GetMousePoint(&xm, &ym);
		square_button(draw, xm, ym, bt1on, bt1of, 0, 50, "L", fr);   //draw->finish = 1を取得の為
		square_button(draw, xm, ym, bt1on, bt1of, 100, 50, "R", fr);//draw->finish = 1を取得の為
		square_button(draw, xm, ym, bt1on, bt1of, 50, 0, "U", fr);   //draw->finish = 1を取得の為
		square_button(draw, xm, ym, bt1on, bt1of, 50, 100, "D", fr);//draw->finish = 1を取得の為
		fr = square_button(draw, xm, ym, bt1on, bt1of, 150, 500, "LF", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 600, 500, "RF", fr);

	}

	if (f == 7){//マウス処理7,3D画像
		fr = square_button(draw, xm, ym, bt1on, bt1of, 150, 200, "LF", fr);
		fr = square_button(draw, xm, ym, bt1on, bt1of, 600, 200, "RF", fr);
	}

	return fr;
}