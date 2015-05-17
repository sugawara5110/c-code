//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(�R���X�g���N�^��)             **//
//**                                  (ImageDraw�̊��N���X)                            **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <dshow.h>
#include <qedit.h>
#include "ImageRead.h"

ImageRead::ImageRead(){}    //�K��R���X�g���N�^

ImageRead::ImageRead(Dx9Init *dx, char *name){ //�����L�R���X�g���N�^

	face_detect_h = NULL;   //���o�X���b�h�n���h���������@
	LPSTR lstr = name;     //directshow����p�t�@�C�����i�[
	BSTR bstr;

	pGraphBuilder = NULL;
	pSampleGrabberFilter = NULL;
	pSampleGrabber = NULL;
	pVideoWindow = NULL;
	pMediaControl = NULL;
	d.pMediaPosition = NULL;
	pBasicAudio = NULL;
	pVideoInfoHeader = NULL;
	nBufferSize = NULL;
	pBuffer = NULL;
	linesize = NULL;
	pMyVB = NULL;

	// COM��������
	CoInitialize(NULL);

	//BSTR�ɕK�v�ȃo�b�t�@�T�C�Y�����߂�(directshow�p)
	int bstrlen = (int)MultiByteToWideChar(
		CP_ACP,		 // �R�[�h�y�[�W ANSI �R�[�h�y�[�W
		0,			// �����̎�ނ��w�肷��t���O
		lstr,	   // �}�b�v��������̃A�h���X
		strlen(lstr), // �}�b�v��������̃o�C�g��
		NULL,		 // �}�b�v�惏�C�h�����������o�b�t�@�̃A�h���X
		0			// �o�b�t�@�̃T�C�Y
		);

	//�o�b�t�@���m�ۂ���
	bstr = SysAllocStringLen(NULL, bstrlen);

	//BSTR�ɕ���
	MultiByteToWideChar(
		CP_ACP,
		0,
		lstr,
		strlen(lstr),
		bstr,
		bstrlen
		);

	strcpy(d.g_name, name);//�t�@�C�����i�[
	int stl = strlen(d.g_name) - 4;

	d.mcf = 0;//�I���t�@�C�����ʏ�����

	if (!strcmp(d.g_name + stl, ".mpg") || !strcmp(d.g_name + stl, ".avi") || //����I����
		!strcmp(d.g_name + stl, ".MPG") || !strcmp(d.g_name + stl, ".AVI") ||
		!strcmp(d.g_name + stl, ".OGG") || !strcmp(d.g_name + stl, ".ogg") ||
		!strcmp(d.g_name + stl, ".WMV") || !strcmp(d.g_name + stl, ".wmv") ||
		!strcmp(d.g_name + stl, ".FLV") || !strcmp(d.g_name + stl, ".flv") ||
		!strcmp(d.g_name + stl, ".MP4") || !strcmp(d.g_name + stl, ".mp4"))d.mcf = 1;

	if (!strcmp(d.g_name, "./dat/img/z_cam_ewc.dat"))d.mcf = 2;//�J�����I����

	d.th_st = 0;    //���o�X���b�h�i�s�󋵏�����
	d.lock = 0;    //�f�[�^���쎞���b�N������(�{�X��)
	d.lock_t = 0; //�f�[�^���쎞���b�N������(���o�X���b�h)
	d.th_fin = 0;//�X���b�hreturn�t���O������
	d.gfr = 0;  //�摜�������[�h������
	mc = 0;    //pic_resize���������m�ۃ`�F�b�N
   
	if (d.mcf == 1){//���揉�������J�n

		// FilterGraph�𐶐�
		CoCreateInstance(CLSID_FilterGraph,
			NULL,
			CLSCTX_INPROC,
			IID_IGraphBuilder,
			(LPVOID *)&pGraphBuilder);

		// SampleGrabber(Filter)�𐶐�
		CoCreateInstance(CLSID_SampleGrabber,
			NULL,
			CLSCTX_INPROC,
			IID_IBaseFilter,
			(LPVOID *)&pSampleGrabberFilter);

		// Filter����ISampleGrabber�C���^�[�t�F�[�X���擾���܂�
		pSampleGrabberFilter->QueryInterface(IID_ISampleGrabber, (LPVOID *)&pSampleGrabber);

		// IVideoWindow�C���^�[�t�F�[�X���擾���܂�
		pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&pVideoWindow);

		// SampleGrabber��ڑ�����t�H�[�}�b�g���w��B
		// �����̎w��̎d���ɂ��SampleGrabber�̑}���ӏ���
		// ����ł��܂��B���̃T���v���̂悤�Ȏw��������
		// ��ʏo�͂̐��O�ŃT���v�����擾�ł��܂��B
		ZeroMemory(&am_media_type, sizeof(am_media_type));
		am_media_type.majortype = MEDIATYPE_Video;
		am_media_type.subtype = MEDIASUBTYPE_RGB24;
		am_media_type.formattype = FORMAT_VideoInfo;
		pSampleGrabber->SetMediaType(&am_media_type);

		// Graph��SampleGrabber Filter��ǉ�
		pGraphBuilder->AddFilter(pSampleGrabberFilter, L"Sample Grabber");

		// MediaControl�C���^�[�t�F�[�X�擾
		pGraphBuilder->QueryInterface(IID_IMediaControl,
			(LPVOID *)&pMediaControl);

		//MediaPosition�C���^�[�t�F�[�X�擾
		pGraphBuilder->QueryInterface(IID_IMediaPosition,
			(LPVOID *)&d.pMediaPosition);

		//BasicAudio�C���^�[�t�F�[�X�擾
		pGraphBuilder->QueryInterface(IID_IBasicAudio,
			(LPVOID *)&pBasicAudio);

		// Graph�𐶐��B
		// ������SampleGrabber���܂�Graph�������I�ɍ쐬����
		pMediaControl->RenderFile(bstr);//bstr�t�@�C����

		// �ڑ����擾�B
		// ���̏�����RenderFile�ɂ��Graph���\�����ꂽ���
		// �s���K�v������܂��B
		pSampleGrabber->GetConnectedMediaType(&am_media_type);
		pVideoInfoHeader = (VIDEOINFOHEADER *)am_media_type.pbFormat;

		// Grab���sTRUE,���s���Ȃ�FALSE  
		// SetBufferSamples���s��Ȃ��ƃo�b�t�@����f�[�^���擾�ł��Ȃ��B
		pSampleGrabber->SetBufferSamples(TRUE);

		pVideoWindow->put_AutoShow(OAFALSE);  //�����\�����Ȃ��悤�ɂ���

		// �Đ��J�n
		pMediaControl->Run();

		// �o�b�t�@��p��
		nBufferSize = am_media_type.lSampleSize;// �f�[�^�T�C�Y
		pBuffer = new BYTE[nBufferSize];

		//pBuffe x����1���C���T�C�Y�v�Z
		linesize = pVideoInfoHeader->bmiHeader.biWidth * 3;
		if (linesize % sizeof(LONG) != 0)
			linesize += sizeof(LONG) - (linesize % sizeof(LONG));

		//�X�g���[���̎��ԕ����擾(�ŏ���1��擾�����ok)
		d.pMediaPosition->get_Duration(&d.time2);
	}

	if (d.mcf == 2){//�J�������������J�n
		d.cap.open(0);                     //�J����open
		d.cap.set(CV_CAP_PROP_FPS, 30.0); //�t���[�����[�g30�ɂ���
	}

	d3 = 1;//3D���[�h�ؑ�
}

ImageRead::~ImageRead(){//�f�X�g���N�^

	if (pMyVB != NULL){
		pMyVB->Release(); pMyVB = NULL;
	}
	d.th_f = 0;   //���o�X���b�h�t���O������
	d.th_fin = 1;//�X���b�hreturn�t���OON
	d.cap.release();    //�J�����I������
	if (d.mcf == 1){   //����I�����������
		free(pBuffer);//���������
		pBasicAudio->Release();
		pBasicAudio = NULL;
		d.pMediaPosition->Release();
		d.pMediaPosition = NULL;
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

	// COM�I��
	CoUninitialize();

	//���������
	for (int i = 0; i < yrs; i++){ free(imgpi[i]); free(imcpy[i]); free(imcpy1[i]); }
	free(imgpi); free(imcpy); free(imcpy1);
}

