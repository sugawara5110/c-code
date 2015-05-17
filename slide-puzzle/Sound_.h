//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　  Sound_クラス                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Sound__Header
#define Class_Sound__Header

class Sound_{

private:
	IGraphBuilder *pGraphBuilder; //インターフェース,グラフへのフィルタの追加、2 つのピンの接続等
	IMediaControl *pMediaControl;//インターフェース,フィルタ グラフを通るデータ フローを制御
	IVideoWindow *pVideoWindow; //インターフェース,ビデオ ウィンドウのプロパティを設定
	IMediaPosition *pMediaPosition; //インターフェース,ストリーム内の位置をシーク
	IBasicAudio *pBasicAudio;//インターフェース,オーディオ ストリームのボリュームとバランスを制御

public:
	Sound_();//コンストラクタ
	void sound();
	~Sound_();//デストラクタ

};

#endif