//===============================
//�v���C���[UI�̏���[PlayerUI.cpp]
//����W��
//===============================

//================================
//�C���N���[�h
//================================
#include "Player2UI.h"

//================================
//�R���X�g���N�^
//================================
Player2UI::Player2UI()
{
	buttonUi = new ButtonUI;
	hpGuage = new HPguageP2;
	charaIcon = new CharaIcon;
	hpGuageEmp = new HPguageEmp;
}

//=================================
//�f�X�g���N�^
//=================================
Player2UI::~Player2UI()
{
	delete buttonUi;
	delete hpGuage;
	delete charaIcon;
	delete hpGuageEmp;
}

//==================================
//������
//==================================
void Player2UI::initialize(Player *player)
{
	this->player = player;
	buttonUi->initialize();
	hpGuage->initialize();
	charaIcon->initialize();
	hpGuageEmp->initialize(basicUiNS::P2);
}

//==================================
//�`��
//==================================
void Player2UI::render()
{
	buttonUi->renderP1();
	hpGuageEmp->render();
	hpGuage->render();
	charaIcon->render();
}

//==================================
//�X�V
//==================================
void Player2UI::update()
{
	//�e�t���O�̊l��
	skyVisionFlag = player->canDoSkyVision();
	shiftFlag = player->canShift();
	shotFlag = player->canShot();
	visionFlag = player->canDoVision();
	hp = player->getHp();

	//P2�p
	buttonUi->update(skyVisionFlag, 3);
	buttonUi->update(shiftFlag, 2);
	buttonUi->update(shotFlag, 1);
	buttonUi->update(visionFlag, 0);

	hpGuage->update(hp);
}
