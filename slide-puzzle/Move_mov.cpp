//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　Moveクラス内メソッド定義(移動処理)　　　　　          **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Menu.h"

int Move::mov(Dx9Init *dx, MSG *msg, ImageDraw *draw, int flg, int autof){  //移動処理関数宣言 cnt移動禁止フラグ,flg移動方向,autof=1:auto_matic関数実行中

	int i;             //for
	int k = -1;       //初期化
	int mv;          //移動量変更用
	int ms = 0;     //猛スピードモードフラグ
	int offset[2]; //画像座標オフセット
	para *prs = &paras[size];
	int *sp = space;
	Menu menu(0);//メニューオブジェクト生成

	offset[0] = 0, offset[1] = 0;//初期化

	for (i = 0; i < prs->idx; i++){
		//↓auto_matic()関数のブロック衝突判定処理
		if (autof == 1 && cnt[i] == 1){ k = -1; continue; }
		//↓移動キー毎に全ブロックとスペースの座標比較,移動対象ブロック決定(添え字k)
		if (flg == 1 && img[i].cx == sp[0] + prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (flg == 2 && img[i].cx == sp[0] - prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (flg == 3 && img[i].cx == sp[0] && img[i].cy == sp[1] + prs->bsize){ k = i; break; }
		if (flg == 4 && img[i].cx == sp[0] && img[i].cy == sp[1] - prs->bsize){ k = i; break; }

	}  //for終わり

	if (k >= 0){             //移動対象ブロック決定時のみ実行
		if (tkf != 1){      //手数計算フラグ1以外に実行
			mv = prs->move;//通常移動量初期化

			if (autof == 1 && menu.mouse(dx, draw, 4, offset) == 1)mv = prs->bsize;//automatic関数実行時ハイスピード
			if (autof == 1 && menu.mouse(dx, draw, 4, offset) == 2)return 2;      //automatic関数中止
			if (autof == 1 && menu.mouse(dx, draw, 4, offset) == 3){ mv = prs->bsize; ms = 1; }//猛スピードオン
		}
		else{//手数計算フラグ1の時実行
			tkc++;//手数カウント
			mv = prs->bsize;//猛スピード
			if (menu.mouse(dx, draw, 5, 0) == 1){ cnt_results = -1; return 2; }//手数計算結果,手数計算中止
		}

		while (img[k].cx != sp[0] || img[k].cy != sp[1]){ //移動中座標更新処理,スペース座標に到達したら終了

			if (flg == 1)img[k].cx -= mv;//座標値更新 ブロック左移動
			if (flg == 2)img[k].cx += mv;//座標値更新 ブロック右移動
			if (flg == 3)img[k].cy -= mv;//座標値更新 ブロック下移動
			if (flg == 4)img[k].cy += mv;//座標値更新 ブロック上移動

			if (ms == 0 && tkf == 0) {//猛スピードoff時,手数計算以外時描写
				if (draw->draw_img(dx, this, offset[0], offset[1], 0) == -1)return -1;//画像描画
				if (autof == 0){ menu.mouse(dx, draw, 3, 0); if (draw->getgfl() == 9)menu.mouse(dx, draw, 6, 0); }
				else menu.mouse(dx, draw, 4, offset);//マウス座像描画のみ

				dx->drawscreen();//描画

			}
		}//while終了

		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				br = 1;//アプリ終了
				return -1;//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		//↓移動完了後スペース座標更新
		if (flg == 1)sp[0] += prs->bsize;
		if (flg == 2)sp[0] -= prs->bsize;
		if (flg == 3)sp[1] += prs->bsize;
		if (flg == 4)sp[1] -= prs->bsize;

	} //if終わり
	if (k >= 0)return 1;

	return 0;
} //mov()終わり

