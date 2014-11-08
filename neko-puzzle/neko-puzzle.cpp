#include "DxLib.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nekog[160000],tempx,tempy,g1;  //�ꎞ�ۊ�
int i,j,k;                         //for��
int frg=0,frga;                   //�L�[����,�t���O
int Color;                       //�F
int size=0;                      //paras�Y����
int spacex,spacey;              //�X�y�[�X���W

typedef struct {  //�摜���W�錾 
   int nx;        //x���W
   int ny;        //y���W
   int ng;        //�摜�f�[�^�y�уs�N�Z���F���
} nekoxy; 

typedef struct {   //�u���b�N�����̃p�����[�^�[�錾
   int soeji;      //neko nxy nekocopy �Y����
   int tateyoko;   //�u���b�N�c,��������
   int size_a;     //�u���b�N�T�C�Y
   int lastposx;   //�E����x���W
   int lastposy;   //�E����y���W
   int rand_i;     //�����_��������
   int rand_s;     //�����_���͈�
   int frgp;       //narabikae�֐��̈ړ������u���b�N����
   int move;       //�u���b�N�ړ���
} para;  

 nekoxy neko[160000];           //�e�摜�����W
 nekoxy nxy[160000];            //�e�摜�ړ�����W(narabikae�֐��Ŏg�p)
 nekoxy nekocopy[160000];       //������Ԃ̃R�s�[
 para paras[6]={{16,4,100,420,350,500,15,31,5},           //�u���b�N�����̃p�����[�^�[
                {64,8,50,470,400,2000,63,127,5},
		{256,16,25,495,425,10000,255,511,5},
		{1600,40,10,510,440,2000,1599,3199,2},
		{10000,100,4,516,446,9000,9999,19999,2},
		{160000,400,1,519,449,200000,159999,319999,1} 
		}; 
		


void narabikae(){              //���ёւ��֐��錾

	frg=0;
	while( frg<paras[size].frgp && ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0  ){
           frg=0;   //for��(soeji-1)��I�����_��frg�K��l���ۂ����肷���for���J�n�O�͏�����
	    
	   for(i=0;i<paras[size].soeji-1;i++){                   //�u���b�N�����J��Ԃ�
	     if(size==5){                                       //size==5�̏ꍇ�̏���
	        DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng ); //size==5�̏ꍇ�̏���
	     }else{	        
	         DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE );//size==0�`4�̏ꍇ�̏��� 
	      }
	     if(neko[i].nx==nxy[i].nx){frg++;}
	        else if((nxy[i].nx-neko[i].nx)<0){neko[i].nx-=paras[size].move;}  //x�ړ���������,frg�K��l�ɂȂ�܂ŏ���
		   else if((nxy[i].nx-neko[i].nx)>0){neko[i].nx+=paras[size].move;}
		   
	     if(neko[i].ny==nxy[i].ny){frg++;}
	        else if((nxy[i].ny-neko[i].ny)<0){neko[i].ny-=paras[size].move;}  //y�ړ���������,frg�K��l�ɂȂ�܂ŏ���
		   else if((nxy[i].ny-neko[i].ny)>0){neko[i].ny+=paras[size].move;}
	     
	     if(size==5){                                        //size==5�̏ꍇ�̏���
	        DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng );  //size==5�̏ꍇ�̏���
	     }else{	        	    
	       DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE );//size==0�`4�̏ꍇ�̏���
	      }
	      
           }   //for�I���
	   
	   if(size==5){     //size==5�̏ꍇ�̏���
	        DrawPixel( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng );
	     }else{	    //size==0�`4�̏ꍇ�̏���    
	      DrawGraph( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng, TRUE ); 
	     }
	   if(neko[paras[size].soeji-1].nx==520){frg++;}                              //�E���摜�E�ړ�
	      else {neko[paras[size].soeji-1].nx+=paras[size].move;} 
           if(size==5){     //size==5�̏ꍇ�̏���
	        DrawPixel( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng );
	     }else{	    //size==0�`4�̏ꍇ�̏���    
	      DrawGraph( neko[paras[size].soeji-1].nx,neko[paras[size].soeji-1].ny, neko[paras[size].soeji-1].ng, TRUE ); 
              }
	      
	}    //while�I���
	
}  //narabikae()�I���	


void shuffle(){   //�V���b�t���֐��錾
     
     if(size<=2){       //size==0�`2�܂ł̏���
        for(i=0;i<paras[size].soeji;i++){nxy[i]=nekocopy[i];} //�����摜�f�[�^����R�s�[
	neko[paras[size].soeji-1].nx=520;
        spacex=paras[size].lastposx; //�X�y�[�X���W������
	spacey=paras[size].lastposy;  
       for(j=0;j<paras[size].rand_i;j++){  
          k=rand()%4;  //�����_����4��������
	  
	 for(i=0;i<paras[size].rand_s;i++){        
	   if(k==0 && nxy[i].nx==spacex-paras[size].size_a && nxy[i].ny==spacey) //�X�y�[�X���W�אڃu���b�N�T��
	   {
	      tempx=nxy[i].nx;nxy[i].nx=spacex;spacex=tempx;                     //�X�y�[�X���W,�אڃu���b�N���W����ւ�
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
	 }//for�I��� 
       } //for�I���
     
       do{    //for���I������spacex,spacey�̍��W��lastposx,lastposy�܂Ńu���b�N�������Ȃ���ړ�������
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
        for(i=0;i<=paras[size].rand_i;i++){         // nxy[]����nx,ny�������_���Ńf�[�^����ւ�,size==3�`5�̏���
	                           //size==5�̏ꍇ�̏���(rand�֐�max�l������Ȃ���)
	    if(size==5){j=(rand()%10000)*15+rand()%10014;k=(rand()%10000)*15+rand()%10014;
	    }else{                 //size==0�`4�̏ꍇ�̏���
	      j=rand()%paras[size].rand_s;   //�������Y����j
	      k=rand()%paras[size].rand_s;   //������Y����k
	     }  
	     tempx=nxy[j].nx;              //����
	     tempy=nxy[j].ny;
	     nxy[j].nx=nxy[k].nx;
	     nxy[j].ny=nxy[k].ny;
	     nxy[k].nx=tempx;
	     nxy[k].ny=tempy;
	} //for�I���  		
     }
}   //�V���b�t���֐��I��� 

int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){    //���C���֐�
        ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen( DX_SCREEN_BACK ); //�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�
        
	srand((unsigned)time(NULL));
	Color = GetColor( 255 , 255 , 255 ) ;   //���̐F
start:         //goto���x��
	  ClearDrawScreen(); //��ʏ���
	  DrawFormatString( 0,0, Color, "0�`5�L�[�������ău���b�N���̌���" ) ;
	  DrawFormatString( 0,16, Color, "[0].16��  [1].64��  [2].256��  [3].1600��  [4].10000�� " ) ;
	  DrawFormatString( 0,36, Color, "[5].160000��(����������2����������܂��E�E�E) " ) ;
	  ScreenFlip();  //�\��ʂɕ`��
	
	while( ProcessMessage()==0 && frg==0 ){   //������
	       if( CheckHitKey(KEY_INPUT_NUMPAD0)==1){frg=1;size=0;}      
	       if( CheckHitKey(KEY_INPUT_NUMPAD1)==1){frg=1;size=1;}      
	       if( CheckHitKey(KEY_INPUT_NUMPAD2)==1){frg=1;size=2;}         
	       if( CheckHitKey(KEY_INPUT_NUMPAD3)==1){frg=1;size=3;}      
	       if( CheckHitKey(KEY_INPUT_NUMPAD4)==1){frg=1;size=4;}
	       if( CheckHitKey(KEY_INPUT_NUMPAD5)==1){frg=1;size=5;}  
	}      //while�I���
	
	    
	   if(size==5){ InitGraph();           //size==5�̏ꍇ�̏���
	       g1= LoadGraph( "nekokin.jpg" ); // 400�~400�̉摜�̂ݑΉ�
	       
	       k=0;
	       for(j=50;j<450;j++){
	          
		  for(i=120;i<520;i++){
	            DrawGraph( 120,50,g1, TRUE );    //GetPixel�֐��ŐF����ǂވׂ̕`��
		    nekog[k++]= GetPixel( i , j );   //size==5�̂݃s�N�Z���P�ʂŐF���擾(GetPixel�֐��͎��Ԃ�������)
		  }
	           ClearDrawScreen();
		   DrawFormatString( 0,0, Color, "%d /160000�I���E�E�E",(j-49)*400 ) ;
		   ScreenFlip(); //�\��ʕ`��
	       }   
	   }else{                               //size==0�`4�̏ꍇ�̏���
	         LoadDivGraph( "nekokin.jpg" ,paras[size].soeji,paras[size].tateyoko,paras[size].tateyoko,
	                       paras[size].size_a,paras[size].size_a, nekog );          //size==0�`4,�����ŉ摜�ǂݍ���
	        }
		
	  for(i=0;i<paras[size].soeji;i++){      //�\���̂ɉ摜�f�[�^���ԂɊi�[�@
	   neko[i].ng=nekog[i];
	  }
	  k=0;
	  for(j=50;j<=paras[size].lastposy;j+=paras[size].size_a){    //�\���̂ɍ��W�f�[�^���ԂɊi�[
	    for(i=120;i<=paras[size].lastposx;i+=paras[size].size_a){
	     neko[k].nx=i;
	     neko[k++].ny=j;
	    }
          }
	  for(i=0;i<paras[size].soeji;i++){   //�f�[�^�R�s�[
	    nxy[i]=neko[i];
	    nekocopy[i]=neko[i];
	    
	  }
 
          ClearDrawScreen();  //��ʏ���
	  DrawFormatString( 0,16, Color, "�u���b�N��%d��",paras[size].soeji ) ;
	  DrawFormatString( 0,0, Color, "�E�G���^�[�L�[�������Ă�������" ) ;
	   
	  for(i=0;i<paras[size].soeji;i++){
	    if(size==5){           //size=5�̏ꍇ�̏���
	        DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng ); //�����摜�`��size==5
	     }else{	           //size=0�`4�̏ꍇ�̏���
	       DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE ); //�����摜�`��size==0�`4
              }
	  }
	  ScreenFlip(); //�\��ʕ`��
	
        while(ProcessMessage()==0 && CheckHitKey( KEY_INPUT_NUMPADENTER ) == 0 );//�L�[���͑҂�
	
	shuffle();    //�V���b�t���֐�
        narabikae();  //���ёւ��֐�

	spacex=paras[size].lastposx;       //�X�y�[�Xx���W������
	spacey=paras[size].lastposy;       //�X�y�[�Xy���W������
	frg=0;           //�ړ��L�[���
	k=-1;            //�ړ��Ώۃu���b�N�Y����
	frga=0;          //�A�N�e�B�u���[�h�ؑփL�[��� 
	
	ClearDrawScreen();  //��ʏ���
        for(i=0;i<paras[size].soeji;i++){  //�����摜�`��	
	  if(size==5){         //size==5�̏ꍇ�̏���
	     DrawPixel( neko[i].nx,neko[i].ny, neko[i].ng );
	  }else{	           //size==0�`4�̏ꍇ�̏���                                  
	     DrawGraph( neko[i].nx,neko[i].ny, neko[i].ng, TRUE );
	   }
	     
	}      
	ScreenFlip();  //�\��ʕ`��
	
	while( ProcessMessage()==0  ){
          if(k<0){     //k>=0�̏ꍇ�u���b�N�ړ����̈�,�ړ��L�[���͒��~�@
	    do{   //do-while�J�n,�������ςȂ��ł��u���b�N�ړ�������ׂɏ���͖������Ɏ��s(CheckHitKeyAll() == 0�ł͂�������) 
	       if( CheckHitKey( KEY_INPUT_NUMPADENTER )==1){frga=!frga;} //�A�N�e�B�u���[�h�t���O�؂�ւ�
	       if( CheckHitKey(KEY_INPUT_NUMPAD6)==1){InitGraph();goto start;} //�ǂݍ��ݍς݃O���t�B�b�N�f�[�^�폜,goto
               
	       if(frga==0){           //���[�h�\���ؑ�
	        DrawBox(0,0,200,14,0,TRUE);
	        DrawFormatString( 0, 0, Color, "�A�N�e�B�u���[�hOFF") ;
	          }else {DrawBox(0,0,200,14,0,TRUE);
		         DrawFormatString( 0, 0, Color, "�A�N�e�B�u���[�hON") ;
	                }
	        
	       DrawFormatString( 0, 30, Color, "[1]�L�[���ʂ�@[2]�L�[�ĕ��ёւ��@[6]�u���b�N���ύX�@���L�[�u���b�N�ړ�") ;
               DrawFormatString( 0, 15, Color, "�E�G���^�[�L�[�؂�ւ�") ;
	       ScreenFlip();//�\��ʕ`��
	       
	       if( CheckHitKey( KEY_INPUT_LEFT )==1)frg=1;      //��  �u���b�N�ړ��L�[���o
	       if( CheckHitKey( KEY_INPUT_RIGHT)==1)frg=2;      //�E
	       if( CheckHitKey( KEY_INPUT_UP )==1)frg=3;         //��
	       if( CheckHitKey( KEY_INPUT_DOWN )==1)frg=4;      //��
	      
	      
	      
	      
	       if( CheckHitKey( KEY_INPUT_NUMPAD1)==1){               //�S�u���b�N�ʒu���ʂ菈����
	           
	           for(i=0;i<paras[size].soeji;i++){nxy[i]=nekocopy[i];} //�����摜�f�[�^����R�s�[
	           narabikae();                   //���ёւ��֐�
		   spacex=paras[size].lastposx;    //�f�[�^������
		   spacey=paras[size].lastposy;
		   frg=0;
		   k=-1; 
		   frga=0;
	       }   //if�I���
	       
	       if( CheckHitKey( KEY_INPUT_NUMPAD2)==1){        //�X�ɕ��ёւ�
	         
	           shuffle();       //�V���b�t���֐�
		   narabikae();       //���ёւ��֐�
		   spacex=paras[size].lastposx;   //�f�[�^������
		   spacey=paras[size].lastposy;
		   frg=0;
		   k=-1; 
		   frga=0;  
	       }   //if�I���
	     }while( CheckHitKeyAll() == 0 && frga==0 ); //do-while�I��� �A�N�e�B�u���[�hON(frga==1)�̏ꍇ�A���ňړ��������s
	    }  //if�I���  
             for(i=0;i<paras[size].soeji;i++){    
	       
	       //���ړ��L�[���ɑS�u���b�N�ƃX�y�[�X�̍��W��r,�ړ��Ώۃu���b�N����(�Y����k)�@
	       if(frg==1 && neko[i].nx==spacex+paras[size].size_a && neko[i].ny==spacey    ){k=i;} 
	       if(frg==2 && neko[i].nx==spacex-paras[size].size_a && neko[i].ny==spacey    ){k=i;}
	       if(frg==3 && neko[i].nx==spacex     && neko[i].ny==spacey+paras[size].size_a){k=i;}
	       if(frg==4 && neko[i].nx==spacex     && neko[i].ny==spacey-paras[size].size_a){k=i;}
	     }  //for�I���
	     
	      //���ړ�����
	     if(k>=0){   //�ړ��Ώۃu���b�N���莞�̂ݎ��s
	      
	       while( neko[k].nx!=spacex || neko[k].ny!=spacey){   //�ړ������W�X�V����,�X�y�[�X���W�ɓ��B������I��)
	        
	            
	            if(frg==1){neko[k].nx-=paras[size].move; //���W�l�X�V
		      if(size!=5){      //size==0�`4�̏���(size==5��1��ňړ������ׁ̈A���X�g�̂ݏ���)
			DrawBox(neko[k].nx+paras[size].size_a,spacey,     //�ړ����̃X�y�[�X�摜����
			        spacex+paras[size].size_a*2,spacey+paras[size].size_a,
				0,TRUE);
		        DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );//�ړ��u���b�N�摜����
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
		  ScreenFlip(); //�\��ʕ`��
		   
		    
	       }//while�I���
	        
		
		//���ړ�������X�y�[�X���W�X�V
		if(frg==1)spacex+=paras[size].size_a;
		if(frg==2)spacex-=paras[size].size_a;
		if(frg==3)spacey+=paras[size].size_a;
		if(frg==4)spacey-=paras[size].size_a;
		if(size!=5){    //�X�y�[�X���W==�摜���W�̎��_��while�I���ׁ̈A==���̉摜����
		   DrawBox(spacex,spacey,spacex+paras[size].size_a,spacey+paras[size].size_a,0,TRUE);
		   DrawGraph( neko[k].nx,neko[k].ny, neko[k].ng, TRUE );
		}else{
		   DrawPixel( spacex,spacey,0 );    //size==5��1��ŏ��������̈ו\��1��̂�
		   DrawPixel( neko[k].nx,neko[k].ny, neko[k].ng );
		 }
		if(frga==0)frg=0; //�ړ�������A�N�e�B�u���[�hoff��frg=0�ɍX�V�����A���ړ������Ȃ�
	        k=-1; //�ړ�������k��������
	     } //if�I��� 
	      
	       
	     
	    
	 
	 
	 
	 }  //while�I���
        
        DxLib_End(); // DX���C�u�����I������
        return 0;
}     // winmain�I���