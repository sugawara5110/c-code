//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�T�E���h����                                          **//
//**                                                                                     **//
//*****************************************************************************************//

#include <stdio.h>
#include <dshow.h>
#include "Sound_.h"

#define	FILENAME L"./dat/ogg/botan.dat"

void Sound_::sound(){

	pBasicAudio->put_Volume(-1000);//����ON
	
	pMediaPosition->put_CurrentPosition(0);//�Đ��ʒu���X�^�[�g�ʒu�ɃZ�b�g
	
}

Sound_::Sound_(){//�R���X�g���N�^

	// COM��������
	CoInitialize(NULL);

	// FilterGraph�𐶐�
	CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,
		IID_IGraphBuilder,
		(LPVOID *)&pGraphBuilder);

	// MediaControl�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IMediaControl,
		(LPVOID *)&pMediaControl);

	// IVideoWindow�C���^�[�t�F�[�X���擾���܂�
	pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

	//MediaPosition�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IMediaPosition,
		(LPVOID *)&pMediaPosition);

	//BasicAudio�C���^�[�t�F�[�X�擾
	pGraphBuilder->QueryInterface(IID_IBasicAudio,
		(LPVOID *)&pBasicAudio);

	// Graph�𐶐�
	pMediaControl->RenderFile(FILENAME);

	pVideoWindow->put_AutoShow(OAFALSE);  //�����\�����Ȃ��悤�ɂ���

	// �Đ��J�n
	pMediaControl->Run();

	pBasicAudio->put_Volume(-10000);//���ʒ���(-10000 �` 0)

}

Sound_::~Sound_(){//�f�X�g���N�^

	// ���������
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

	// COM�I��
	CoUninitialize();

}