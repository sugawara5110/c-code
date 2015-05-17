//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　スライドパズルmain                                    **//
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
// メッセージ処理用コールバック関数
// 引数
//		hWnd	: ウィンドウハンドル
//		msg		: メッセージ
//		wParam	: メッセージの最初のパラメータ(押されたキーの特定等に使用)
//		lParam	: メッセージの2番目のパラメータ
// 戻り値
//		メッセージ処理結果
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){

	Menu menu(0);
	menu.xm = GET_X_LPARAM(lParam);
	menu.ym = GET_Y_LPARAM(lParam);
	switch (msg) {
	case WM_CLOSE:			//×ボタン
		PostQuitMessage(0);//アプリケーション終了処理,メッセージキューにWM_QUITをポスト
		break;
	case WM_LBUTTONDOWN://左クリック押し
		menu.clf = 1;
		break;
	default:
		if (wParam == MK_LBUTTON)menu.clf = 1; else menu.clf = 0;
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//-------------------------------------------------------------
// アプリケーションのエントリポイント
// 引数
//		hInstance     : 現在のインスタンスのハンドル
//		hPrevInstance : 以前のインスタンスのハンドル
//		lpCmdLine	  : コマンドラインパラメータ
//		nCmdShow	  : ウィンドウの表示状態
// 戻り値
//		成功したら0以外の値
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	char clsName[] = "Slide Puzzle";// ウィンドウクラス名

	HWND hWnd;//ウィンドウハンドル
	MSG msg; //メッセージ
	//ウインドウクラスの初期化
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); //この構造体のサイズ
	wcex.style = NULL;               //ウインドウスタイル(default)
	wcex.lpfnWndProc = WindowProc;  //メッセージ処理関数の登録
	wcex.cbClsExtra = 0;       //通常は0	                
	wcex.cbWndExtra = 0;      //通常は0					
	wcex.hInstance = hInstance; //インスタンスへのハンドル				
	wcex.hIcon = NULL;         //アイコン (無し)				
	wcex.hCursor = NULL;      //カーソルの形				
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //背景				
	wcex.lpszMenuName = NULL;                       //メニュー無し				
	wcex.lpszClassName = (TCHAR*)clsName;          //クラス名               
	wcex.hIconSm = NULL;                          //小アイコン			   

	//ウインドウクラスの登録(RegisterClassEx関数)
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	//ウインドウ生成ウインドウモード
	if (!(hWnd = CreateWindow(clsName, //登録クラス名
		clsName,                      //ウインドウ名
		WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,//ウインドウスタイル
		CW_USEDEFAULT, //ウインドウ横位置
		0,            //ウインドウ縦位置
		800,             //ウインドウ幅
		600,            //ウインドウ高さ
		NULL,          //親ウインドウハンドル
		NULL,         //メニュー,子ウインドウハンドル
		hInstance,   //アプリケーションインスタンスハンドル
		NULL)))     //ウインドウ作成データ
		return E_FAIL;

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	// WM_PAINTが呼ばれないようにする
	ValidateRect(hWnd, 0);

	//DirectX初期化
	Dx9Init dx;
	if (dx.init(hWnd) == E_FAIL)return -1;

	int flg;        //キー入力,入力履歴
	char n[100];   //file返り値一時保管
	int msgf = 0; //アプリ終了ループ抜けフラグ

	Move move;          //移動クラスオブジェクト生成
	File file;         //ファイルクラスオブジェクト生成
	Filter filter;    //フィルタークラスオブジェクト生成
	Menu menu;       //メニュークラスオブジェクト生成
	ImageDraw *draw;//画像描画クラスオブジェクト定義

	srand((unsigned)time(NULL));

	strcpy(n, file.file(&dx, &msg));//nに返り値格納
	if (strcmp(n, "stop") == 0){   //file内でPostQuitMessage()が呼ばれた為アプリ終了処理
		return (int)msg.wParam;
	}

	draw = new ImageDraw(&dx, n);//ファイル関数返り値でdrawオブジェクト生成
	while (1){ //1番外側のwhile文

		flg = 0;       //移動フラグ初期化
		draw->drawing_img(&dx, &filter, NULL, 0, 0, 0, 1); //描画前処理
		while (flg == 0){    //個数決定
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {//メッセージ取得時実行される
				if (msg.message == WM_QUIT) {//WindowProcでPostQuitMessage()が呼ばれた
					msgf = 1;
					break;	//ループの終了
				}
				else {
					// メッセージの翻訳とディスパッチ
					TranslateMessage(&msg);//キーボード関連のイベントを翻訳
					DispatchMessage(&msg);//メッセージをウインドウプロシージャに転送する
				}
			}
			draw->drawing_img(&dx, &filter, NULL, 0, 0, 0, 0);//描画
			flg = menu.mouse(&dx, draw, 2, 0);  //マウス関数
			dx.drawscreen();//描画
		}               //while終わり
		if (msgf)break;//アプリ終了ループ抜け
		move.coordinates(flg - 1);//(frg - 1)ブロックサイズ番号で座標確保

		//ブロック画像フィルター作成
		filter.filter(&move, draw);

		if (move.shuffle(&dx, &msg, &filter, draw) == -1){//シャッフル関数
			break;	//ループの終了
		}

		while (1){//パズル実行中ループ
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {	// PostQuitMessage()が呼ばれた
					msgf = 1;
					break;	//ループの終了
				}
				else {
					// メッセージの翻訳とディスパッチ
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			flg = 0; //移動フラグ初期化

			flg = menu.mouse(&dx, draw, 3, 0); //マウス関数

			switch (flg){

			case 5://全ブロック位置元通り処理↓
				if (move.recovery(&dx, &msg, &filter, draw) == -1){//ブロック元通り関数
					msgf = 1;
					break;	//ループの終了
				}
				break;

			case 6://並び替え
				if (move.shuffle(&dx, &msg, &filter, draw) == -1){//シャッフル関数
					msgf = 1;
					break;	//ループの終了
				}
				break;

			case 7://automatic関数実行
				if (move.auto_matic(&dx, &msg, &filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0) == -1){ msgf = 1; break; }
				draw->drawing_img(&dx, &filter, &move, 0, 0, 0, 0);
				break;

			case 9://画像変更
				delete draw;        //オブジェクト破棄  
				strcpy(n, file.file(&dx, &msg));//nに返り値格納
				if (strcmp(n, "stop") == 0){   //file内でPostQuitMessage()が呼ばれた為アプリ終了処理
					return (int)msg.wParam;
				}
				draw = new ImageDraw(&dx, n);//ファイル関数返り値でdrawオブジェクト生成
				draw->drawing_img(&dx, &filter, &move, 0, 0, 0, 1);//背景,画像描画
				break;

			case 13://手数計算
				if (move.count(&dx, &msg, &filter, draw, 0, move.paras[move.size].pcs, move.paras[move.size].pcs, 0) == -1){ msgf = 1; break; }
				break;

			}//switch終了
			if (msgf)break;//ループ終了

			//画像処理準備
			if (draw->image_processing_flg(&dx, &filter, &move, flg) == 1)break;//==1はパズル実行中ループ抜け

			dx.drawscreen();//描画

			if (move.mov(&dx, &msg, &filter, draw, 0, flg, 0) == -1){ msgf = 1; break; } //移動処理,ループ抜け

			if (draw->drawing_img(&dx, &filter, &move, 0, 0, 0, 0) == -1){ msgf = 1; break; }//移動しない場合の為の処理

		}//パズル実行中ループ
		if (msgf)break;//ループ終了

	}//一番外のwhile終わり

	if (draw != NULL){//オブジェクト生成されている時のみ実行
		delete draw;                  //オブジェクト破棄
		draw = NULL;
	}

	return (int)msg.wParam;
}// winmain終わり

