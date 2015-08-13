//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@ImageRead�N���X�����\�b�h��`(�`��)                         **//
//**                                    (ImageDraw�̊��N���X)                          **//
//*****************************************************************************************//

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Dx9Init.h"
#include <process.h>
#include "face_detect.h"
#include "ImageRead.h"
#include "ImageDraw.h"
#include "Move.h"

int ImageDraw::draw_img(Dx9Init *dx, int x, int y, int z){//�t�@�C���I��p�摜�`��
	if (read_img() == -1)return -1;                        //�摜�ǂݍ���
	if (FAILED(f_vertex(dx, x, y, z)))return -1;          //�t�@�C���I��p�摜���_����
	return 0;
}

int ImageDraw::draw_img(Dx9Init *dx, Move *move, int x, int y, int z){//�摜�`��
	if (read_img() == -1)return -1;                               //�摜�ǂݍ���
	if (FAILED(processing_img()))return -1;                     //�摜�ϊ�
	if (FAILED(p_vertex(dx, move, x, y, z)))return -1;  //���_����
	return 0;
}
