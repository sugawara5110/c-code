//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　構造体型定義.h                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>

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

typedef struct {
	cv::VideoCapture cap;         //USBカメラハンドル
	cv::Mat mt_temp;             //検出画像格納
	int th_f;                   //検出スレッド操作
	volatile int th_st;        //検出スレッド進行状況 volatile コンパイラ最適化防止
	volatile int lock, lock_t;//データ操作時ロック(排他制御)
	imxy *img;                         //画像座標 
	int **imgpi;                      //画像ピクセル情報
	int **imcpy;                     //画像ピクセル情報コピー
	int **block;                    //ブロック画像フィルター
	int xrs, yrs;                  //画像リサイズ後のサイズ
	int finish;                   //完成画像表示フラグ 0:非表示 1:表示 (マウス関数で操作,移動ボタン範囲内にカーソル時表示)
	int mc;                      //pic_resize内のメモリ確保チェック
	char g_name[100];           //画像ファイル名格納
    int size;                  //paras添え字
	int space[2];             //スペース座標0:x 1:y
    para paras[6];           //ブロック毎のパラメーター
	int gfr;                //画像モード 0:通常 1:モノクロ 2:モザイク 3:エッジ検出 4:エンボス処理 5:絵画風処理 6:顔面検出処理 7:顔面モザイク 8:ネガポジ
	int mof;               //動画ファイルオープン用ハンドル(再生,停止判別用)
	int tkf;              //手数計算実行フラグ
	int tkc;             //手数計算カウント
	int cnt_results;    //auto_matic関数のカウント結果
	int mcf;           //0:静止画選択中 1:動画選択中 2:カメラ選択中 
}alldata, *alldata_t; //ほぼ全関数で使うデータ群構造体p   検出スレッドデータ p_t