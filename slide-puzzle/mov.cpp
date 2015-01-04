//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　移動処理　　　　　                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include "struct_set.h"
#include "mouse.h"
#include "sound.h"
#include "drawing_img.h"

int mov(alldata *p,int *cnt,int frg, int autof){  //移動処理関数宣言 cnt移動禁止フラグ,frg移動方向,autof=1:auto_matic関数実行中
	
	int i;             //for
	int k = -1;       //初期化
	int mv;          //移動量変更用
	int ms = 0;     //猛スピードモードフラグ
	int mov_p = 0; //移動実施判定用
	int offset[2];//画像座標オフセット
	para *prs = &p->paras[p->size];
	imxy *img = p->img;
	int *sp = p->space;
	int stl; //ファイル名文字列

	offset[0] = 0, offset[1] = 0;//初期化
		
	for (i = 0; i<prs->idx; i++){
		//↓auto_matic()関数のブロック衝突判定処理
		if (autof == 1 && cnt[i] == 1){ k = -1; continue; }
		//↓移動キー毎に全ブロックとスペースの座標比較,移動対象ブロック決定(添え字k)
		if (frg == 1 && img[i].cx == sp[0] + prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (frg == 2 && img[i].cx == sp[0] - prs->bsize && img[i].cy == sp[1]){ k = i; break; }
		if (frg == 3 && img[i].cx == sp[0] && img[i].cy == sp[1] + prs->bsize){ k = i; break; }
		if (frg == 4 && img[i].cx == sp[0] && img[i].cy == sp[1] - prs->bsize){ k = i; break; }

	}  //for終わり


	if (k >= 0){             //移動対象ブロック決定時のみ実行
		if (p->tkf != 1){   //手数計算フラグ1以外に実行
			mv = prs->move;//通常移動量初期化
			stl = strlen(p->g_name) - 4;
			if (!strcmp(p->g_name + stl, ".mpg") || !strcmp(p->g_name + stl, ".avi") ||
				!strcmp(p->g_name, "z_cam_ewc.bmp")){//動画ブロック時,移動量に更新
				mv = prs->movem;                    //動画ブロック時移動量更新
			}

			if (autof == 1 && mouse(p,4, offset) == 1)mv = prs->bsize;//automatic関数実行時ハイスピード
			if (autof == 1 && mouse(p,4, offset) == 2)return 2;      //automatic関数中止
			if (autof == 1 && mouse(p,4, offset) == 3){ mv = prs->bsize; ms = 1; }//猛スピードオン
			
			if (strcmp(p->g_name + stl, ".mpg") && strcmp(p->g_name + stl, ".avi"))sound(2);//サウンド関数ブロック移動処理(静止画の時のみ実行)
		}
		else{//手数計算フラグ1の時実行
			p->tkc++;//手数カウント
			mv = prs->bsize;//猛スピード
			if (mouse(p,5, 0) == 1){ p->cnt_results = -1; return 2; }//手数計算結果,手数計算中止
		}

		while (img[k].cx != sp[0] || img[k].cy != sp[1]){ //移動中座標更新処理,スペース座標に到達したら終了
			
			if (ProcessMessage() == -1){ InitSoftImage(); DxLib_End(); free(cnt); free(img); return-1; }//強制終了
								
			if (frg == 1)img[k].cx -= mv;//座標値更新 ブロック左移動
            if (frg == 2)img[k].cx += mv;//座標値更新 ブロック右移動
			if (frg == 3)img[k].cy -= mv;//座標値更新 ブロック上移動
			if (frg == 4)img[k].cy += mv;//座標値更新 ブロック下移動

			if (ms == 0 && p->tkf == 0) {//猛スピードoff時描写
				drawing_img(p, offset[0], offset[1], 0);//画像描画
				if (autof == 0)mouse(p,3, 0); else mouse(p,4, offset);//マウス座像描画のみ
				ScreenFlip(); //表画面描写
			}
         }//while終了
		
		//↓移動完了後スペース座標更新
		if (frg == 1)sp[0] += prs->bsize;
		if (frg == 2)sp[0] -= prs->bsize;
		if (frg == 3)sp[1] += prs->bsize;
		if (frg == 4)sp[1] -= prs->bsize;
		
	} //if終わり
	if (k >= 0)mov_p = 1;
	
	return mov_p;
}  //mov()終わり
