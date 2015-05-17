//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@�X���C�h�p�Y��main                                    **//
//**                                                                                     **//
//*****************************************************************************************//

#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"
#include "Menu.h"
#include "File.h"
#include "Filter.h"

//-------------------------------------------------------------
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		hWnd	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^(�����ꂽ�L�[�̓��蓙�Ɏg�p)
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	Menu menu(0);
	menu.xm = GET_X_LPARAM(lParam);
	menu.ym = GET_Y_LPARAM(lParam);
	switch (msg) {
	case WM_CLOSE:			//�~�{�^��
		PostQuitMessage(0);//�A�v���P�[�V�����I������,���b�Z�[�W�L���[��WM_QUIT���|�X�g
		break;
	case WM_LBUTTONDOWN://���N���b�N����
		menu.clf = 1;
		break;
	default:
		if (wParam == MK_LBUTTON)menu.clf = 1; else menu.clf = 0;
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//-------------------------------------------------------------
// �A�v���P�[�V�����̃G���g���|�C���g
// ����
//		hInstance     : ���݂̃C���X�^���X�̃n���h��
//		hPrevInstance : �ȑO�̃C���X�^���X�̃n���h��
//		lpCmdLine	  : �R�}���h���C���p�����[�^
//		nCmdShow	  : �E�B���h�E�̕\�����
// �߂�l
//		����������0�ȊO�̒l
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	char clsName[] = "Slide Puzzle";// �E�B���h�E�N���X��

	HWND hWnd;//�E�B���h�E�n���h��
	MSG msg; //���b�Z�[�W
	//�E�C���h�E�N���X�̏�����
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //���̍\���̂̃T�C�Y
	wcex.style = NULL;               //�E�C���h�E�X�^�C��(default)
	wcex.lpfnWndProc = WindowProc;  //���b�Z�[�W�����֐��̓o�^
	wcex.cbClsExtra = 0;       //�ʏ��0	                
	wcex.cbWndExtra = 0;      //�ʏ��0					
	wcex.hInstance = hInstance; //�C���X�^���X�ւ̃n���h��				
	wcex.hIcon = NULL;         //�A�C�R�� (����)				
	wcex.hCursor = NULL;      //�J�[�\���̌`				
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //�w�i				
	wcex.lpszMenuName = NULL;                       //���j���[����				
	wcex.lpszClassName = (TCHAR*)clsName;          //�N���X��               
	wcex.hIconSm = NULL;                          //���A�C�R��			   

	//�E�C���h�E�N���X�̓o�^(RegisterClassEx�֐�)
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	//�E�C���h�E�����E�C���h�E���[�h
	if (!(hWnd = CreateWindow(clsName, //�o�^�N���X��
		clsName,                      //�E�C���h�E��
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,//�E�C���h�E�X�^�C��
		CW_USEDEFAULT, //�E�C���h�E���ʒu
		0,            //�E�C���h�E�c�ʒu
		800,             //�E�C���h�E��
		600,            //�E�C���h�E����
		NULL,          //�e�E�C���h�E�n���h��
		NULL,         //���j���[,�q�E�C���h�E�n���h��
		hInstance,   //�A�v���P�[�V�����C���X�^���X�n���h��
		NULL)))     //�E�C���h�E�쐬�f�[�^
		return E_FAIL;

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
	ValidateRect(hWnd, 0);

	//DirectX������
	Dx9Init dx;
	if (dx.init(hWnd) == E_FAIL)return -1;

	int flg;        //�L�[����,���͗���
	char n[100];   //file�Ԃ�l�ꎞ�ۊ�
	int msgf = 0; //�A�v���I�����[�v�����t���O

	Move move;          //�ړ��N���X�I�u�W�F�N�g����
	File file;         //�t�@�C���N���X�I�u�W�F�N�g����
	Filter filter;    //�t�B���^�[�N���X�I�u�W�F�N�g����
	Menu menu;       //���j���[�N���X�I�u�W�F�N�g����
	ImageDraw *draw;//�摜�`��N���X�I�u�W�F�N�g��`

	srand((unsigned)time(NULL));

	strcpy(n, file.file(&dx, &msg));//n�ɕԂ�l�i�[
	if (strcmp(n, "stop") == 0){   //file����PostQuitMessage()���Ă΂ꂽ�׃A�v���I������
		return (int)msg.wParam;
	}

	draw = new ImageDraw(&dx, n);//�t�@�C���֐��Ԃ�l��draw�I�u�W�F�N�g����
	while (1){ //1�ԊO����while��

		flg = 0;       //�ړ��t���O������
		draw->drawing_img(&dx, &filter, NULL, 0, 0, 0, 1); //�`��O����
		while (flg == 0){    //������
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {//���b�Z�[�W�擾�����s�����
				if (msg.message == WM_QUIT) {//WindowProc��PostQuitMessage()���Ă΂ꂽ
					msgf = 1;
					break;	//���[�v�̏I��
				}
				else {
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);//�L�[�{�[�h�֘A�̃C�x���g��|��
					DispatchMessage(&msg);//���b�Z�[�W���E�C���h�E�v���V�[�W���ɓ]������
				}
			}
			draw->drawing_img(&dx, &filter, NULL, 0, 0, 0, 0);//�`��
			flg = menu.mouse(&dx, draw, 2, 0);  //�}�E�X�֐�
			dx.drawscreen();//�`��
		}               //while�I���
		if (msgf)break;//�A�v���I�����[�v����
		move.coordinates(flg - 1);//(frg - 1)�u���b�N�T�C�Y�ԍ��ō��W�m��

		//�u���b�N�摜�t�B���^�[�쐬
		filter.filter(&move, draw);

		if (move.shuffle(&dx, &msg, &filter, draw) == -1){//�V���b�t���֐�
			break;	//���[�v�̏I��
		}

		while (1){//�p�Y�����s�����[�v
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {	// PostQuitMessage()���Ă΂ꂽ
					msgf = 1;
					break;	//���[�v�̏I��
				}
				else {
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			flg = 0; //�ړ��t���O������

			flg = menu.mouse(&dx, draw, 3, 0); //�}�E�X�֐�

			switch (flg){

			case 5://�S�u���b�N�ʒu���ʂ菈����
				if (move.recovery(&dx, &msg, &filter, draw) == -1){//�u���b�N���ʂ�֐�
					msgf = 1;
					break;	//���[�v�̏I��
				}
				break;

			case 6://���ёւ�
				if (move.shuffle(&dx, &msg, &filter, draw) == -1){//�V���b�t���֐�
					msgf = 1;
					break;	//���[�v�̏I��
				}
				break;

			case 7://automatic�֐����s
				if (move.auto_matic(&dx, &msg, &filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0) == -1){ msgf = 1; break; }
				draw->drawing_img(&dx, &filter, &move, 0, 0, 0, 0);
				break;

			case 9://�摜�ύX
				delete draw;        //�I�u�W�F�N�g�j��  
				strcpy(n, file.file(&dx, &msg));//n�ɕԂ�l�i�[
				if (strcmp(n, "stop") == 0){   //file����PostQuitMessage()���Ă΂ꂽ�׃A�v���I������
					return (int)msg.wParam;
				}
				draw = new ImageDraw(&dx, n);//�t�@�C���֐��Ԃ�l��draw�I�u�W�F�N�g����
				draw->drawing_img(&dx, &filter, &move, 0, 0, 0, 1);//�w�i,�摜�`��
				break;

			case 13://�萔�v�Z
				if (move.count(&dx, &msg, &filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0) == -1){ msgf = 1; break; }
				break;

			}//switch�I��
			if (msgf)break;//���[�v�I��

			//�摜��������
			if (draw->image_processing_flg(&dx, &filter, &move, flg) == 1)break;//==1�̓p�Y�����s�����[�v����

			dx.drawscreen();//�`��

			if (move.mov(&dx, &msg, &filter, draw, 0, flg, 0) == -1){ msgf = 1; break; } //�ړ�����,���[�v����

			if (draw->drawing_img(&dx, &filter, &move, 0, 0, 0, 0) == -1){ msgf = 1; break; }//�ړ����Ȃ��ꍇ�ׂ̈̏���

		}//�p�Y�����s�����[�v
		if (msgf)break;//���[�v�I��

	}//��ԊO��while�I���

	if (draw != NULL){//�I�u�W�F�N�g��������Ă��鎞�̂ݎ��s
		delete draw;                  //�I�u�W�F�N�g�j��
		draw = NULL;
	}

	return (int)msg.wParam;
}// winmain�I���

