//============================
// [TutorialTex.h]
// ����@�W��
//============================
#pragma once

//============================
//�C���N���[�h
//============================
#include "Sprite.h"
#include "TextureLoader.h"

//============================
//���O��� �萔
//============================
namespace tutorialTex
{
	enum
	{
		TUTORIAL_2D_SCENE01,
		TUTORIAL_2D_SCENE02,
		TUTORIAL_2D_SCENE_MAX		//2D�̃`���[�g���A���V�[���̑���
	};

	const int WIDTH = WINDOW_WIDTH;								//����
	const int HEIGHT = WINDOW_HEIGHT * 2;						//�c��
	const D3DXVECTOR3 POSITION = D3DXVECTOR3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);//�ʒu
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//��]
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F
}
//============================
//�N���X��`
//============================
class TutorialTex
{
private: //�����o�[�ϐ�
	Sprite * tutorial1;
	Sprite * tutorial2;

public://�����o�[�ϐ�
	int nextPage;			//�`���[�g���A���̃y�[�W���Ǘ�

public: //�����o�[�֐�
	void initialize();		//����
	void uninitialize();	//�I��
	void render();			//�`��
	void update();			//�X�V


};
