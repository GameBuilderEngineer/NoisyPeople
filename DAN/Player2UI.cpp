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
	electGuageEmp = new ElectGuageEmp;
	electGuage = new ElectGuage;
	electTank = new ElectTank;
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
	delete electGuageEmp;
	delete electGuage;
	delete electTank;
}

//==================================
//������
//==================================
void Player2UI::initialize(Player *player)
{
	this->player = player;
	buttonUi->initialize();
	hpGuage->initialize();
	charaIcon->initialize(basicUiNS::P2);
	hpGuageEmp->initialize(basicUiNS::P2);
	electGuageEmp->initialize(basicUiNS::P2);
	electGuage->initialize(basicUiNS::P2);
	electTank->initialize(basicUiNS::P2);
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
	electGuageEmp->render();
	electGuage->render();
	electTank->render();
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
	power = player->getPower();

	//P2�p
	buttonUi->update(skyVisionFlag, 3);
	buttonUi->update(shiftFlag, 2);
	buttonUi->update(shotFlag, 1);
	buttonUi->update(visionFlag, 0);

	//HP�Q�[�W����
	hpGuage->update(hp);

	//�d�̓Q�[�W����
	electGuage->update(power);
	electTank->update(power);
}
