//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　顔面検出処理関数(並列スレッド)                        **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <process.h>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_lib.hpp>
#include "ImageRead.h"
#include "ImageDraw.h"

unsigned __stdcall face_detect(void *th){

	// 学習済み検出器の読み込み
	cv::string cascadeName = "dat\\xml\\haarcascade_frontalface_alt2.xml";
	cv::CascadeClassifier cascade;
	if (!cascade.load(cascadeName))
		return -1;

	ImageRead::drawdata_t d_t = (ImageRead::drawdata_t)th;//本スレッドからデータ受け取り(drawdataとdrawdata_tは中身は同じ)
	cv::vector<cv::Rect> faces;     //検出箇所
	cv::VideoCapture video;        //検出スレッド動画再生ハンドル
	cv::Mat ori, ori2;
	cv::Mat dts;
	cv::Mat grayImage;         //検出用グレースケールデータ格納用
	REFTIME time1;            //本スレッドと同期取り用変数
	int x, y, i, j, x1, y1;  //for

	typedef struct{
		int max_x;
		int max_y;
		int min_x;
		int min_y;
	}detect_range;
	detect_range dtr[200];//顔面検出範囲
	int dtrp;            //検出個数
	int dti;            //添え字
	int col, row;      //for
	int xs, ys;       //検出箇所元サイズ
	int xrs, yrs;    //検出箇所リサイズ後サイズ

	d_t->th_st = 0;         //スレッド進行状況初期化

	while (1){            //この関数が呼ばれたらずっとループしっぱなし 
		try{             //例外処理,機能してるか知らんが一応つけた。
			if (video.isOpened() == true){//動画ファイルがopenの場合
				video.release();         //動画ファイル閉じ
			}

			while (d_t->th_f == 1 && (d_t->gfr == 6 || d_t->gfr == 7 || d_t->gfr == 10)){//検出処理スタートフラグ,  検出処理選択

				switch (d_t->mcf){  //0:静止画選択中 1:動画選択中 2:カメラ選択中

					//静止画処理開始
				case 0:
					ori = cv::imread(d_t->g_name, CV_LOAD_IMAGE_COLOR);//画像読み込み

					break;
					//静止画処理終了

					//動画処理開始
				case 1:
					if (video.isOpened() == false){//動画ファイルcloseの場合
						video.open(d_t->g_name);  //動画ファイルopen
					}

					d_t->pMediaPosition->get_CurrentPosition(&time1);//再生位置取得(秒)
					video.set(CV_CAP_PROP_POS_MSEC, time1 * 1000);//映像同期(こっちはミリ秒単位なので1000かける)
					video >> ori;                         //フレーム読み込み
					if (ori.empty()){                    //フレームが空か？	
						video.release();
						video.open(d_t->g_name);
						video >> ori;
					}

					break;
					//動画処理終了

					//カメラ処理開始
				case 2:
					d_t->cap >> ori;
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

				/*****************************************顔面モザイク,顔すげ替え処理開始***********************************************************/
				if (d_t->gfr == 7 || d_t->gfr == 10){
					//半分にリサイズ
					cv::resize(ori, ori2, cv::Size(), 0.5, 0.5, cv::INTER_CUBIC);
					dtrp = 0;//検出個数初期化
					dti = 0;//添え字

					//検出範囲,個数探索処理開始
					for (row = 0; row < dts.rows; row++){
						for (col = 0; col < dts.cols; col++){

							//if255が入っているか
							if (dts.data[row * dts.step + col * dts.channels() + 2] == 0 &&
								dts.data[row * dts.step + col * dts.channels() + 1] == 0 &&
								dts.data[row * dts.step + col * dts.channels() + 0] == 255)
							{
								dtr[dti].max_x = dtr[dti].max_y = 0;
								dtr[dti].min_x = dts.cols - 1;
								dtr[dti].min_y = dts.rows - 1;
								x1 = col;
								y1 = row;
								while (1){//while1回抜けで1矩形終了
									//モザイク描画範囲決定処理
									if (dtr[dti].max_x < x1)dtr[dti].max_x = x1;
									if (dtr[dti].max_y < y1)dtr[dti].max_y = y1;
									if (dtr[dti].min_x > x1)dtr[dti].min_x = x1;
									if (dtr[dti].min_y > y1)dtr[dti].min_y = y1;

									//矩形の図形ラインをたどって範囲を探索,探索箇所を0に更新して255が無くなったら終わり
									if (y1 - 1 >= 0 &&
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 2] == 0 &&
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 1] == 0 &&
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 0] == 255)
									{
										dts.data[(y1 - 1) * dts.step + x1 * dts.channels() + 0] = 0;
										y1 -= 1;
										continue;
									}

									if (y1 + 1 < dts.rows &&
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 2] == 0 &&
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 1] == 0 &&
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 0] == 255)
									{
										dts.data[(y1 + 1) * dts.step + x1 * dts.channels() + 0] = 0;
										y1 += 1;
										continue;
									}

									if (x1 - 1 >= 0 &&
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 2] == 0 &&
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 1] == 0 &&
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 0] == 255)
									{
										dts.data[y1 * dts.step + (x1 - 1) * dts.channels() + 0] = 0;
										x1 -= 1;
										continue;
									}

									if (x1 + 1 < dts.cols &&
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 2] == 0 &&
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 1] == 0 &&
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 0] == 255)
									{
										dts.data[y1 * dts.step + (x1 + 1) * dts.channels() + 0] = 0;
										x1 += 1;
										continue;
									}
									//矩形の図形ライン1ピクセル分終了

									break;

								}//while1回抜けで1矩形終了

								//1矩形終了したので添え字1追加
								dti++;
								if (dti >= 200)break;//配列アクセス違反防止
							}//if255が入っているか
						}//for cols
						if (dti >= 200){ dti = 200; break; }//配列アクセス違反防止
					}//for rows
					dtrp = dti;//検出個数代入
					//検出範囲,個数探索処理終了

					if (d_t->gfr == 7){
						//検出箇所モザイク処理開始(1ループ1箇所)
						for (dti = 0; dti < dtrp; dti++){
							//ifアクセス違反防止
							if (dts.rows - dtr[dti].max_y >= 10 && dts.rows - dtr[dti].min_y >= 10 &&
								dts.cols - dtr[dti].max_x >= 10 && dts.cols - dtr[dti].min_x >= 10){
								for (y = dtr[dti].min_y; y <= dtr[dti].max_y; y += 10){
									for (x = dtr[dti].min_x; x <= dtr[dti].max_x; x += 10){
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
								}//forここまで
							}//ifアクセス違反防止
						}//for dti
						//検出箇所モザイク処理終了

					}//if gfr == 7

					if (d_t->gfr == 10){
						//検出箇所すげ替え処理開始
						for (dti = 0; dti < dtrp; dti++){

							//入れ替え元画像サイズ決定
							xs = dtr[dti].max_x - dtr[dti].min_x + 1;
							ys = dtr[dti].max_y - dtr[dti].min_y + 1;
							//入れ替え先画像サイズ決定
							xrs = dtr[dtrp - dti - 1].max_x - dtr[dtrp - dti - 1].min_x + 1;
							yrs = dtr[dtrp - dti - 1].max_y - dtr[dtrp - dti - 1].min_y + 1;

							//ifアクセス違反防止
							if (dts.rows > dtr[dti].min_y + ys &&
								dts.cols > dtr[dti].min_x + xs &&
								dts.rows > dtr[dtrp - dti - 1].min_y + yrs &&
								dts.cols > dtr[dtrp - dti - 1].min_x + xrs){

								for (y = 0; y < yrs; y++){
									for (x = 0; x < xrs; x++){

										dts.data[(y + dtr[dtrp - dti - 1].min_y) * dts.step +
											(x + dtr[dtrp - dti - 1].min_x) * dts.channels() + 2] =
											ori2.data[(y * ys / yrs + dtr[dti].min_y) * ori2.step +
											(x * xs / xrs + dtr[dti].min_x) * ori2.channels() + 2];

										dts.data[(y + dtr[dtrp - dti - 1].min_y) * dts.step +
											(x + dtr[dtrp - dti - 1].min_x) * dts.channels() + 1] =
											ori2.data[(y * ys / yrs + dtr[dti].min_y) * ori2.step +
											(x * xs / xrs + dtr[dti].min_x) * ori2.channels() + 1];

										dts.data[(y + dtr[dtrp - dti - 1].min_y) * dts.step +
											(x + dtr[dtrp - dti - 1].min_x) * dts.channels() + 0] =
											ori2.data[(y * ys / yrs + dtr[dti].min_y) * ori2.step +
											(x * xs / xrs + dtr[dti].min_x) * ori2.channels() + 0];

									}
								}
							}//ifアクセス違反
						}//for dti
						//検出箇所すげ替え処理終了
					}//if gfr == 10
				}//gfr==7,10処理終了
				/*****************************************顔面モザイク,顔すげ替え処理終了***********************************************************/
				d_t->lock_t = 1;                                //検索スレッドロック
				if (d_t->lock == 1){ d_t->lock_t = 0; continue; }//本スレロック時,検索スレッドロック解除後continue
				d_t->mt_temp = dts.clone();                   //本スレデータ引き渡し.clone()完全コピー
				d_t->lock_t = 0;                             //検索スレッドロック解除
				d_t->th_st = 1;//スレッド進行状況(NULLによるエラー防止)
				if (d_t->th_fin == 1)return 0;//スレッド抜け
			}//while検出部
		}
		catch (...){
			// "..." を指定すると try ブロック内で発生した全ての例外を捕捉します。このとき、例外を変数で受けることはできません。
			continue;
		}
		if (d_t->th_fin == 1)return 0;//スレッド抜け
	}//while(ずっとループしっぱなし)

	return 0;
}
