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
		TUTORIAL_2D_SCENE03_1,
		TUTORIAL_2D_SCENE03_2,
		TUTORIAL_2D_STANDBY1,
		TUTORIAL_2D_STANDBY2,
		TUTORIAL_2D_SCENE_MAX		//2D�̃`���[�g���A���V�[���̑���
	};

	const int WIDTH = WINDOW_WIDTH;								//����
	const int HEIGHT = WINDOW_HEIGHT;							//�c��
	const D3DXVECTOR3 POSITION = D3DXVECTOR3(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0);//�ʒu
	const D3DXVECTOR3 ROTATION = D3DXVECTOR3(0, 0, 0);			//��]
	const D3DCOLOR COLOR = D3DCOLOR_RGBA(255, 255, 255, 255);	//�F

	const D3DXVECTOR3 POSITION_1P = D3DXVECTOR3(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, 0);
	const D3DXVECTOR3 POSITION_2P = D3DXVECTOR3(WINDOW_WIDTH / 4 * 3, WINDOW_HEIGHT / 2, 0);
}
//============================
//�N���X��`
//============================
class TutorialTex
{
private: //�����o�[�ϐ�
	Sprite * tutorial1;
	Sprite * tutorial2;
	Sprite * tutorial3_1;
	Sprite * tutorial3_2;
	Sprite * tutorialStandby1;
	Sprite * tutorialStandby2;
	bool renderFlag[4];
public://�����o�[�ϐ�
	int nextPage;			//�`���[�g���A���̃y�[�W���Ǘ�

public: //�����o�[�֐�
	void initialize();		//����
	void uninitialize();	//�I��
	void render();			//�`��
	void update();			//�X�V
	Sprite* getHalfSlide(int playerNo, int number);
	void setRender(int index, bool setting);
	bool getRenderFlag(int index) {
		return renderFlag[index];
	}
};
