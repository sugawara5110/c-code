//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　ImageReadクラス定義                                   **//
//**                              (ImageDrawの基底クラス)                                **//
//*****************************************************************************************//

#ifndef Class_ImageRead_Header
#define Class_ImageRead_Header
// #ifndef から #endif までの間に定義を記載します。

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include <dshow.h>  //directshow
#include <qedit.h> // SampleGrabber用

class Move; //前方宣言(プロトタイプ宣言みたいなもん)

class ImageRead{//ImageDrawの基本クラス

private://非公開
	IGraphBuilder *pGraphBuilder;      //インターフェース,グラフへのフィルタの追加、2 つのピンの接続等
	IBaseFilter *pSampleGrabberFilter;//インターフェース,フィルタを制御
	ISampleGrabber *pSampleGrabber;  //インターフェース,フィルタ グラフ内を通る個々のメディア サンプルを取得
	IVideoWindow *pVideoWindow;     //インターフェース,ビデオ ウィンドウのプロパティを設定
	IMediaControl *pMediaControl;  //インターフェース,フィルタ グラフを通るデータ フローを制御
	VIDEOINFOHEADER *pVideoInfoHeader;//構造体,ビデオ イメージのビットマップと色情報
	AM_MEDIA_TYPE am_media_type;     //メディア サンプルの メジャー タイプを指定するグローバル一意識別子 (GUID)

	IMediaPosition *pMediaPosition; //インターフェース,ストリーム内の位置をシーク
	REFTIME time2;                   //動画の全再生時間
	IBasicAudio *pBasicAudio;      //インターフェース,オーディオ ストリームのボリュームとバランスを制御

	long nBufferSize;//バッファサイズサイズ
	BYTE *pBuffer;  //ピクセルデータバッファ
	int linesize;   //1ラインサイズ

	cv::Mat g2;                 //静止画ハンドル
	HANDLE face_detect_h;    //検出スレッドハンドル
	cv::Mat c_img;           //カメラ画像格納
	int xs, ys;  //画像リサイズ前のサイズ
	float rate; //再生速度

	void face_detect_after(); //顔面検出の後処理
	int resize();             //画像リサイズ処理

protected://派生クラス以外非公開
	typedef struct{
		REFTIME time1;          //動画の現再生位置
		cv::VideoCapture cap;  //USBカメラハンドル
		cv::Mat mt_temp;             //検出画像格納
		int th_f;                    //検出スレッド操作
		volatile int th_st;         //検出スレッド進行状況 volatile コンパイラ最適化防止
		volatile int lock, lock_t;//データ操作時ロック(排他制御)
		int th_fin;               //スレッドreturnフラグ
		char g_name[100];        //画像ファイル名格納
		int gfl;                //画像モード 0:通常 1:モノクロ 2:モザイク 3:エッジ検出 4:エンボス処理 5:絵画風処理 6:顔面検出処理 7:顔面モザイク 8:ネガポジ 9:画像エンボス 10:顔すげ替え 11:スリットスキャン 12:ノイズ除去
		int mcf;               //0:静止画選択中 1:動画選択中 2:カメラ選択中
	}drawdata;               //検出スレッド間でのデータ        
	drawdata d;

public://公開
	int **imgpi;   //画像ピクセル情報
	int xrs, yrs; //画像リサイズ後のサイズ
	
	ImageRead();            //規定コンストラクタ
	ImageRead(char *name); //引数有コンストラクタ(ファイル名)
	int read_img();       //画像読み込み
	~ImageRead();        //デストラクタ

	//変数操作
	int getgfl();               //gfl出力
	int getmcf();              //mcf出力
	REFTIME gettime1();      //現再生位置出力
	REFTIME gettime2();     //全再生時間出力
	float getrate();        //再生速度出力
	void putmediapos(REFTIME p);//再生位置入力
	void putrate(float ra);      //再生速度入力
	void putVol(int v);         //音量入力
};

#endif