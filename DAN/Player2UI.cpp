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
	mark = new Mark;
	green = new Green;
	greenRate = new GreenRate;
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
	delete mark;
	delete green;
	delete greenRate;
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
	mark->initialize(basicUiNS::P2);
	green->initialize(basicUiNS::P2);
	greenRate->initialize(basicUiNS::P2);
}

//==================================
//�`��
//==================================
void Player2UI::render()
{
	state = player->getState();
	buttonUi->renderP2(state);
	hpGuageEmp->render();
	hpGuage->render();
	electGuageEmp->render();
	electGuage->render();
	charaIcon->render();
	mark->render();
	green->render(state);
	greenRate->render(basicUiNS::P2,state);
}

//==================================
//�X�V
//==================================
void Player2UI::update(float neRate)
{

	//�e�t���O�̊l��
	getFlag();

	//P2�p
	buttonUi->update(shotFlag, 0);
	buttonUi->update(jumpFlag, 1);
	buttonUi->update(visionFlag, 2);
	buttonUi->update(skyVisionFlag, 3);
	buttonUi->update(shiftFlag, 4);

	//HP�Q�[�W����
	hpGuage->update(hp);

	//�d�̓Q�[�W����
	electGuage->update(power);

	//�Ή���
	greenRate->update(neRate);
}

//==================================
//�e�t���O�̊l��
//==================================
void Player2UI::getFlag()
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