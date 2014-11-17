//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　スライドパズル                                        **//
//**                                                                                     **//
//*****************************************************************************************//
#include "DxLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nekog[160000],tempx,tempy,g1;  //一時保管
int i,j,k,a;                         //for文
int frg=0,frga;                   //キー入力,フラグ
int Color;                       //色
int size=0;                      //paras添え字
int space[2];                  //スペース座標0:x 1:y

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
		
int mov(int *space,int frg,int autof){  //移動処理関数宣言
          int mv; //auto_matic関数用移動スピード
	  int mov_p=0; //移動実施判定用
	  k=-1; //初期化
	  for(i=0;i<paras[size].soeji;i++){    
	         //↓auto_matic()関数のブロック衝突判定処理
		 if(autof==1 && nekog[i]==1){k=-1;continue;}
	         //↓移動キー毎に全ブロックとスペースの座標比較,移動対象ブロック決定(添え字k)　
	         if(frg==1 && neko[i].nx==space[0]+paras[size].size_a && neko[i].ny==space[1]    ){k=i;break;} 
	         if(frg==2 && neko[i].nx==space[0]-paras[size].size_a && neko[i].ny==space[1]    ){k=i;break;}
	         if(frg==3 && neko[i].nx==space[0]     && neko[i].ny==space[1]+paras[size].size_a){k=i;break;}
	         if(frg==4 && neko[i].nx==space[0]     && neko[i].ny==space[1]-paras[size].size_a){k=i;break;}
	  }  //for終わり
	  

          if(k>=0){   //移動対象ブロック決定時のみ実行
               if(autof==0)mv=paras[size].move;
	       if(autof==1){mv=paras[size].move;DrawBox(0,0,640,50,0,TRUE);
	                    DrawFormatString( 0,16, Color, "[0]ハイスピードON" );}
	       if(autof==1 && CheckHitKey(KEY_INPUT_NUMPAD0)==1)mv=paras[size].size_a;
	        
 	       while( neko[k].nx!=space[0] || neko[k].ny!=space[1]){   //移動中座標更新処理,スペース座標に到達したら終了)
	        
	            
	            if(frg==1){neko[k].nx-=mv; //座標値更新
		      if(size!=5){      //size==0～4の処理(size==5は1回で移動完了の為、ラストのみ処理)
			DrawBox(neko[k].nx+paras[size].size_a,space[1],     //移動元のスペース画像処理
			        space[0]+paras[size].size_a*2,space[1]+paras[size].size_a,
				0,TRUE);
		        DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );//移動ブロック画像処理
		      } 
		    }
		    
		    if(frg==2){neko[k].nx+=mv;
		      if(size!=5){
		        DrawBox(space[0]-paras[size].size_a,space[1],neko[k].nx-1,neko[k].ny+paras[size].size_a,0,TRUE);
			DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		      } 
		    }
		    
		   if(frg==3){neko[k].ny-=mv;
		     if(size!=5){
			DrawBox(space[0],neko[k].ny+paras[size].size_a,
			        space[0]+paras[size].size_a,space[1]+paras[size].size_a*2,
				0,TRUE);
		        DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		     } 
	           }
		   
		   if(frg==4){neko[k].ny+=mv;
		     if(size!=5){
		        DrawBox(space[0],space[1]-paras[size].size_a,space[0]+paras[size].size_a,neko[k].ny-1,0,TRUE);
			DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		     }
		   } 
		  ScreenFlip(); //表画面描写
		   
		    
	       }//while終わり
	        
		
		//↓移動完了後スペース座標更新
		if(frg==1)space[0]+=paras[size].size_a;
		if(frg==2)space[0]-=paras[size].size_a;
		if(frg==3)space[1]+=paras[size].size_a;
		if(frg==4)space[1]-=paras[size].size_a;
		if(size!=5){    //スペース座標==画像座標の時点でwhile終了の為、==時の画像処理
		   DrawBox(space[0],space[1],space[0]+paras[size].size_a,space[1]+paras[size].size_a,0,TRUE);
		   DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		}else{
		   DrawPixel( space[0],space[1],0 );    //size==5は1回で処理完了の為表示1回のみ
		   DrawPixel( neko[k].nx,neko[k].ny, neko[k].ng );
		 }
          } //if終わり
	  if(k>=0)mov_p=1;
	  k=-1; //移動完了後kを初期化
	  return mov_p;	       
 }  //mov()終わり
 


void auto_space_mov(int *space,int *x,int *y){//スペースを目的に隣接するまで自動移動
     int f;//mov()結果
     int s=paras[size].size_a;
     while(1){
          if((space[0]==*x && space[1]==*y-s)||(space[0]==*x+s && space[1]==*y-s)|| //目的地に隣接
             (space[0]==*x+s && space[1]==*y)||(space[0]==*x+s && space[1]==*y+s)||
	     (space[0]==*x && space[1]==*y+s)||(space[0]==*x-s && space[1]==*y+s)||
	     (space[0]==*x-s && space[1]==*y)||(space[0]==*x-s && space[1]==*y-s))break;
        
          if(space[0]-*x>0)f=mov(space,2,1);//スペース右位置関係別に引数を変えmov関数を呼び出す
	  if(space[0]-*x>0 && f==0){mov(space,3,1);continue;} 
	  if(space[0]-*x<0)mov(space,1,1);//スペース左
	  if(space[1]-*y>0)mov(space,4,1);//スペース下 
          if(space[1]-*y<0)mov(space,3,1);//スペース上
           
     }//while終わり
}//auto_space_mov終わり
                                


 
void auto_round(int *x,int *y,int cx,int cy){ //スペース回転,ブロック移動関数
     int s=paras[size].size_a;
     int fr=0; //回転方向 0:時計 0以外:反時計 movp==0(移動未実施)の度切り替わる
     int movp=0;//回転方向切換フラグ
     auto_space_mov(space,x,y);
     
     while( *x!=cx || *y!=cy){
          if(space[0]==*x && space[1]==*y-s){       //スペース上
	    if(*y-cy>0){mov(space,3,1);continue;}//目的地上方向
	    if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,1,1);else movp=mov(space,2,1); //回転方向分岐,右,左
	  }
          if(space[0]==*x+s && space[1]==*y-s){     //スペース右上
            if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,3,1);else movp=mov(space,2,1); //回転方向分岐,下,左
	  }
          if(space[0]==*x+s && space[1]==*y){       //スペース右
	    if(*x-cx<0){mov(space,2,1);continue;}//目的地右方向
	    if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,3,1);else movp=mov(space,4,1); //回転方向分岐,下,上
	  }
          if(space[0]==*x+s && space[1]==*y+s){     //スペース右下
	    if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,2,1);else movp=mov(space,4,1); //回転方向分岐,左,上
	  }
          if(space[0]==*x && space[1]==*y+s){       //スペース下
	    if(*y-cy<0){mov(space,4,1);continue;}//目的地下方向
	    if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,2,1);else movp=mov(space,1,1); //回転方向分岐,左,右
	  }
          if(space[0]==*x-s && space[1]==*y+s){     //スペース左下
	    if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,4,1);else movp=mov(space,1,1); //回転方向分岐,上,右
	  }
          if(space[0]==*x-s && space[1]==*y){       //スペース左
	    if(*x-cx>0){mov(space,1,1);continue;}//目的地左方向
	    if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,4,1);else movp=mov(space,3,1); //回転方向分岐,上,下
	  }
          if(space[0]==*x-s && space[1]==*y-s){     //スペース左上
	    if(movp==0)fr=!fr;  //移動未実施時,回転方向切換
	    if(fr==0)movp=mov(space,1,1);else movp=mov(space,3,1); //回転方向分岐,右,下
	  }
	  
          
      } //while終わり
     


}//auto_round()終わり


                     
void auto_matic(int *space,int tateyoko,int range,int j){    //オート関数宣言
              if(range==tateyoko){ //初期化
                for(a=0;a<paras[size].soeji;a++){ 
                    nekog[a]=0; //mov()関数内の移動禁止フラグ
                } 
              } 
       
              
        huka1:   
	      //↓範囲内上1列1回目処理 
              for(a=j+1;a<range+j;a++){  
	          auto_round(&neko[a].nx,&neko[a].ny,nekocopy[a-1].nx,nekocopy[a-1].ny); 
	          nekog[a]=1;	//移動禁止ON
	      } //for終わり
	 	      
	      //↓移動不可状態処理
	      if((neko[j].nx==nekocopy[range+j-1].nx && neko[j].ny==nekocopy[range+j-1].ny)||
	         (neko[j].nx==nekocopy[range+j-1+tateyoko].nx && neko[j].ny==nekocopy[range+j-1+tateyoko].ny &&
	          space[0]==nekocopy[range+j-1].nx && space[1]==nekocopy[range+j-1].ny )){
		  for(a=j+1;a<range+j;a++){
		    nekog[a]=0;
		  }
		  auto_round(&neko[j].nx,&neko[j].ny,nekocopy[range+j-1+tateyoko*2].nx,nekocopy[range+j-1+tateyoko*2].ny);
	          goto huka1;}	  
	      
	      //↓範囲内上1列左1個処理
	      auto_round(&neko[j].nx,&neko[j].ny,nekocopy[j+tateyoko].nx,nekocopy[j+tateyoko].ny);
	      nekog[j]=1;  //移動禁止ON
	      
	      //↓範囲内上1列2回目処理
	      for(a=range+j-1;a>=j+1;a--){
	          nekog[a]=0;        //移動前移動禁止OFF
		  auto_round(&neko[a].nx,&neko[a].ny,nekocopy[a].nx,nekocopy[a].ny);
		  nekog[a]=1;        //移動後移動禁止ON
	      }//for終わり
	      
	      if(range==2){nekog[j]=0;mov(space,3,1);mov(space,1,1);mov(space,1,1);return;}      //完了
	      
	      //↓範囲内上1列左1個処理
	      nekog[j]=0;            //移動前移動禁止OFF
	      auto_round(&neko[j].nx,&neko[j].ny,nekocopy[j].nx,nekocopy[j].ny);
              nekog[j]=1;            //移動後移動禁止ON
	      
	      
	      j+=tateyoko;         //for文初期値変更
	      range--;             //範囲変更
              
	      
	huka2:     
	      //↓範囲内左1列1回目処理
	      for(a=j+tateyoko;a<=j+tateyoko*(range-1);a+=tateyoko){
	          auto_round(&neko[a].nx,&neko[a].ny,nekocopy[a-tateyoko].nx,nekocopy[a-tateyoko].ny);
	          nekog[a]=1;  //移動禁止ON
	      } //for終わり
              
	      //↓移動不可状態処理
	      if((neko[j].nx==nekocopy[j+tateyoko*(range-1)].nx && neko[j].ny==nekocopy[j+tateyoko*(range-1)].ny)||
	         (neko[j].nx==nekocopy[j+tateyoko*(range-1)+1].nx && neko[j].ny==nekocopy[j+tateyoko*(range-1)+1].ny &&
		  space[0]==nekocopy[j+tateyoko*(range-1)].nx && space[1]==nekocopy[j+tateyoko*(range-1)].ny)){
		  for(a=j+tateyoko;a<=j+tateyoko*(range-1);a+=tateyoko){
		  nekog[a]=0;
		  }
		  auto_round(&neko[j].nx,&neko[j].ny,nekocopy[j+tateyoko*(range-1)+2].nx,nekocopy[j+tateyoko*(range-1)+2].ny);
		  goto huka2;} 
	      
	      //↓範囲内左1列上1個処理
	      auto_round(&neko[j].nx,&neko[j].ny,nekocopy[j+1].nx,nekocopy[j+1].ny);
	      nekog[j]=1;         //移動禁止ON
	      //↓範囲内左1列2回目処理
	      for(a=j+tateyoko*(range-1);a>=j+tateyoko;a-=tateyoko){
	          nekog[a]=0;   //移動禁止OFF
		  auto_round(&neko[a].nx,&neko[a].ny,nekocopy[a].nx,nekocopy[a].ny);
		  nekog[a]=1;   //移動禁止ON
	      }//for終わり
	      //↓範囲内左1列上1個処理
	      nekog[j]=0;  //移動禁止OFF
	      auto_round(&neko[j].nx,&neko[j].ny,nekocopy[j].nx,nekocopy[j].ny);
              nekog[j]=1;  //移動禁止ON
	      
	      j++; //for文初期値変更
         //↓再帰呼び出し  
         auto_matic(space,tateyoko,range,j);

}//auto_matic()終わり
     
    


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
        for(i=0;i<paras[size].rand_i;i++){         // nxy[]内でnx,nyをランダムでデータ入れ替え
	                           //size==5の場合の処理(rand関数max値が足りない為)
	    if(size==5){j=(rand()%10000)*15+rand()%10014;k=(rand()%10000)*15+rand()%10014;
	    }else{                 //size==0～4の場合の処理
	      j=rand()%paras[size].rand_s;   //交換元添え字j
	      k=rand()%paras[size].rand_s;   //交換先添え字k
	      if(j==k){i--;continue;}
	     }  
	     tempx=nxy[j].nx;    //交換
	     tempy=nxy[j].ny;
	     nxy[j].nx=nxy[k].nx;
	     nxy[j].ny=nxy[k].ny;
	     nxy[k].nx=tempx;
	     nxy[k].ny=tempy;
	} //for終わり  		
     
}   //shuffle()終わり 


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

	space[0]=paras[size].lastposx;       //スペースx座標初期化
	space[1]=paras[size].lastposy;       //スペースy座標初期化
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
	        DrawBox(0,0,500,16,0,TRUE);
	        DrawFormatString( 0, 0, Color, "アクティブモードOFF [右エンター]切り替え") ;
	          }else {DrawBox(0,0,500,16,0,TRUE);
		         DrawFormatString( 0, 0, Color, "アクティブモードON [右エンター]切り替え") ;
	                }
	       DrawBox(0,16,500,50,0,TRUE); 
	       DrawFormatString( 0, 32, Color, "[3]オートモードスタート ") ;
               DrawFormatString( 0, 17, Color, "[1]元通り　[2]シャッフル  [6]ブロック個数変更　[↑↓→←]ブロック移動") ;
	       ScreenFlip();//表画面描写
	       
	       if( CheckHitKey( KEY_INPUT_LEFT )==1)frg=1;      //左  ブロック移動キー検出
	       if( CheckHitKey( KEY_INPUT_RIGHT)==1)frg=2;      //右
	       if( CheckHitKey( KEY_INPUT_UP )==1)frg=3;         //上
	       if( CheckHitKey( KEY_INPUT_DOWN )==1)frg=4;      //下
	      
	       if( CheckHitKey( KEY_INPUT_NUMPAD3)==1)auto_matic(space,paras[size].tateyoko,paras[size].tateyoko,0);
	          
                   
		 
		  
	      
	      
	       if( CheckHitKey( KEY_INPUT_NUMPAD1)==1){               //全ブロック位置元通り処理↓
	           
	           for(i=0;i<paras[size].soeji;i++){nxy[i]=nekocopy[i];} //完成画像データからコピー
	           narabikae();                   //並び替え関数
		   space[0]=paras[size].lastposx;    //データ初期化
		   space[1]=paras[size].lastposy;
		   frg=0;
		   k=-1; 
		   frga=0;
	       }   //if終わり
	       
	       if( CheckHitKey( KEY_INPUT_NUMPAD2)==1){        //更に並び替え
	         
	           shuffle();       //シャッフル関数
		   narabikae();       //並び替え関数
		   space[0]=paras[size].lastposx;   //データ初期化
		   space[1]=paras[size].lastposy;
		   frg=0;
		   k=-1; 
		   frga=0;  
	       }   //if終わり
	     }while( CheckHitKeyAll() == 0 && frga==0 ); //do-while終わり アクティブモードON(frga!=0)の場合連続で移動処理実行
	    }  //if終わり  
             
	     
	      
	    
	     
	     mov(space,frg,0); //移動処理
	     
	     
	        
            if(frga==0)frg=0; //移動完了後アクティブモードoff時frg=0に更新させ連続移動させない
	    
	 
	 }  //while終わり
        
        DxLib_End(); // DXライブラリ終了処理
        return 0;
}     // winmain終わり

//********************************************************************************************************//
//**** libjpeg　Copyright (C) 1991-2013, Thomas G. Lane, Guido Vollbeding.  ******************************//
//**** DX Library Copyright (C) 2001-2014 Takumi Yamada.   ***********************************************//
//********************************************************************************************************//