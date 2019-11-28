//===============================
//�v���C���[UI�̏���[PlayerUI.cpp]
//����W��
//===============================

//================================
//�C���N���[�h
//================================
#include "Player1UI.h"

//================================
//�R���X�g���N�^
//================================
Player1UI::Player1UI()
{
	buttonUiP1 = new ButtonUI;
	buttonUiP2 = new ButtonUI;
	hpGuage = new HPguage;
	charaIcon = new CharaIcon;
	hpGuageEmp = new HPguageEmp;
	electGuageEmp = new ElectGuageEmp;
	electGuage = new ElectGuage;
	electTank = new ElectTank;
}

//=================================
//�f�X�g���N�^
//=================================
Player1UI::~Player1UI()
{
	delete buttonUiP1;
	delete buttonUiP2;
	delete hpGuage;
	delete charaIcon;
	delete hpGuageEmp;
	delete electGuageEmp;
	delete electGuage;
	delete electTank;
}

//==================================
//������
//==================================
void Player1UI::initialize(Player *player)
{
	this->player = player;
	buttonUiP1->initialize();
	buttonUiP2->initialize();
	hpGuage->initialize();
	charaIcon->initialize(basicUiNS::P1);
	hpGuageEmp->initialize(basicUiNS::P1);
	electGuageEmp->initialize(basicUiNS::P1);
	electGuage->initialize(basicUiNS::P1);
	electTank->initialize(basicUiNS::P1);
}

//==================================
//�`��
//==================================
void Player1UI::render()
{
	buttonUiP1->renderP1();
	buttonUiP2->renderP2();
	hpGuageEmp->render();
	hpGuage->render();
	charaIcon->render();
	electGuageEmp->render();
	electGuage->render();
	electTank->render();
}

//==================================
//�X�V
//==================================
void Player1UI::update()
{
	//�e�t���O�̊l��
	getFlag();

	//P1�p
	buttonUiP1->update(shotFlag, 0);
	buttonUiP1->update(jumpFlag, 1);
	buttonUiP1->update(visionFlag, 2);
	buttonUiP1->update(skyVisionFlag, 3);
	buttonUiP1->update(shiftFlag, 4);

	hpGuage->update(hp);
	electGuage->update(power);
	electTank->update(power,basicUiNS::P1);
}

//==================================
//�e�t���O�̊l��
//==================================
void Player1UI::getFlag()
{
	//�e�t���O�̊l��
	skyVisionFlag = player->canDoSkyVision();
	shiftFlag = player->canShift();
	shotFlag = player->canShot();
	visionFlag = player->canDoVision();
	jumpFlag = player->canJump();
	hp = player->getHp();
	power = player->getPower();
}