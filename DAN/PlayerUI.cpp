//===============================
//�v���C���[UI�̏���[PlayerUI.cpp]
//����W��
//===============================

//================================
//�C���N���[�h
//================================
#include "PlayerUI.h"

//================================
//�R���X�g���N�^
//================================
PlayerUI::PlayerUI()
{
	buttonUiP1 = new ButtonUI;
	buttonUiP2 = new ButtonUI;
}

//=================================
//�f�X�g���N�^
//=================================
PlayerUI::~PlayerUI()
{
	delete buttonUiP1;
	delete buttonUiP2;
}

//==================================
//������
//==================================
void PlayerUI::initialize()
{
	buttonUiP1->initialize();
	buttonUiP2->initialize();
}

//==================================
//�`��
//==================================
void PlayerUI::render()
{
	buttonUiP1->renderP1();
	buttonUiP2->renderP2();
}

//==================================
//�X�V
//==================================
void PlayerUI::update()
{
	//�e�t���O�̊l��
	skyVisionFlag = Player::canDoSkyVision();
	shotFlag = Player::canShot();
	jumpFlag = Player::canJump();
	visionFlag = Player::canDoVision();

	//P1�p
	buttonUiP1->update(skyVisionFlag, 3);
	buttonUiP1->update(shotFlag, 2);
	buttonUiP1->update(jumpFlag, 1);
	buttonUiP1->update(visionFlag, 0);

	//P2�p
	buttonUiP2->update(skyVisionFlag, 3);
	buttonUiP2->update(shotFlag, 2);
	buttonUiP2->update(jumpFlag, 1);
	buttonUiP2->update(visionFlag, 0);
}