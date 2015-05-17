//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　サウンド処理                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include <stdio.h>
#include <dshow.h>
#include "Sound_.h"

#define	FILENAME L"./dat/ogg/botan.dat"

void Sound_::sound(){

	pBasicAudio->put_Volume(-1000);//音声ON
	
	pMediaPosition->put_CurrentPosition(0);//再生位置をスタート位置にセット
	
}

Sound_::Sound_(){//コンストラクタ

	// COMを初期化
	CoInitialize(NULL);

	// FilterGraphを生成
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// MediaControlインターフェース取得
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	// IVideoWindowインターフェースを取得します
	pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

	//MediaPositionインターフェース取得
	pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);

	//BasicAudioインターフェース取得
	pGraphBuilder->QueryInterface(IID_IBasicAudio,
		(LPVOID *)&pBasicAudio);

	// Graphを生成
	pMediaControl->RenderFile(FILENAME);

	pVideoWindow->put_AutoShow(OAFALSE);  //自動表示しないようにする

	// 再生開始
	pMediaControl->Run();

	pBasicAudio->put_Volume(-10000);//音量調整(-10000 ～ 0)

}

Sound_::~Sound_(){//デストラクタ

	// 資源を解放
	pBasicAudio->Release();
	pBasicAudio = NULL;
	pMediaPosition->Release();
	pMediaPosition = NULL;
	pVideoWindow->Release();
	pVideoWindow = NULL;
	pMediaControl->Release();
	pMediaControl = NULL;
	pGraphBuilder->Release();
	pGraphBuilder = NULL;

	// COM終了
	CoUninitialize();

}