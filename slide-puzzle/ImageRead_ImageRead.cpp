//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　ImageReadクラス内メソッド定義(コンストラクタ等)             **//
//**                                  (ImageDrawの基底クラス)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

ImageRead::ImageRead(){}    //規定コンストラクタ

ImageRead::ImageRead(char *name){ //引数有コンストラクタ

	face_detect_h = NULL;   //検出スレッドハンドル初期化　
	LPSTR lstr = name;     //directshow動画用ファイル名格納
	BSTR bstr;

	pGraphBuilder = NULL;
	pSampleGrabberFilter = NULL;
	pSampleGrabber = NULL;
	pVideoWindow = NULL;
	pMediaControl = NULL;
	pMediaPosition = NULL;
	pBasicAudio = NULL;
	pVideoInfoHeader = NULL;
	nBufferSize = NULL;
	pBuffer = NULL;
	linesize = NULL;

	// COMを初期化
	CoInitialize(NULL);

	//BSTRに必要なバッファサイズを求める(directshow用)
	int bstrlen = (int)MultiByteToWideChar(
		CP_ACP,		 // コードページ ANSI コードページ
		0,			// 文字の種類を指定するフラグ
		lstr,	   // マップ元文字列のアドレス
		strlen(lstr), // マップ元文字列のバイト数
		NULL,		 // マップ先ワイド文字列を入れるバッファのアドレス
		0			// バッファのサイズ
		);

	//バッファを確保する
	bstr = SysAllocStringLen(NULL, bstrlen);

	//BSTRに複製
	MultiByteToWideChar(
		CP_ACP,
		0,
		lstr,
		strlen(lstr),
		bstr,
		bstrlen
		);

	strcpy(d.g_name, name);//ファイル名格納
	int stl = strlen(d.g_name) - 4;

	d.mcf = 0;//選択ファイル判別初期化

	if (!strcmp(d.g_name + stl, ".mpg") || !strcmp(d.g_name + stl, ".avi") || //動画選択時
		!strcmp(d.g_name + stl, ".MPG") || !strcmp(d.g_name + stl, ".AVI") ||
		!strcmp(d.g_name + stl, ".OGG") || !strcmp(d.g_name + stl, ".ogg") ||
		!strcmp(d.g_name + stl, ".WMV") || !strcmp(d.g_name + stl, ".wmv") ||
		!strcmp(d.g_name + stl, ".FLV") || !strcmp(d.g_name + stl, ".flv") ||
		!strcmp(d.g_name + stl, ".MP4") || !strcmp(d.g_name + stl, ".mp4"))d.mcf = 1;

	if (!strcmp(d.g_name, "./dat/img/z_cam_ewc.dat"))d.mcf = 2;//カメラ選択時

	imgpi = NULL;    //ピクセル配列NULL判定用
	d.th_st = 0;    //検出スレッド進行状況初期化
	d.lock = 0;    //データ操作時ロック初期化(本スレ)
	d.lock_t = 0; //データ操作時ロック初期化(検出スレッド)
	d.th_fin = 0;//スレッドreturnフラグ初期化
	d.gfl = 0;  //画像処理モード初期化
	rate = 1.0;     //再生速度 
	
	if (d.mcf == 1){//動画初期処理開始

		// FilterGraphを生成
		CoCreateInstance(CLSID_FilterGraph,
			NULL,
			CLSCTX_INPROC,
			IID_IGraphBuilder,
			(LPVOID *)&pGraphBuilder);

		// SampleGrabber(Filter)を生成
		CoCreateInstance(CLSID_SampleGrabber,
			NULL,
			CLSCTX_INPROC,
			IID_IBaseFilter,
			(LPVOID *)&pSampleGrabberFilter);

		// FilterからISampleGrabberインターフェースを取得します
		pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (LPVOID *)&pSampleGrabber);

		// IVideoWindowインターフェースを取得します
		pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

		// SampleGrabberを接続するフォーマットを指定。
		// ここの指定の仕方によりSampleGrabberの挿入箇所を
		// 決定できます。このサンプルのような指定をすると
		// 画面出力の寸前でサンプルを取得できます。
		ZeroMemory(&am_media_type, sizeof(am_media_type));
		am_media_type.majortype = MEDIATYPE_Video;
		am_media_type.subtype = MEDIASUBTYPE_RGB24;
		am_media_type.formattype = FORMAT_VideoInfo;
		pSampleGrabber->SetMediaType(&am_media_type);

		// GraphにSampleGrabber Filterを追加
		pGraphBuilder->AddFilter(pSampleGrabberFilter, L"Sample Grabber");

		// MediaControlインターフェース取得
		pGraphBuilder->QueryInterface(IID_IMediaControl,
			(LPVOID *)&pMediaControl);

		//MediaPositionインターフェース取得
		pGraphBuilder->QueryInterface(IID_IMediaPosition,
			(LPVOID *)&pMediaPosition);

		//BasicAudioインターフェース取得
		pGraphBuilder->QueryInterface(IID_IBasicAudio,
			(LPVOID *)&pBasicAudio);

		// Graphを生成。
		// ここでSampleGrabberを含んだGraphが自動的に作成され
		pMediaControl->RenderFile(bstr);//bstrファイル名

		// 接続情報取得。
		// この処理はRenderFileによりGraphが構成された後に
		// 行う必要があります。
		pSampleGrabber->GetConnectedMediaType(&am_media_type);
		pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;

		// Grab実行TRUE,実行しないFALSE  
		// SetBufferSamplesを行わないとバッファからデータを取得できない。
		pSampleGrabber->SetBufferSamples(TRUE);

		pVideoWindow->put_AutoShow(OAFALSE);  //自動表示しないようにする

		// 再生開始
		pMediaControl->Run();

		// バッファを用意
		nBufferSize = am_media_type.lSampleSize;// データサイズ
		pBuffer = new BYTE[nBufferSize];

		//pBuffe x方向1ラインサイズ計算
		linesize = pVideoInfoHeader->bmiHeader.biWidth * 3;
		if (linesize % sizeof(LONG) != 0)
			linesize += sizeof(LONG) - (linesize % sizeof(LONG));

		//ストリームの時間幅を取得(最初に1回取得すればok)
		pMediaPosition->get_Duration(&time2);
	}

	if (d.mcf == 2){//カメラ初期処理開始
		d.cap.open(0);                     //カメラopen
		d.cap.set(CV_CAP_PROP_FPS, 30.0); //フレームレート30にする
	}

}

ImageRead::~ImageRead(){//デストラクタ

	d.th_f = 0;   //検出スレッドフラグ初期化
	d.th_fin = 1;//スレッドreturnフラグON
	if (d.cap.isOpened() == true){//カメラopenの場合
		d.cap.release(); //カメラ終了処理(=NULLとかやらないように)
	}
	if (d.mcf == 1){   //動画選択時資源解放
		free(pBuffer);//メモリ解放
		pBasicAudio->Release();
		pBasicAudio = NULL;
		pMediaPosition->Release();
		pMediaPosition = NULL;
		pMediaControl->Release();
		pMediaControl = NULL;
		pVideoWindow->Release();
		pVideoWindow = NULL;
		pSampleGrabber->Release();
		pSampleGrabber = NULL;
		pSampleGrabberFilter->Release();
		pSampleGrabberFilter = NULL;
		pGraphBuilder->Release();
		pGraphBuilder = NULL;
	}

	// COM終了
	CoUninitialize();

	//メモリ解放
	for (int i = 0; i < yrs; i++)free(imgpi[i]);
	free(imgpi);
	imgpi = NULL;
}

//変数操作
int ImageRead::getgfl(){  //gfl出力
	return d.gfl;
}

int ImageRead::getmcf(){  //mcf読み込み
	return d.mcf;
}

REFTIME ImageRead::gettime1(){  //全再生時間出力
	return d.time1;
}
	
REFTIME ImageRead::gettime2(){  //全再生時間出力
	return time2;
}

float ImageRead::getrate(){  //再生速度出力
	return rate;
}

void ImageRead::putmediapos(REFTIME p){ //再生位置入力
	pMediaPosition->put_CurrentPosition(p);
}

void ImageRead::putrate(float ra){ //再生速度入力
	rate = ra;
}

void ImageRead::putVol(int v){   //音量入力
	pBasicAudio->put_Volume(v);
}

