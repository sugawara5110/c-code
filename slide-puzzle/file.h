//*****************************************************************************************//
//**                                                                                     **//
//**                   　　　　　　Fileクラス定義                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_File_Header
#define Class_File_Header
// #ifndef から #endif までの間に定義を記載します。

class Move;//前方宣言 

class File{

private:
	HANDLE hFile;       //デレクトリ関係
	WIN32_FIND_DATA fd;

	char f_name_tbl[256][100];//画像ファイル名文字列格納
	int i, i1, k;            //i:file現要素 i1:e_file現要素 k:要素数  
	
public:
	File();                                //コンストラクタ
	char *file(Dx9Init *dx, MSG *msg);    //基本画像選択用
	char *e_file(ImageDraw *draw, int f);//画像エンボス用

};

#endif