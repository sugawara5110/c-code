//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@Filter�N���X��`                                      **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Filter_Header
#define Class_Filter_Header

class Filter{

public:
	int **block;                                 //�t�B���^�[�i�[�p

	Filter();                                  //�R���X�g���N�^
	void filter(Move *move, ImageDraw *draw); //�u���b�N�t�B���^�[
	~Filter();                               //�f�X�g���N�^ 
	
};

#endif