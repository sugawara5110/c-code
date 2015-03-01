//*****************************************************************************************//
//**                                                                                     **//
//**                   @@@@@”wŒi‰æ‘œˆ—                                            **//
//**                                                                                     **//
//*****************************************************************************************//

#include "DxLib.h"
#include <stdlib.h>
#include <time.h>

void back_image(int f){//f‘€ì—p

	static int hk;   //‰æ‘œƒnƒ“ƒhƒ‹
	int r;    //ƒ‰ƒ“ƒ_ƒ€
	
	srand((unsigned)time(NULL));
	
	if (f == 0){
        r = rand() % 5;
		if (r == 0)hk = LoadGraph("./dat/hk/hk0.dat");//”wŒi‰æ‘œ“Ç‚İ‚İ
		if (r == 1)hk = LoadGraph("./dat/hk/hk1.dat");
		if (r == 2)hk = LoadGraph("./dat/hk/hk2.dat");
		if (r == 3)hk = LoadGraph("./dat/hk/hk3.dat");
		if (r == 4)hk = LoadGraph("./dat/hk/hk4.dat");

		return ;
	}

	if (f == 1){

		DrawGraph(0, 0, hk, TRUE);//”wŒi‰æ‘œ•`‰æ
		
		return;
	}

}