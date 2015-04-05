//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　Filterクラス定義                                      **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Filter_Header
#define Class_Filter_Header

class Filter{

public:
	int **block;                                 //フィルター格納用

	Filter();                                  //コンストラクタ
	void filter(Move *move, ImageDraw *draw); //ブロックフィルター
	~Filter();                               //デストラクタ 
	
};

#endif