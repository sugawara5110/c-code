//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　構造体型定義.h                                        **//
//**                                                                                     **//
//*****************************************************************************************//

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
} para;           //ブロック個数毎のパラメーター構造体paras

typedef struct {                   
	imxy *img;                        //画像座標 
	int **imgpi;                     //画像ピクセル情報
	int **imcpy;                    //画像ピクセル情報コピー
	int **block;                   //ブロック画像フィルター
	int xrs, yrs;                 //画像リサイズ後のサイズ
	int mc;                      //pic_resize内のメモリ確保チェック
	char g_name[25];            //画像ファイル名格納
    int size;                  //paras添え字
	int space[2];             //スペース座標0:x 1:y
    para paras[6];           //ブロック毎のパラメーター
	int gfr;                //画像モード 0:通常 1:モノクロ 2:モザイク 3:エッジ検出 4:エンボス処理 5:水彩画風処理
	int mof;               //動画ファイルオープン用ハンドル(再生,停止判別用)
	int tkf;              //手数計算実行フラグ
	int tkc;             //手数計算カウント
	int cnt_results;    //auto_matic関数のカウント結果
}alldata;              //ほぼ全関数で使うデータ群構造体p