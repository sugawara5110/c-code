//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@File�N���X��`                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_File_Header
#define Class_File_Header
// #ifndef ���� #endif �܂ł̊Ԃɒ�`���L�ڂ��܂��B

class Move;//�O���錾 

class File{

private:
	HANDLE hFile;       //�f���N�g���֌W
	WIN32_FIND_DATA fd;

	char f_name_tbl[256][100];//�摜�t�@�C����������i�[
	int i, i1, k;            //i:file���v�f i1:e_file���v�f k:�v�f��  
	
public:
	File();                                //�R���X�g���N�^
	char *file(Dx9Init *dx, MSG *msg);    //��{�摜�I��p
	char *e_file(ImageDraw *draw, int f);//�摜�G���{�X�p

};

#endif