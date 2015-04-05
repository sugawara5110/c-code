//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　Moveクラス定義                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Move_Header
#define Class_Move_Header
// #ifndef から #endif までの間に定義を記載します。

class Filter;    //前方宣言(プロトタイプ宣言みたいなもん)
class ImageDraw;//前方宣言(プロトタイプ宣言みたいなもん)

class Move{

private://非公開部
	int auto_space_mov(Filter *filter, ImageDraw *draw, int *cnt, int *x, int *y);             //ブロック自動移動
	int auto_round(Filter *filter, ImageDraw *draw, int *cnt, int *x, int *y, int cx, int cy);//ブロック自動移動

public://公開部
	typedef struct {
		int cx;              //現画像x座標
		int cy;             //現画像y座標
		int fx;            //完成画像x座標
		int fy;           //完成画像y座標
		int chx;         //change関数用x座標
		int chy;        //change関数用y座標
	} imxy;            //画像座標構造体img

	typedef struct {
		int idx;                //img 添え字
		int pcs;               //ブロック縦,横方向個数
		int bsize;            //ブロックサイズ
		int lastposx;        //右下のx座標
		int lastposy;       //右下のy座標
		int rand_i;        //ランダム発生回数
		int rand_s;       //ランダム範囲
		int count;       //change関数の移動完了ブロック判定
		int move;       //ブロック移動量
		int movem;     //動画用ブロック移動量
	}para;            //ブロック個数毎のパラメーター構造体paras

	imxy *img;                //画像座標 
	int size;                //paras添え字
	int space[2];           //スペース座標0:x 1:y
	para paras[6];         //ブロック毎のパラメーター

    int tkf;              //手数計算実行フラグ
	int tkc;             //手数計算カウント
	int cnt_results;    //auto_matic関数のカウント結果

	Move();//コンストラクタ
	int mov(Filter *filter, ImageDraw *draw, int *cnt, int frg, int autof);//ブロック手動移動
	void auto_matic(Filter *filter, ImageDraw *draw, int *cnt, int pcs, int range, int j);//ブロック自動移動
	int change(Filter *filter, ImageDraw *draw);//ブロック交換
    void shuffle();//シャッフル

};

#endif