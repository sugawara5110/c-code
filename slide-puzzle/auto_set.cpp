//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　オート関数セット                                      **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include "struct_set.h"
#include "mov.h"

int auto_space_mov(alldata *p, int *cnt, int *x, int *y){//スペースを目的ブロックに隣接するまで自動移動,cnt 移動禁止フラグ,x,y目標ブロック
	
	para *prs = &p->paras[p->size];
	imxy *img = p->img;
	int *sp = p->space;
	int f;//mov()結果
	int s = prs->bsize;
	int ret = 0;//return 1 判別用

	while (1){
		if ((sp[0] == *x && sp[1] == *y - s) || (sp[0] == *x + s && sp[1] == *y - s) || //目的地に隣接
			(sp[0] == *x + s && sp[1] == *y) || (sp[0] == *x + s && sp[1] == *y + s) ||
			(sp[0] == *x && sp[1] == *y + s) || (sp[0] == *x - s && sp[1] == *y + s) ||
			(sp[0] == *x - s && sp[1] == *y) || (sp[0] == *x - s && sp[1] == *y - s))break;

		if (sp[0] - *x>0)f = mov(p, cnt, 2, 1);//スペース右位置関係別に引数を変えmov関数を呼び出す
		if (sp[0] - *x>0 && f == 0){ mov(p, cnt, 3, 1); continue; }
		if (sp[0] - *x<0)f=mov(p, cnt, 1, 1);//スペース左
		if (sp[1] - *y>0)f=mov(p, cnt, 4, 1);//スペース下 
		if (sp[1] - *y<0)f=mov(p, cnt, 3, 1);//スペース上
		if (f == 2){ ret = 1; break; }
	}//while終わり
	if (ret == 1)return 1;//auto_matic関数抜け用
	return 0;
}//auto_space_mov終わり




int auto_round(alldata *p, int *cnt,int *x, int *y, int cx, int cy){ //スペース回転,ブロック移動関数,cnt移動禁止フラグ,*x*y移動ブロック,cx,cy移動先
	
	para *prs = &p->paras[p->size];
	imxy *img = p->img;
	int *sp = p->space;
	int s = prs->bsize;
	int fr = 0;   //回転方向 0:時計 0以外:反時計 movp==0(移動未実施)の度切り替わる
	int movp = 0;//回転方向切換フラグ
	int ret = 0;//return 1 判別用
	
	if (auto_space_mov(p, cnt, x, y))return 1;

	while (*x != cx || *y != cy){
		if (sp[0] == *x && sp[1] == *y - s){       //スペース上
			if (*y - cy>0){ mov(p, cnt, 3, 1); continue; }//目的地上方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 1, 1); else movp = mov(p, cnt, 2, 1); //回転方向分岐,右,左
		}
		if (sp[0] == *x + s && sp[1] == *y - s){     //スペース右上
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 3, 1); else movp = mov(p, cnt, 2, 1); //回転方向分岐,下,左
		}
		if (sp[0] == *x + s && sp[1] == *y){       //スペース右
			if (*x - cx<0){ mov(p, cnt, 2, 1); continue; }//目的地右方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 3, 1); else movp = mov(p, cnt, 4, 1); //回転方向分岐,下,上
		}
		if (sp[0] == *x + s && sp[1] == *y + s){     //スペース右下
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 2, 1); else movp = mov(p, cnt, 4, 1); //回転方向分岐,左,上
		}
		if (sp[0] == *x && sp[1] == *y + s){       //スペース下
			if (*y - cy<0){ mov(p, cnt, 4, 1); continue; }//目的地下方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 2, 1); else movp = mov(p, cnt, 1, 1); //回転方向分岐,左,右
		}
		if (sp[0] == *x - s && sp[1] == *y + s){     //スペース左下
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 4, 1); else movp = mov(p, cnt, 1, 1); //回転方向分岐,上,右
		}
		if (sp[0] == *x - s && sp[1] == *y){       //スペース左
			if (*x - cx>0){ mov(p, cnt, 1, 1); continue; }//目的地左方向
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 4, 1); else movp = mov(p, cnt, 3, 1); //回転方向分岐,上,下
		}
		if (sp[0] == *x - s && sp[1] == *y - s){     //スペース左上
			if (movp == 0)fr = !fr;  //移動未実施時,回転方向切換
			if (fr == 0)movp = mov(p, cnt, 1, 1); else movp = mov(p, cnt, 3, 1); //回転方向分岐,右,下
		}
		if (movp == 2){ ret = 1; break; }

	} //while終わり

	if (ret == 1)return 1;//auto_matic関数抜け用
	return 0;

}//auto_round()終わり



void auto_matic(alldata *p,int *cnt, int pcs, int range, int j){    //オート関数宣言 cnt移動禁止フラグ,pcs縦横個数,range対象範囲,jfor文範囲
	
	para *prs = &p->paras[p->size];
	imxy *img = p->img;
	int *sp = p->space;
	int i; //for用
	int ret = 0;//return 判別用

	if (range == pcs){ //初期化
		cnt = (int*)malloc(sizeof(int)*prs->idx);
		for (i = 0; i<prs->idx; i++){
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
			if (range == 2){ mov(p, cnt, 1, 1); mov(p, cnt, 1, 1); p->cnt_results = 0; ret = 1; break; }//一致の場合この処理で終了
		    break; //一致した場合while抜け
		}
		
		//↓範囲内上1列1回目処理 
		for (i = j + 1; i < range + j; i++){
			if (auto_round(p, cnt, &img[i].cx, &img[i].cy, img[i - 1].fx, img[i - 1].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;	//移動禁止ON
		} //for終わり
		if (ret == 1)break;//上のfor内ブレークの場合while抜け

		//↓移動不可状態処理
		if ((img[j].cx == img[range + j - 1].fx && img[j].cy == img[range + j - 1].fy) ||
			(img[j].cx == img[range + j - 1 + pcs].fx && img[j].cy == img[range + j - 1 + pcs].fy &&
			sp[0] == img[range + j - 1].fx && sp[1] == img[range + j - 1].fy)){
			for (i = j + 1; i < range + j; i++){
				cnt[i] = 0;
			}
			if (auto_round(p, cnt, &img[j].cx, &img[j].cy, img[range + j - 1 + pcs * 2].fx, img[range + j - 1 + pcs * 2].fy)){
				ret = 1; break;//中止の場合while抜け
			}
			continue;//移動不可状態処理後最初に戻る
		}

		//↓範囲内上1列左1個処理
		if (auto_round(p, cnt, &img[j].cx, &img[j].cy, img[j + pcs].fx, img[j + pcs].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;  //移動禁止ON

		//↓範囲内上1列2回目処理
		for (i = range + j - 1; i >= j + 1; i--){
			cnt[i] = 0;        //移動前移動禁止OFF
			if (auto_round(p, cnt, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;        //移動後移動禁止ON
		}//for終わり
		if (ret == 1)break;//上のfor内breakの場合while抜け

		if (range == 2){ cnt[j] = 0; mov(p, cnt, 3, 1); mov(p, cnt, 1, 1); mov(p, cnt, 1, 1); p->cnt_results = 0; ret = 1; break; }//完了

		//↓範囲内上1列左1個処理
		cnt[j] = 0;            //移動前移動禁止OFF
		if (auto_round(p, cnt, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;            //移動後移動禁止ON
	
		break;//while抜け
	}//上1列処理ループ終わり
	if (ret == 1)return;//while内breakの場合関数終了

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

		//↓範囲内左1列1回目処理
		for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
			if (auto_round(p, cnt, &img[i].cx, &img[i].cy, img[i - pcs].fx, img[i - pcs].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;  //移動禁止ON
		} //for終わり
		if (ret == 1)break;//上for内breakの場合while抜け

		//↓移動不可状態処理
		if ((img[j].cx == img[j + pcs*(range - 1)].fx && img[j].cy == img[j + pcs*(range - 1)].fy) ||
			(img[j].cx == img[j + pcs*(range - 1) + 1].fx && img[j].cy == img[j + pcs*(range - 1) + 1].fy &&
			sp[0] == img[j + pcs*(range - 1)].fx && sp[1] == img[j + pcs*(range - 1)].fy)){
			for (i = j + pcs; i <= j + pcs*(range - 1); i += pcs){
				cnt[i] = 0;
			}
			if (auto_round(p, cnt, &img[j].cx, &img[j].cy, img[j + pcs*(range - 1) + 2].fx, img[j + pcs*(range - 1) + 2].fy)){
				ret = 1; break;//中止の場合while抜け
			}
			continue;//移動不可状態処理後最初に戻る
		}

		//↓範囲内左1列上1個処理
		if (auto_round(p, cnt, &img[j].cx, &img[j].cy, img[j + 1].fx, img[j + 1].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;         //移動禁止ON

		//↓範囲内左1列2回目処理
		for (i = j + pcs*(range - 1); i >= j + pcs; i -= pcs){
			cnt[i] = 0;   //移動禁止OFF
			if (auto_round(p, cnt, &img[i].cx, &img[i].cy, img[i].fx, img[i].fy)){ ret = 1; break; }//中止の場合for抜け
			cnt[i] = 1;   //移動禁止ON
		}//for終わり
		if (ret == 1)break;//上for内breakの場合while抜け

		//↓範囲内左1列上1個処理
		cnt[j] = 0;  //移動禁止OFF
		if (auto_round(p, cnt, &img[j].cx, &img[j].cy, img[j].fx, img[j].fy)){ ret = 1; break; }//中止の場合while抜け
		cnt[j] = 1;  //移動禁止ON
	
		break;//while抜け
	}//左1列処理ループ終わり
	if (ret == 1)return;//while内breakの場合関数終了
	
	j++; //for文初期値変更
	//↓再帰呼び出し  
	auto_matic(p,cnt,pcs,range,j);
		
}//auto_matic()終わり