//*****************************************************************************************//
//**                                                                                     **//
//**                   �@�@�@�@�@�@  Sound_�N���X                                        **//
//**                                                                                     **//
//*****************************************************************************************//

#ifndef Class_Sound__Header
#define Class_Sound__Header

class Sound_{

private:
	IGraphBuilder *pGraphBuilder; //�C���^�[�t�F�[�X,�O���t�ւ̃t�B���^�̒ǉ��A2 �̃s���̐ڑ���
	IMediaControl *pMediaControl;//�C���^�[�t�F�[�X,�t�B���^ �O���t��ʂ�f�[�^ �t���[�𐧌�
	IVideoWindow *pVideoWindow; //�C���^�[�t�F�[�X,�r�f�I �E�B���h�E�̃v���p�e�B��ݒ�
	IMediaPosition *pMediaPosition; //�C���^�[�t�F�[�X,�X�g���[�����̈ʒu���V�[�N
	IBasicAudio *pBasicAudio;//�C���^�[�t�F�[�X,�I�[�f�B�I �X�g���[���̃{�����[���ƃo�����X�𐧌�

public:
	Sound_();//�R���X�g���N�^
	void sound();
	~Sound_();//�f�X�g���N�^

};

#endif