#include "DxLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nekog[160000],tempx,tempy,g1;  //一時保管
int i,j,k;                         //for文
int frg=0,frga;                   //キー入力,フラグ
int Color;                       //色
int size=0;                      //paras添え字
int spacex,spacey;              //スペース座標

typedef struct {  //画像座標宣言 
   int nx;        //x座標
   int ny;        //y座標
   int ng;        //画像データ及びピクセル色情報
} nekoxy; 

typedef struct {   //ブロック個数毎のパラメーター宣言
   int soeji;      //neko nxy nekocopy 添え字
   int tateyoko;   //ブロック縦,横方向個数
   int size_a;     //ブロックサイズ
   int lastposx;   //右下のx座標
   int lastposy;   //右下のy座標
   int rand_i;     //ランダム発生回数
   int rand_s;     //ランダム範囲
   int frgp;       //narabikae関数の移動完了ブロック判定
   int move;       //ブロック移動量
} para;  

 nekoxy neko[160000];           //各画像現座標
 nekoxy nxy[160000];            //各画像移動先座標(narabikae関数で使用)
 nekoxy nekocopy[160000];       //完成状態のコピー
 para paras[6]={{16,4,100,420,350,500,15,31,5},           //ブロック個数毎のパラメーター
                {64,8,50,470,400,2000,63,127,5},
		{256,16,25,495,425,10000,255,511,5},
		{1600,40,10,510,440,2000,1599,3199,2},
		{10000,100,4,516,446,9000,9999,19999,2},
		{160000,400,1,519,449,200000,159999,319999,1} 
		}; 
		


void narabikae(){              //並び替え関数宣言

	frg=0;
	while( frg<paras[size].frgp && ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0  ){
           frg=0;   //for文(soeji-1)回終了時点でfrg規定値か否か判定する為for文開始前は初期化
	    
	   for(i=0;i<paras[size].soeji-1;i++){                   //ブロック個数分繰り返し
	     if(size==5){                                       //size==5の場合の処理
	        DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng ); //size==5の場合の処理
	     }else{	        
	         DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE );//size==0～4の場合の処理 
	      }
	     if(neko[i].nx==nxy[i].nx){frg++;}
	        else if((nxy[i].nx-neko[i].nx)<0){neko[i].nx-=paras[size].move;}  //x移動方向判別,frg規定値になるまで処理
		   else if((nxy[i].nx-neko[i].nx)>0){neko[i].nx+=paras[size].move;}
		   
	     if(neko[i].ny==nxy[i].ny){frg++;}
	        else if((nxy[i].ny-neko[i].ny)<0){neko[i].ny-=paras[size].move;}  //y移動方向判別,frg規定値になるまで処理
		   else if((nxy[i].ny-neko[i].ny)>0){neko[i].ny+=paras[size].move;}
	     
	     if(size==5){                                        //size==5の場合の処理
	        DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng );  //size==5の場合の処理
	     }else{	        	    
	       DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE );//size==0～4の場合の処理
	      }
	      
           }   //for終わり
	   
	   if(size==5){     //size==5の場合の処理
	        DrawPixel( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng );
	     }else{	    //size==0～4の場合の処理    
	      DrawGraph( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng, TRUE ); 
	     }
	   if(neko[paras[size].soeji-1].nx==520){frg++;}                              //右下画像右移動
	      else {neko[paras[size].soeji-1].nx+=paras[size].move;} 
           if(size==5){     //size==5の場合の処理
	        DrawPixel( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng );
	     }else{	    //size==0～4の場合の処理    
	      DrawGraph( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng, TRUE ); 
              }
	      
	}    //while終わり
	
}  //narabikae()終わり	


void shuffle(){   //シャッフル関数宣言
     
     if(size<=2){       //size==0～2までの処理
        for(i=0;i<paras[size].soeji;i++){nxy[i]=nekocopy[i];} //完成画像データからコピー
	neko[paras[size].soeji-1].nx=520;
        spacex=paras[size].lastposx; //スペース座標初期化
	spacey=paras[size].lastposy;  
       for(j=0;j<paras[size].rand_i;j++){  
          k=rand()%4;  //ランダムで4方向決定
	  
	 for(i=0;i<paras[size].rand_s;i++){        
	   if(k==0 && nxy[i].nx==spacex-paras[size].size_a && nxy[i].ny==spacey) //スペース座標隣接ブロック探索
	   {
	      tempx=nxy[i].nx;nxy[i].nx=spacex;spacex=tempx;                     //スペース座標,隣接ブロック座標入れ替え
	      tempy=nxy[i].ny;nxy[i].ny=spacey;spacey=tempy;
	   }
	   if(k==1 && nxy[i].nx==spacex+paras[size].size_a && nxy[i].ny==spacey)  
	   {
	      tempx=nxy[i].nx;nxy[i].nx=spacex;spacex=tempx;
	      tempy=nxy[i].ny;nxy[i].ny=spacey;spacey=tempy;
	   }
	   if(k==2 && nxy[i].ny==spacey-paras[size].size_a && nxy[i].nx==spacex)
	   {
	      tempx=nxy[i].nx;nxy[i].nx=spacex;spacex=tempx;
	      tempy=nxy[i].ny;nxy[i].ny=spacey;spacey=tempy;
	   }
	   if(k==3 && nxy[i].ny==spacey+paras[size].size_a && nxy[i].nx==spacex)
	   {
	      tempx=nxy[i].nx;nxy[i].nx=spacex;spacex=tempx;
	      tempy=nxy[i].ny;nxy[i].ny=spacey;spacey=tempy;
	   } 
	 }//for終わり 
       } //for終わり
     
       do{    //for文終了時のspacex,spaceyの座標をlastposx,lastposyまでブロック交換しながら移動させる
          for(i=0;i<paras[size].soeji-1;i++){ 
	   if(nxy[i].nx==spacex+paras[size].size_a && nxy[i].ny==spacey ){tempx=nxy[i].nx;nxy[i].nx=spacex;spacex=tempx;}   
	  }      
       }while(spacex<paras[size].lastposx);   
       
       do{
         for(i=0;i<paras[size].soeji-1;i++){ 
           if(nxy[i].ny==spacey+paras[size].size_a && nxy[i].nx==spacex){tempy=nxy[i].ny;nxy[i].ny=spacey;spacey=tempy;}
	 }     
       }while(spacey<paras[size].lastposy);
      
      }else{  
        for(i=0;i<=paras[size].rand_i;i++){         // nxy[]内でnx,nyをランダムでデータ入れ替え,size==3～5の処理
	                           //size==5の場合の処理(rand関数max値が足りない為)
	    if(size==5){j=(rand()%10000)*15+rand()%10014;k=(rand()%10000)*15+rand()%10014;
	    }else{                 //size==0～4の場合の処理
	      j=rand()%paras[size].rand_s;   //交換元添え字j
	      k=rand()%paras[size].rand_s;   //交換先添え字k
	     }  
	     tempx=nxy[j].nx;              //交換
	     tempy=nxy[j].ny;
	     nxy[j].nx=nxy[k].nx;
	     nxy[j].ny=nxy[k].ny;
	     nxy[k].nx=tempx;
	     nxy[k].ny=tempy;
	} //for終わり  		
     }
}   //シャッフル関数終わり 

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){    //メイン関数
        ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen( DX_SCREEN_BACK ); //ウィンドウモード変更と初期化と裏画面設定
        
	srand((unsigned)time(NULL));
	Color = GetColor( 255 , 255 , 255 ) ;   //字の色
start:         //gotoラベル
	  ClearDrawScreen(); //画面消去
	  DrawFormatString( 0,0, Color, "0～5キーを押してブロック個数の決定" ) ;
	  DrawFormatString( 0,16, Color, "[0].16個  [1].64個  [2].256個  [3].1600個  [4].10000個 " ) ;
	  DrawFormatString( 0,36, Color, "[5].160000個(初期処理に2分程かかります・・・) " ) ;
	  ScreenFlip();  //表画面に描写
	
	while( ProcessMessage()==0 && frg==0 ){   //個数決定
	       if( CheckHitKey(KEY_INPUT_NUMPAD0)==1){frg=1;size=0;}      
	       if( CheckHitKey(KEY_INPUT_NUMPAD1)==1){frg=1;size=1;}      
	       if( CheckHitKey(KEY_INPUT_NUMPAD2)==1){frg=1;size=2;}         
	       if( CheckHitKey(KEY_INPUT_NUMPAD3)==1){frg=1;size=3;}      
	       if( CheckHitKey(KEY_INPUT_NUMPAD4)==1){frg=1;size=4;}
	       if( CheckHitKey(KEY_INPUT_NUMPAD5)==1){frg=1;size=5;}  
	}      //while終わり
	
	    
	   if(size==5){ InitGraph();           //size==5の場合の処理
	       g1= LoadGraph( "nekokin.jpg" ); // 400×400の画像のみ対応
	       
	       k=0;
	       for(j=50;j<450;j++){
	          
		  for(i=120;i<520;i++){
	            DrawGraph( 120,50,g1, TRUE );    //GetPixel関数で色情報を読む為の描写
		    nekog[k++]= GetPixel( i , j );   //size==5のみピクセル単位で色情報取得(GetPixel関数は時間がかかる)
		  }
	           ClearDrawScreen();
		   DrawFormatString( 0,0, Color, "%d /160000終了・・・",(j-49)*400 ) ;
		   ScreenFlip(); //表画面描写
	       }   
	   }else{                               //size==0～4の場合の処理
	         LoadDivGraph( "nekokin.jpg" ,paras[size].soeji,paras[size].tateyoko,paras[size].tateyoko,
	                       paras[size].size_a,paras[size].size_a, nekog );          //size==0～4,分割で画像読み込み
	        }
		
	  for(i=0;i<paras[size].soeji;i++){      //構造体に画像データ順番に格納　
	   neko[i].ng=nekog[i];
	  }
	  k=0;
	  for(j=50;j<=paras[size].lastposy;j+=paras[size].size_a){    //構造体に座標データ順番に格納
	    for(i=120;i<=paras[size].lastposx;i+=paras[size].size_a){
	     neko[k].nx=i;
	     neko[k++].ny=j;
	    }
          }
	  for(i=0;i<paras[size].soeji;i++){   //データコピー
	    nxy[i]=neko[i];
	    nekocopy[i]=neko[i];
	    
	  }
 
          ClearDrawScreen();  //画面消去
	  DrawFormatString( 0,16, Color, "ブロック個数%d個",paras[size].soeji ) ;
	  DrawFormatString( 0,0, Color, "右エンターキーを押してください" ) ;
	   
	  for(i=0;i<paras[size].soeji;i++){
	    if(size==5){           //size=5の場合の処理
	        DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng ); //初期画像描写size==5
	     }else{	           //size=0～4の場合の処理
	       DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE ); //初期画像描画size==0～4
              }
	  }
	  ScreenFlip(); //表画面描写
	
        while(ProcessMessage()==0 && CheckHitKey( KEY_INPUT_NUMPADENTER ) == 0 );//キー入力待ち
	
	shuffle();    //シャッフル関数
        narabikae();  //並び替え関数

	spacex=paras[size].lastposx;       //スペースx座標初期化
	spacey=paras[size].lastposy;       //スペースy座標初期化
	frg=0;           //移動キー情報
	k=-1;            //移動対象ブロック添え字
	frga=0;          //アクティブモード切替キー情報 
	
	ClearDrawScreen();  //画面消去
        for(i=0;i<paras[size].soeji;i++){  //初期画像描写	
	  if(size==5){         //size==5の場合の処理
	     DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng );
	  }else{	           //size==0～4の場合の処理                                  
	     DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE );
	   }
	     
	}      
	ScreenFlip();  //表画面描写
	
	while( ProcessMessage()==0  ){
          if(k<0){     //k>=0の場合ブロック移動中の為,移動キー入力中止　
	    do{   //do-while開始,押しっぱなしでもブロック移動させる為に初回は無条件に実行(CheckHitKeyAll() == 0ではじかれる為) 
	       if( CheckHitKey( KEY_INPUT_NUMPADENTER )==1){frga=!frga;} //アクティブモードフラグ切り替え
	       if( CheckHitKey(KEY_INPUT_NUMPAD6)==1){InitGraph();goto start;} //読み込み済みグラフィックデータ削除,goto
               
	       if(frga==0){           //モード表示切替
	        DrawBox(0,0,200,14,0,TRUE);
	        DrawFormatString( 0, 0, Color, "アクティブモードOFF") ;
	          }else {DrawBox(0,0,200,14,0,TRUE);
		         DrawFormatString( 0, 0, Color, "アクティブモードON") ;
	                }
	        
	       DrawFormatString( 0, 30, Color, "[1]キー元通り　[2]キー再並び替え　[6]ブロック個数変更　矢印キーブロック移動") ;
               DrawFormatString( 0, 15, Color, "右エンターキー切り替え") ;
	       ScreenFlip();//表画面描写
	       
	       if( CheckHitKey( KEY_INPUT_LEFT )==1)frg=1;      //左  ブロック移動キー検出
	       if( CheckHitKey( KEY_INPUT_RIGHT)==1)frg=2;      //右
	       if( CheckHitKey( KEY_INPUT_UP )==1)frg=3;         //上
	       if( CheckHitKey( KEY_INPUT_DOWN )==1)frg=4;      //下
	      
	      
	      
	      
	       if( CheckHitKey( KEY_INPUT_NUMPAD1)==1){               //全ブロック位置元通り処理↓
	           
	           for(i=0;i<paras[size].soeji;i++){nxy[i]=nekocopy[i];} //完成画像データからコピー
	           narabikae();                   //並び替え関数
		   spacex=paras[size].lastposx;    //データ初期化
		   spacey=paras[size].lastposy;
		   frg=0;
		   k=-1; 
		   frga=0;
	       }   //if終わり
	       
	       if( CheckHitKey( KEY_INPUT_NUMPAD2)==1){        //更に並び替え
	         
	           shuffle();       //シャッフル関数
		   narabikae();       //並び替え関数
		   spacex=paras[size].lastposx;   //データ初期化
		   spacey=paras[size].lastposy;
		   frg=0;
		   k=-1; 
		   frga=0;  
	       }   //if終わり
	     }while( CheckHitKeyAll() == 0 && frga==0 ); //do-while終わり アクティブモードON(frga==1)の場合連続で移動処理実行
	    }  //if終わり  
             for(i=0;i<paras[size].soeji;i++){    
	       
	       //↓移動キー毎に全ブロックとスペースの座標比較,移動対象ブロック決定(添え字k)　
	       if(frg==1 && neko[i].nx==spacex+paras[size].size_a && neko[i].ny==spacey    ){k=i;} 
	       if(frg==2 && neko[i].nx==spacex-paras[size].size_a && neko[i].ny==spacey    ){k=i;}
	       if(frg==3 && neko[i].nx==spacex     && neko[i].ny==spacey+paras[size].size_a){k=i;}
	       if(frg==4 && neko[i].nx==spacex     && neko[i].ny==spacey-paras[size].size_a){k=i;}
	     }  //for終わり
	     
	      //↓移動処理
	     if(k>=0){   //移動対象ブロック決定時のみ実行
	      
	       while( neko[k].nx!=spacex || neko[k].ny!=spacey){   //移動中座標更新処理,スペース座標に到達したら終了)
	        
	            
	            if(frg==1){neko[k].nx-=paras[size].move; //座標値更新
		      if(size!=5){      //size==0～4の処理(size==5は1回で移動完了の為、ラストのみ処理)
			DrawBox(neko[k].nx+paras[size].size_a,spacey,     //移動元のスペース画像処理
			        spacex+paras[size].size_a*2,spacey+paras[size].size_a,
				0,TRUE);
		        DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );//移動ブロック画像処理
		      } 
		    }
		    
		    if(frg==2){neko[k].nx+=paras[size].move;
		      if(size!=5){
		        DrawBox(spacex-paras[size].size_a,spacey,neko[k].nx-1,neko[k].ny+paras[size].size_a,0,TRUE);
			DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		      } 
		    }
		    
		   if(frg==3){neko[k].ny-=paras[size].move;
		     if(size!=5){
			DrawBox(spacex,neko[k].ny+paras[size].size_a,
			        spacex+paras[size].size_a,spacey+paras[size].size_a*2,
				0,TRUE);
		        DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		     } 
	           }
		   
		   if(frg==4){neko[k].ny+=paras[size].move;
		     if(size!=5){
		        DrawBox(spacex,spacey-paras[size].size_a,spacex+paras[size].size_a,neko[k].ny-1,0,TRUE);
			DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		     }
		   } 
		  ScreenFlip(); //表画面描写
		   
		    
	       }//while終わり
	        
		
		//↓移動完了後スペース座標更新
		if(frg==1)spacex+=paras[size].size_a;
		if(frg==2)spacex-=paras[size].size_a;
		if(frg==3)spacey+=paras[size].size_a;
		if(frg==4)spacey-=paras[size].size_a;
		if(size!=5){    //スペース座標==画像座標の時点でwhile終了の為、==時の画像処理
		   DrawBox(spacex,spacey,spacex+paras[size].size_a,spacey+paras[size].size_a,0,TRUE);
		   DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		}else{
		   DrawPixel( spacex,spacey,0 );    //size==5は1回で処理完了の為表示1回のみ
		   DrawPixel( neko[k].nx,neko[k].ny, neko[k].ng );
		 }
		if(frga==0)frg=0; //移動完了後アクティブモードoff時frg=0に更新させ連続移動させない
	        k=-1; //移動完了後kを初期化
	     } //if終わり 
	      
	       
	     
	    
	 
	 
	 
	 }  //while終わり
        
        DxLib_End(); // DXライブラリ終了処理
        return 0;
}     // winmain終わり