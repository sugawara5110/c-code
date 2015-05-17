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

class Filter;//前方宣言(プロトタイプ宣言みたいなもん)
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
	long nBufferSize;
	BYTE *pBuffer;
	int linesize;

	cv::Mat g2;                 //静止画ハンドル
	HANDLE face_detect_h;      //検出スレッドハンドル
	cv::Mat c_img;            //カメラ画像格納
	int mc;                  //pic_resize内のメモリ確保チェック

	void face_detect_after();    //顔面検出の後処理
	int pic_resize(Dx9Init *dx);//画像リサイズ処理

protected://派生クラス以外非公開
	LPDIRECT3DVERTEXBUFFER9 pMyVB;//頂点バッファー
	int **imcpy1; //画像ピクセル情報コピー(静止画用)
	int xs, ys;  //動画描画前処理用の画像サイズ
	int d3;     //3D切り替え

public://公開
	typedef struct{
		IMediaPosition *pMediaPosition; //インターフェース,ストリーム内の位置をシーク
		REFTIME time1, time2;          //動画の再生位置,再生時間

		cv::VideoCapture cap;         //USBカメラハンドル
		cv::Mat mt_temp;             //検出画像格納
		int th_f;                   //検出スレッド操作
		volatile int th_st;        //検出スレッド進行状況 volatile コンパイラ最適化防止
		volatile int lock, lock_t;//データ操作時ロック(排他制御)
		int th_fin;              //スレッドreturnフラグ
		char g_name[100];      //画像ファイル名格納
		int gfr;              //画像モード 0:通常 1:モノクロ 2:モザイク 3:エッジ検出 4:エンボス処理 5:絵画風処理 6:顔面検出処理 7:顔面モザイク 8:ネガポジ 9:画像エンボス 10:顔すげ替え
		int mcf;             //0:静止画選択中 1:動画選択中 2:カメラ選択中
	}drawdata, *drawdata_t; //検出スレッド間でのデータ        

	drawdata d;
	IBasicAudio *pBasicAudio;//インターフェース,オーディオ ストリームのボリュームとバランスを制御

	int **imgpi;    //画像ピクセル情報
	int **imcpy;   //画像ピクセル情報コピー
	int xrs, yrs; //画像リサイズ後のサイズ
	int finish;  //完成画像表示フラグ 0:非表示 1:表示 (マウス関数で操作,移動ボタン範囲内にカーソル時表示)

	ImageRead();                         //規定コンストラクタ
	ImageRead(Dx9Init *dx, char *name); //引数有コンストラクタ(ファイル名)
	int drawing_img(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z, int f); //画像読み込み描画処理
	virtual HRESULT draw(Dx9Init *dx, Filter *filter, Move *move, int x, int y, int z);  //画像描画処理,virtual オーバーライドされる関数
	~ImageRead();                                                   //デストラクタ

};

#endif