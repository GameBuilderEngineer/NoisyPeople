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
	hpGuage = new HPguage;
	charaIcon = new CharaIcon;
}

//=================================
//�f�X�g���N�^
//=================================
PlayerUI::~PlayerUI()
{
	delete buttonUiP1;
	delete buttonUiP2;
	delete hpGuage;
	delete charaIcon;
}

//==================================
//������
//==================================
void PlayerUI::initialize(Player *player)
{
	this->player = player;
	buttonUiP1->initialize();
	buttonUiP2->initialize();
	hpGuage->initialize();
	charaIcon->initialize();
}

//==================================
//�`��
//==================================
void PlayerUI::render()
{
	buttonUiP1->renderP1();
	buttonUiP2->renderP2();
	hpGuage->render();
	charaIcon->render();
}

//==================================
//�X�V
//==================================
void PlayerUI::update()
{
	//�e�t���O�̊l��
	skyVisionFlag = player->canDoSkyVision();
	shotFlag = player->canShot();
	jumpFlag = player->canJump();
	visionFlag = player->canDoVision();
	hp = player->getHp();

	//P1�p
	buttonUiP1->update(skyVisionFlag, 3);
	buttonUiP1->update(shotFlag, 2);
	buttonUiP1->update(jumpFlag, 1);
	buttonUiP1->update(visionFlag, 0);

	hpGuage->update(hp);
}
