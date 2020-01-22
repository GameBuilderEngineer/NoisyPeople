//==========================================
//�d�̓^���N����[electTank.cpp]
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Green.h"
#include "Player.h"

//==========================================
// using�錾
//==========================================
using namespace greenNS;
using namespace playerNS;

//==========================================
//�R���X�g���N�^
//==========================================
Green::Green()
{
		green = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Green::~Green()
{
		delete green;
}

//==========================================
//������
//==========================================
void Green::initialize(int playerType)
{
	BasicUI::assingSize(WIDTH_GREEN, HEIGHT_GREEN);
	switch (playerType)
	{
	case basicUiNS::P1:
			BasicUI::assingPosition(GREEN_POSTION);
			BasicUI::initialize(green, textureNS::reference(textureNS::UI_EN_TANK));
		break;
	case basicUiNS::P2:
		BasicUI::assingPosition(GREEN_POSTION_P2);
		BasicUI::initialize(green, textureNS::reference(textureNS::UI_EN_TANK));
		break;
	}
}

//==========================================
//�`�揈��
//==========================================
void Green::render(int state, float time)
{
	float gametime = time;
	if (gametime > 60)//�c�莞�Ԃ�60�ȏ�̎��̂ݕ\��
	{
		switch (state)
		{
		case STATE::VISION:
			green->render();
			break;
		case STATE::SKY_VISION:
			green->render();
			break;
		}
	}
	
}
