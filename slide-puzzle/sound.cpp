//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　サウンド処理                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"

void sound(int f){
	static int bt; //ボタン音ハンドル
	static int br;//ブロック移動音ハンドル

	if (f == 0){//初期処理
		bt = LoadSoundMem("./ogg/botan.ogg");
		br = LoadSoundMem("./ogg/mov.ogg");
		return;
	}

	if (f == 1){//ボタン音再生
		PlaySoundMem(bt, DX_PLAYTYPE_BACK, TRUE);
		return;
	}

	if (f == 2){//ブロック移動音再生
		StopSoundMem(br);//ハンドルbr再生ストップ
		PlaySoundMem(br, DX_PLAYTYPE_BACK, TRUE);
		return;
	}

}