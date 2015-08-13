//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　Moveクラス内メソッド定義(自動)                        **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Menu.h"

int Move::auto_space_mov(Dx9Init *dx, MSG *msg, ImageDraw *draw, int *x, int *y){//スペースを目的ブロックに隣接するまで自動移動,cnt 移動禁止フラグ,x,y目標ブロック

	para *prs = &paras[size];
	int *sp = space;
	int f;//mov()結果
	int s = prs->bsize;
	int ret = 0;//return 1 判別用

	while (1){
		if ((sp[0] == *x && sp[1] == *y - s) || (sp[0] == *x + s && sp[1] == *y - s) || //目的地に隣接
			(sp[0] == *x + s && sp[1] == *y) || (sp[0] == *x + s && sp[1] == *y + s) ||
			(sp[0] == *x && sp[1] == *y + s) || (sp[0] == *x - s && sp[1] == *y + s) ||
			(sp[0] == *x - s && sp[1] == *y) || (sp[0] == *x - s && sp[1] == *y - s))break;
		
		if (sp[0] - *x > 0)f = mov(dx, msg, draw, 2, 1);//スペース右位置関係別に引数を変えmov関数を呼び出す
		if (sp[0] - *x > 0 && f == 0){ mov(dx, msg, draw, 3, 1); continue; }
		if (sp[0] - *x < 0)f = mov(dx, msg, draw, 1, 1);//スペース左
		if (sp[1] - *y > 0)f = mov(dx, msg, draw, 4, 1);//スペース下 
		if (sp[1] - *y < 0)f = mov(dx, msg, draw, 3, 1);//スペース上
		if (br)return 0;//アプリ終了
		if (f == 2){ ret = 1; break; }
	}//while終わり
	if (ret == 1)return 1;//auto_matic関数抜け用
	return 0;
}//auto_space_mov終わり




int Move::auto_round(Dx9Init *dx, MSG *msg, ImageDraw *draw, int *x, int *y, int cx, int cy){ //スペース回転,ブロック移動関数,cnt移動禁止フラグ,*x*y移動ブロック,cx,cy移動先

	para *prs = &paras[size];
	int *sp = space;
	int s = prs->bsize;
	int fr = 0;   //回転方向 0:時計 0以外:反時計 movp==0(移動未実施)の度切り替わる
	int movp = 0;//回転方向切換フラグ
	int ret = 0;//return 1 判別用

	if (auto_space_mov(dx, msg, draw, x, y))return 1;
	if (br)return 0;//アプリ終了
	while (*x != cx || *y != cy){
		if (sp[0] == *x && sp[1] == *y - s){       //スペース上
			if (*y - cy > 0){ mov(dx, msg, draw, 3, 1); continue; }//目的地上方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 1, 1); else movp = mov(dx, msg, draw, 2, 1); //回転方向分岐,右,左
		}
		if (sp[0] == *x + s && sp[1] == *y - s){     //スペース右上
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 3, 1); else movp = mov(dx, msg, draw, 2, 1); //回転方向分岐,下,左
		}
		if (sp[0] == *x + s && sp[1] == *y){       //スペース右
			if (*x - cx < 0){ mov(dx, msg, draw, 2, 1); continue; }//目的地右方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 3, 1); else movp = mov(dx, msg, draw, 4, 1); //回転方向分岐,下,上
		}
		if (sp[0] == *x + s && sp[1] == *y + s){     //スペース右下
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 2, 1); else movp = mov(dx, msg, draw, 4, 1); //回転方向分岐,左,上
		}
		if (sp[0] == *x && sp[1] == *y + s){       //スペース下
			if (*y - cy < 0){ mov(dx, msg, draw, 4, 1); continue; }//目的地下方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 2, 1); else movp = mov(dx, msg, draw, 1, 1); //回転方向分岐,左,右
		}
		if (sp[0] == *x - s && sp[1] == *y + s){     //スペース左下
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 4, 1); else movp = mov(dx, msg, draw, 1, 1); //回転方向分岐,上,右
		}
		if (sp[0] == *x - s && sp[1] == *y){       //スペース左
			if (*x - cx > 0){ mov(dx, msg, draw, 1, 1); continue; }//目的地左方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 4, 1); else movp = mov(dx, msg, draw, 3, 1); //回転方向分岐,上,下
		}
		if (sp[0] == *x - s && sp[1] == *y - s){     //スペース左上
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(dx, msg, draw, 1, 1); else movp = mov(dx, msg, draw, 3, 1); //回転方向分岐,右,下
		}
		if (br)return 0;//アプリ終了
		if (movp == 2){ ret = 1; break; }

	} //while終わり

	if (ret == 1)return 1;//auto_matic関数抜け用
	return 0;

}//auto_round()終わり


int Move::auto_matic(Dx9Init *dx, MSG *msg, ImageDraw *draw, int range, int j){    //オート関数宣言 cnt移動禁止フラグ,pcs縦横個数,range対象範囲,jfor文範囲

	para *prs = &paras[size];
	int pcs = paras[size].pcs;
	int *sp = space;
	int i; //for用
	int ret = 0;//return 判別用

	if (range == NULL){ //初期化
		range = pcs;
		cnt = (int*)malloc(sizeof(int)*prs->idx);
		for (i = 0; i < prs->idx; i++){
			cnt[i] = 0; //mov()関数内の移動禁止フラグ
		}
	}

	while (1){//上1列処理ループ

		//上1列完成判別
		for (i = j; i < range + j; i++){//上1列チェック
			if (img[i].cx != img[i].fx || img[i].cy != img[i].fy)break;//値が一致の間繰り返し
		}
		if (i == range + j){//全部一致したら移動禁止処理,break
			for (i = j; i < range + j; i++){ cnt[i] = 1; }
			if (range == 2){ mov(dx, msg, draw, 1, 1); mov(dx, msg, draw, 1, 1); cnt_results = 0; ret = 1; break; }//一致の場合この処理で終了
			break; //一致した場合while抜け
		}
		if (br)return -1;//アプリ終了
		//↓範囲内上1列1回目処理 
		for (i = j + 1; i < range + j; i++){
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i - 1].fx, img[i - 1].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;	//移動禁止ON
			if (br)return -1;//アプリ終了
		} //for終わり
		if (ret == 1)break;//上のfor内ブレークの場合while抜け

		//↓移動不可状態処理
		if ((img[j].cx == img[range + j - 1].fx && img[j].cy == img[range + j - 1].fy) ||
			(img[j].cx == img[range + j - 1 + pcs].fx && img[j].cy == img[range + j - 1 + pcs].fy &&
			sp[0] == img[range + j - 1].fx && sp[1] == img[range + j - 1].fy)){
			for (i = j + 1; i < range + j; i++){
				cnt[i] = 0;
			}
			if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[range + j - 1 + pcs * 2].fx, img[range + j - 1 + pcs * 2].fy)){
				ret = 1; break;//中止の場合while抜け
			}
			if (br)return -1;//アプリ終了
			continue;//移動不可状態処理後最初に戻る
		}

		//↓範囲内上1列左1個処理
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j + pcs].fx, img[j + pcs].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;  //移動禁止ON
		if (br)return -1;//アプリ終了
		//↓範囲内上1列2回目処理
		for (i = range + j - 1; i >= j + 1; i--){
			cnt[i] = 0;        //移動前移動禁止OFF
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;        //移動後移動禁止ON
			if (br)return -1;//アプリ終了
		}//for終わり
		if (ret == 1)break;//上のfor内breakの場合while抜け

		if (range == 2){ cnt[j] = 0; mov(dx, msg, draw, 3, 1); mov(dx, msg, draw, 1, 1); mov(dx, msg, draw, 1, 1); cnt_results = 0; ret = 1; break; }//完了

		//↓範囲内上1列左1個処理
		cnt[j] = 0;            //移動前移動禁止OFF
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;            //移動後移動禁止ON
		if (br)return -1;//アプリ終了
		break;//while抜け
	}//上1列処理ループ終わり
	if (ret == 1)return 0;//while内breakの場合関数終了

	j += pcs;         //for文初期値更新
	range--;         //範囲更新

	while (1){//左1列処理ループ

		//左1列完成判別
		for (i = j; i <= j + pcs*(range - 1); i += pcs){
			if (img[i].cx != img[i].fx || img[i].cy != img[i].fy)break;//値が一致の間繰り返し
		}
		if (i > j + pcs*(range - 1)){ //全部一致したら移動禁止処理,break
			for (i = j; i <= j + pcs*(range - 1); i += pcs){ cnt[i] = 1; }
			break;//一致した場合while抜け
		}
		if (br)return -1;//アプリ終了
		//↓範囲内左1列1回目処理
		for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i - pcs].fx, img[i - pcs].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;  //移動禁止ON
			if (br)return -1;//アプリ終了
		} //for終わり
		if (ret == 1)break;//上for内breakの場合while抜け

		//↓移動不可状態処理
		if ((img[j].cx == img[j + pcs*(range - 1)].fx && img[j].cy == img[j + pcs*(range - 1)].fy) ||
			(img[j].cx == img[j + pcs*(range - 1) + 1].fx && img[j].cy == img[j + pcs*(range - 1) + 1].fy &&
			sp[0] == img[j + pcs*(range - 1)].fx && sp[1] == img[j + pcs*(range - 1)].fy)){
			for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
				cnt[i] = 0;
			}
			if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j + pcs*(range - 1) + 2].fx, img[j + pcs*(range - 1) + 2].fy)){
				ret = 1; break;//中止の場合while抜け
			}
			if (br)return -1;//アプリ終了
			continue;//移動不可状態処理後最初に戻る
		}

		//↓範囲内左1列上1個処理
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j + 1].fx, img[j + 1].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;         //移動禁止ON
		if (br)return -1;//アプリ終了
		//↓範囲内左1列2回目処理
		for (i = j + pcs*(range - 1); i >= j + pcs; i -= pcs){
			cnt[i] = 0;   //移動禁止OFF
			if (auto_round(dx, msg, draw, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;   //移動禁止ON
			if (br)return -1;//アプリ終了
		}//for終わり
		if (ret == 1)break;//上for内breakの場合while抜け

		//↓範囲内左1列上1個処理
		cnt[j] = 0;  //移動禁止OFF
		if (auto_round(dx, msg, draw, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;  //移動禁止ON
		if (br)return -1;//アプリ終了
		break;//while抜け
	}//左1列処理ループ終わり
	if (br)return -1;//アプリ終了
	if (ret == 1)return 0;//while内breakの場合関数終了

	j++; //for文初期値変更
	//↓再帰呼び出し  
	auto_matic(dx, msg, draw, range, j);
	return 0;
}//auto_matic()終わり

int Move::count(Dx9Init *dx, MSG *msg, ImageDraw *draw){//手数計算

	Menu menu;//一時的に使う
	int i;
	int sm[2];//手数計算処理前スペース座標退避用
	int msgf = 0;   //アプリ終了ループ抜けフラグ

	tkf = 1; tkc = 0;//手数計算実行フラグ,カウント初期化
	for (i = 0; i < paras[size].idx; i++){
		img[i].chx = img[i].cx;//手数計算前座標値退避
		img[i].chy = img[i].cy;//手数計算前座標値退避
	}
	sm[0] = space[0];//手数計算前座標値退避
	sm[1] = space[1];//手数計算前座標値退避
	dx->text("画像内クリックで終了", 305, 522);//テキスト描画
	dx->drawscreen();//描画
	if (auto_matic(dx, msg, draw, NULL, 0) == -1)return -1;//auto_matic関数処理

	for (i = 0; i < paras[size].idx; i++){
		img[i].cx = img[i].chx;//手数計算後座標値戻し
		img[i].cy = img[i].chy;//手数計算後座標値戻し
	}
	space[0] = sm[0];//手数計算後座標値戻し
	space[1] = sm[1];//手数計算後座標値戻し
	if (cnt_results != -1){//-1だと中止してる
		char str[30];
		sprintf(str, "%d手で完了します", tkc);
		dx->text(str, 305, 502);//テキスト描画
		dx->text("画像内クリックで終了", 305, 522);//テキスト描画
		dx->drawscreen();//描画

		while (menu.mouse(dx, draw, 5, 0) != 1){//クリック待ち
			if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg->message == WM_QUIT) {	// PostQuitMessage()が呼ばれた
					msgf = 1;
					break;	//ループの終了
				}
				else {
					// メッセージの翻訳とディスパッチ
					TranslateMessage(msg);
					DispatchMessage(msg);
				}
			}
		}
		if (msgf == 1)return -1;
	}//if終わり
	tkf = 0; tkc = 0;//フラグ,カウント初期化
	return 0;
}