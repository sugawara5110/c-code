//*****************************************************************************************//
//**                                                                                     **//
//**                   　Moveクラス内メソッド定義(ブロックシャッフル)                    **//
//**                                                                                     **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Filter.h"

int Move::change(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw){ //ブロック交換関数宣言

	int i;           //for用
	int cnt;        //移動完了ブロック数カウント 
	para *prs = &paras[size];
	int *sp = space;
	int mv;       //移動量変更用

	mv = prs->move;//通常ブロック移動量に初期化

	if (draw->d.mcf == 1 || draw->d.mcf == 2){//動画ブロック時,移動量に更新
		mv = prs->movem;           //動画ブロック時移動量更新
	}

	for (int i = 0; i < prs->idx; i++){//z方向移動先格納
		img[i].chz = (rand() % 600) - 300;
	}

	cnt = 0;
	while (cnt < prs->idx){//z方向飛び出し処理
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		cnt = 0;   //for文idx回終了時点でcnt規定値か否か判定する為for文開始前は初期化
		for (i = 0; i < prs->idx; i++){ //ブロック個数分繰り返し

			//z移動方向判別,cnt規定値になるまで処理
			if (img[i].cz == img[i].chz){ cnt++; continue; }    //到達後処理

			if ((img[i].chz - img[i].cz) < 0){                 //-方向移動後処理
				img[i].cz -= mv; if ((img[i].chz - img[i].cz) >= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}

			if ((img[i].chz - img[i].cz) > 0){                //+方向移動後処理
				img[i].cz += mv; if ((img[i].chz - img[i].cz) <= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}
		}

		draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//画像描画
		dx->drawscreen();//描画
	}//z方向飛び出し処理終了

	cnt = 0;
	while (cnt < prs->count){//xy移動処理

		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		cnt = 0;   //for文(idx-1)回終了時点でcnt規定値か否か判定する為for文開始前は初期化
		for (i = 0; i < prs->idx - 1; i++){                   //ブロック個数分繰り返し

			if (img[i].cx == img[i].chx){ cnt++; }
			else if ((img[i].chx - img[i].cx) < 0){ img[i].cx -= mv; }  //x移動方向判別,cnt規定値になるまで処理
			else if ((img[i].chx - img[i].cx) > 0){ img[i].cx += mv; }

			if (img[i].cy == img[i].chy){ cnt++; }
			else if ((img[i].chy - img[i].cy) < 0){ img[i].cy -= mv; }  //y移動方向判別,cnt規定値になるまで処理
			else if ((img[i].chy - img[i].cy) > 0){ img[i].cy += mv; }

		}//for終わり

		if (img[prs->idx - 1].cx == prs->lastposx){ cnt++; }  //右下画像右移動
		else { img[prs->idx - 1].cx -= mv; }

		draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//画像描画
		dx->drawscreen();//描画
	}//xy移動終了

	sp[0] = 400;                  //スペースx座標初期化
	sp[1] = prs->lastposy;       //スペースy座標初期化

	for (i = 0; i < prs->idx; i++){//z方向戻り
		img[i].chz = 0;
	}

	cnt = 0;
	while (cnt < prs->idx){//z方向戻り
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}

		cnt = 0;   //for文idx回終了時点でcnt規定値か否か判定する為for文開始前は初期化
		for (i = 0; i < prs->idx; i++){ //ブロック個数分繰り返し

			//z移動方向判別,cnt規定値になるまで処理
			if (img[i].cz == img[i].chz){ cnt++; continue; }    //到達後処理

			if ((img[i].chz - img[i].cz) < 0){                 //-方向移動後処理
				img[i].cz -= mv; if ((img[i].chz - img[i].cz) >= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}

			if ((img[i].chz - img[i].cz) > 0){                //+方向移動後処理
				img[i].cz += mv; if ((img[i].chz - img[i].cz) <= 0){
					img[i].cz = img[i].chz; cnt++; continue;
				}
			}
		}

		draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//画像描画
		dx->drawscreen();//描画
	}//z方向戻り終わり

	while (1){
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg->message == WM_QUIT) {
				return -1;//ループの終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(msg);
				DispatchMessage(msg);
			}
		}
		int the = draw->theta(1, 5);
		if (draw->drawing_img(dx, filter, this, 0, 0, 0, 0) == -1) return -1;//画像描画
		dx->drawscreen();//描画
		if (the == 0)break;
	}

	return 0;
} //change()終わり	


int Move::shuffle(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw){   //シャッフル関数宣言 

	int i, j, k, tempx, tempy;
	para *prs = &paras[size];

	k = 0;
	for (j = 0; j <= prs->lastposy; j += prs->bsize){    //座標データ初期化
		for (i = 0; i <= prs->lastposx; i += prs->bsize){
			img[k].chx = i;
			img[k++].chy = j;
		}
	}

	for (i = 0; i < prs->rand_i; i++){  //img[]内でchx,chyをランダムでデータ入れ替え
		//size==5の場合の処理(rand関数max値が足りない為)
		if (size == 5){
			j = (rand() % 10000) * 15 + rand() % 10014; k = (rand() % 10000) * 15 + rand() % 10014;
		}
		else{                 //size==0～4の場合の処理
			j = rand() % prs->rand_s;   //交換元添え字j
			k = rand() % prs->rand_s;   //交換先添え字k
			if (j == k){ i--; continue; }
		}
		tempx = img[j].chx;    //交換
		tempy = img[j].chy;
		img[j].chx = img[k].chx;
		img[j].chy = img[k].chy;
		img[k].chx = tempx;
		img[k].chy = tempy;
	} //for終わり  		

	return change(dx, msg, filter, draw);//change返り値をそのまま返す
}

int Move::recovery(Dx9Init *dx, MSG *msg, Filter *filter, ImageDraw *draw){//元通り

	for (int i = 0; i < paras[size].idx; i++){
		img[i].chx = img[i].fx; img[i].chy = img[i].fy;
	}

	return change(dx, msg, filter, draw);//change返り値をそのまま返す
}