//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　マウス座標処理                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "struct_set.h"
                    
int rectangle_button(int xm,int ym, int bt2on,int bt2of,int x,int y,char *str,int f,int fr){//ボタン用関数
	DrawGraph(x, y, bt2on, TRUE);//押してないボタン描画
	if (xm >= x && xm < 100+x && ym >= y && ym < 20+y)DrawGraph(x, y, bt2of, TRUE);//マウス座標==ボタン範囲内か？
	DrawFormatString(x, y+2, GetColor(10, 10, 10) , str);
	if (xm >= x && xm < 100+x && ym >= y && ym < 20+y && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
		return f;//ボタン範囲内クリック
	return fr;  //何も押してない場合元の数値を返す
}

int mouse(alldata *p,int f,int *offset){//fで処理操作,offsetで画像全体移動
	
	static int bt1on, bt1of, bt2on, bt2of;   //ボタン画像ハンドル
	int i, j;   //for用
	int fr = 0;//キー決定
	int cr,crs;    //記号色
	int xm, ym;   //マウス座標
	static int xof, yof;//左クリック時座標,マウス処理4で使用
	static int mf;     //左クリック履歴,マウス処理4で使用
	
	if (f != 4)mf = 0;//画像オフセット処理初期化
	cr = GetColor(0, 0, 255);
	crs = GetColor(10, 10, 10);
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

		return 0;
	}

	if (f == 1){                       //マウス処理1 
		GetMousePoint(&xm, &ym);      //マウス座標取得                
		DrawGraph(0, 0, bt1on, TRUE);//押してないボタン描画,矢印左
		if (xm >= 0 && xm < 50 && ym >= 0 && ym < 50)DrawGraph(0, 0, bt1of, TRUE);//マウス座標==ボタン範囲内か？
		if (xm >= 0 && xm < 50 && ym >= 0 && ym < 50 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
			fr = 1;//返り値
		DrawGraph(60, 0, bt1on, TRUE);//押してないボタン描画,矢印右
		if (xm >= 60 && xm < 110 && ym >= 0 && ym < 50)DrawGraph(60, 0, bt1of, TRUE);//マウス座標==ボタン範囲内か？
		if (xm >= 60 && xm < 110 && ym >= 0 && ym < 50 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//範囲内かつ左クリック
			fr = 2;//返り値
		DrawTriangle(5, 25, 30, 5, 30, 45, cr, TRUE);   //矢印図形描画,左
		DrawTriangle(105, 25, 80, 5, 80, 45, cr, TRUE); //矢印図形描画,右
		fr = rectangle_button(xm, ym, bt2on, bt2of, 5, 60, "  画像決定", 3, fr);
        
		return fr;
	}

	if (f == 2){                  //マウス処理2
		GetMousePoint(&xm, &ym);  //マウス座標取得
		fr = rectangle_button(xm, ym, bt2on, bt2of, 0, 20,   "   16個", 1, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 110, 20, "   64個", 2, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 220, 20, "   256個", 3, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 330, 20, "  1600個", 4, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 440, 20, "  10000個", 5, fr);
		fr = rectangle_button(xm, ym, bt2on, bt2of, 550, 20, " 160000個", 6, fr);

		return fr;
	}

	if (f == 3){                 //マウス処理3
		GetMousePoint(&xm, &ym);                       
		DrawGraph(0, 50, bt1on, TRUE);//矢印左
		if (xm >= 0 && xm < 50 && ym >= 50 && ym < 100)DrawGraph(0, 50, bt1of, TRUE);
		if (xm >= 0 && xm < 50 && ym >= 50 && ym < 100 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 1;
		DrawGraph(100, 50, bt1on, TRUE);//矢印右
		if (xm >= 100 && xm < 150 && ym >= 50 && ym < 100)DrawGraph(100, 50, bt1of, TRUE);
		if (xm >= 100 && xm < 150 && ym >= 50 && ym < 100 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 2;
		DrawGraph(50, 0, bt1on, TRUE);//矢印上
		if (xm >= 50 && xm < 100 && ym >= 0 && ym < 50)DrawGraph(50, 0, bt1of, TRUE);
		if (xm >= 50 && xm < 100 && ym >= 0 && ym < 50 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 3;
		DrawGraph(50, 100, bt1on, TRUE);//矢印下
		if (xm >= 50 && xm < 100 && ym >= 100 && ym < 150)DrawGraph(50, 100, bt1of, TRUE);
		if (xm >= 50 && xm < 100 && ym >= 100 && ym < 150 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
			fr = 4;
		DrawTriangle(5, 75, 30, 55, 30, 95, cr, TRUE);       //矢印左図形描画
		DrawTriangle(145, 75, 120, 55, 120, 95, cr, TRUE);   //矢印右図形描画
		DrawTriangle(75, 5, 55, 30, 95, 30, cr, TRUE);       //矢印上図形描画
		DrawTriangle(75, 145, 55, 120, 95, 120, cr, TRUE);   //矢印下図形描画
		
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
		fr = rectangle_button(xm, ym, bt2on, bt2of, 10, 360, " 水彩画風", 16, fr);
		
		return fr;
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
		
			return fr;
	}

	if (f == 5){   //マウス処理5,手数計算
		GetMousePoint(&xm, &ym);
		if (xm >= 200 && xm < 600 && ym >= 100 && ym < 500 && (GetMouseInput() & MOUSE_INPUT_LEFT) != 0)fr = 1;//画像内を差し,かつ左クリック
		return fr;
	}
	return 0;
}