//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　ブロック交換,並び替えセット                           **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "struct_set.h"
#include "drawing_img.h"

int change(alldata *p){              //ブロック交換関数宣言

	int i; //for用
	int cnt = 0;   //移動完了ブロック数カウント 
	para *prs = &p->paras[p->size];
	imxy *img = p->img;
	int *sp = p->space;
	int mv;       //移動量変更用

	mv = prs->move;//通常ブロック移動量に初期化
	
	if (p->mcf == 1 || p->mcf == 2){//動画ブロック時,移動量に更新
		mv = prs->movem;           //動画ブロック時移動量更新
	}

	while (ScreenFlip() == 0 && cnt<prs->count){
		if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End();  free(img); return -1; }
		cnt = 0;   //for文(idx-1)回終了時点でcnt規定値か否か判定する為for文開始前は初期化
				
		for (i = 0; i<prs->idx - 1; i++){                   //ブロック個数分繰り返し
			
			if (img[i].cx == img[i].chx){ cnt++; }
			else if ((img[i].chx - img[i].cx)<0){ img[i].cx -= mv; }  //x移動方向判別,cnt規定値になるまで処理
			else if ((img[i].chx - img[i].cx)>0){ img[i].cx += mv; }

			if (img[i].cy == img[i].chy){ cnt++; }
			else if ((img[i].chy - img[i].cy)<0){ img[i].cy -= mv; }  //y移動方向判別,cnt規定値になるまで処理
			else if ((img[i].chy - img[i].cy)>0){ img[i].cy += mv; }

		}   //for終わり

		if (img[prs->idx - 1].cx == prs->lastposx){ cnt++; }                 //右下画像右移動
		else { img[prs->idx - 1].cx -= mv; }

		drawing_img(p, 0, 0, 0);//画像描画
		

	}    //while終わり
	sp[0] = 600;                  //スペースx座標初期化
	sp[1] = prs->lastposy;       //スペースy座標初期化
	return 0;
}  //change()終わり	


void shuffle(alldata *p){   //シャッフル関数宣言 

	int i, j, k, tempx, tempy;
	para *prs = &p->paras[p->size];
	imxy *img = p->img;

	k = 0;
	for (j = 100; j <= prs->lastposy; j += prs->bsize){    //座標データ初期化
		for (i = 200; i <= prs->lastposx; i += prs->bsize){
			img[k].chx = i;
			img[k++].chy = j;
		}
	}

	for (i = 0; i<prs->rand_i; i++){         // img[]内でchx,chyをランダムでデータ入れ替え
		//size==5の場合の処理(rand関数max値が足りない為)
		if (p->size == 5){
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

}   //shuffle()終わり 