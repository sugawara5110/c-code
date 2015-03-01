//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　顔面検出処理関数(並列スレッド)                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <process.h>
#include "struct_set.h"

unsigned __stdcall face_detect(void *th){


	// 学習済み検出器の読み込み
	cv::string cascadeName = "dat\\xml\\haarcascade_frontalface_alt2.xml";
	cv::CascadeClassifier cascade;
	if (!cascade.load(cascadeName))
		return -1;

	alldata_t p_t = (alldata_t)th;//本スレッドからデータ受け取り(alldataとalldata_tは中身は同じ)
	cv::vector<cv::Rect> faces;     //検出箇所
	cv::VideoCapture video;        //検出スレッド動画再生ハンドル
	cv::Mat ori, ori2;
    cv::Mat dts;
    cv::Mat grayImage;           //検出用グレースケールデータ格納用
    int time1;                  //本スレッドと同期取り用変数
	int x, y, i, j;            //for
	int max_x, max_y,         //顔面モザイク範囲
		min_x, min_y;
	p_t->th_st = 0;         //スレッド進行状況初期化

	while (1){            //この関数が呼ばれたらずっとループしっぱなし 
		try{             //例外処理,機能してるか知らんが一応つけた。
			if (video.isOpened() == true){//動画ファイルがopenの場合
				video.release();         //動画ファイル閉じ
			}

			while (p_t->th_f == 1 && (p_t->gfr == 6 || p_t->gfr == 7)){//検出処理スタートフラグ,  検出処理選択

				switch (p_t->mcf){  //0:静止画選択中 1:動画選択中 2:カメラ選択中

					//静止画処理開始
				case 0:
					ori = cv::imread(p_t->g_name, CV_LOAD_IMAGE_COLOR);//画像読み込み

					break;
					//静止画処理終了

					//動画処理開始
				case 1:
					if (video.isOpened() == false){//動画ファイルcloseの場合
						video.open(p_t->g_name);  //動画ファイルopen
					}
					time1 = TellMovieToGraph(p_t->mof);     //再生位置取得(ミリ秒)
					video.set(CV_CAP_PROP_POS_MSEC, time1);//映像音声同期
					video >> ori;                         //フレーム読み込み
					if (ori.empty()){                    //フレームが空か？	
						video.release();
						video.open(p_t->g_name);
						video >> ori;
					}

					break;
					//動画処理終了

					//カメラ処理開始
				case 2:
					p_t->cap >> ori;
					break;
					//カメラ処理終了

				}//switch終わり

				//半分にリサイズ
				cv::resize(ori, dts, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
				//グレースケール変換
				cvtColor(dts, grayImage, CV_RGB2GRAY);
				//検出処理
				cascade.detectMultiScale(    //検出処理
					grayImage,              // 画像
					faces,                 // 出力される矩形
					1.1,                  // 縮小用のスケール
					2,                   // 最小の投票数
					CV_HAAR_SCALE_IMAGE,// フラグ
					cv::Size(30, 30)   // 最小の矩形
					);

				//検出箇所の矩形描画用のデータ格納構造体を0で初期化
				for (y = 0; y < dts.rows; y++){
					for (x = 0; x < dts.cols; x++){
						dts.data[y * dts.step + x * dts.channels() + 2] = 0;
						dts.data[y * dts.step + x * dts.channels() + 1] = 0;
						dts.data[y * dts.step + x * dts.channels() + 0] = 0;
					}
				}

				// 矩形データをp_t->dtsに格納  (.begin 行列の最初の要素を指した状態の，行列イテレータを返す)
				for (cv::vector<cv::Rect>::iterator iter = faces.begin(); iter != faces.end(); iter++) {

					rectangle(dts, *iter, cv::Scalar(255, 0, 0), 1);//検出個所に描画する内容
					//void rectangle(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
				}

/*****************************************顔面モザイク処理開始***********************************************************/
				if (p_t->gfr == 7){
					//半分にリサイズ
					cv::resize(ori, ori2, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
					max_x = max_y = 0;      //モザイク範囲初期化
					min_x = ori2.cols - 1; //モザイク範囲初期化
					min_y = ori2.rows - 1;//モザイク範囲初期化

					for (y = 0; y < dts.rows; y++){
						for (x = 0; x < dts.cols; x++){
							if (dts.data[y * dts.step + x * dts.channels() + 2] > 0 ||
								dts.data[y * dts.step + x * dts.channels() + 1] > 0 ||
								dts.data[y * dts.step + x * dts.channels() + 0] > 0)
							{
								//モザイク描画範囲決定処理
								if (max_x < x)max_x = x;
								if (max_y < y)max_y = y;
								if (min_x > x)min_x = x;
								if (min_y > y)min_y = y;

							}
						}
					}
					//ifアクセス違反防止
					if (dts.rows - max_y >= 10 && dts.rows - min_y >= 10 && dts.cols - max_x >= 10 && dts.cols - min_x >= 10){
						for (y = min_y; y <= max_y; y += 10){
							for (x = min_x; x <= max_x; x += 10){
								for (j = y; j < y + 10; j++){
									for (i = x; i < x + 10; i++){
										//モザイク描画処理
										dts.data[j * dts.step + i * dts.channels() + 2] =
											ori2.data[y * ori2.step + x * ori2.channels() + 2];
										dts.data[j * dts.step + i * dts.channels() + 1] =
											ori2.data[y * ori2.step + x * ori2.channels() + 1];
										dts.data[j * dts.step + i * dts.channels() + 0] =
											ori2.data[y * ori2.step + x * ori2.channels() + 0];

									}
								}
							}
						}
					}
				}
/*****************************************顔面モザイク処理終了***********************************************************/
				p_t->lock_t = 1;                                //検索スレッドロック
				if (p_t->lock == 1){p_t->lock_t = 0; continue;}//本スレロック時,検索スレッドロック解除後continue
				p_t->mt_temp = dts.clone();                   //本スレデータ引き渡し.clone()完全コピー
				p_t->lock_t = 0;                             //検索スレッドロック解除
				p_t->th_st = 1;//スレッド進行状況(NULLによるエラー防止)
			}//while検出部
		}
		catch (...){
			// "..." を指定すると try ブロック内で発生した全ての例外を捕捉します。このとき、例外を変数で受けることはできません。
			continue;
		}

	}//while(ずっとループしっぱなし)

	return 0;
}
