//==========================================
//�d�̓^���N����[electTank.cpp]
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "ElectTank.h"

//==========================================
// using�錾
//==========================================
using namespace electTankNS;

//==========================================
//�R���X�g���N�^
//==========================================
ElectTank::ElectTank()
{
	for (int i = 0; i < TANK_MAX; i++)
	{
		electTank[i] = new Sprite;
	}
}

//==========================================
//�f�X�g���N�^
//==========================================
ElectTank::~ElectTank()
{
	for (int i = 0; i < TANK_MAX; i++)
	{
		delete electTank[i];
	}
}

//==========================================
//������
//==========================================
void ElectTank::initialize(int playerType)
{
	BasicUI::assingSize(WIDTH_EN_GUAGE, HEIGHT_EN_GUAGE);
	switch (playerType)
	{
	case basicUiNS::P1:
		for (int i = 0; i < TANK_MAX; i++)
		{
			postion.x = TANK_POSTION.x+ (35*i);
			postion.y = TANK_POSTION.y;
			postion.z = TANK_POSTION.z;
			BasicUI::assingPosition(postion);
			BasicUI::initialize(electTank[i], textureNS::reference(textureNS::UI_EN_TANK));
		}
		break;
	case basicUiNS::P2:
		for (int i = 0; i < TANK_MAX; i++)
		{
			postion.x = TANK_POSTION_P2.x + (35 * i);
			postion.y = TANK_POSTION_P2.y;
			postion.z = TANK_POSTION_P2.z;
			BasicUI::assingPosition(postion);
			BasicUI::initialize(electTank[i], textureNS::reference(textureNS::UI_EN_TANK));
		}
		break;
	}
}

//==========================================
//�`�揈��
//==========================================
void ElectTank::render()
{
	for (int i = 0; i < TANK_MAX; i++)
	{
		electTank[i]->render();
	}
}

//==========================================
//�X�V����
//==========================================
void ElectTank::update(int power)
{
	renderRestriction(power);
}

//==========================================
//�^���N�̕`�搧��
//�����F�d��
//==========================================
void ElectTank::renderRestriction(int power)
{
	if (power <100)
	{
		BasicUI::increaseAlpha(electTank[0], ALPHA_MIN);
		BasicUI::increaseAlpha(electTank[1], ALPHA_MIN);
	}
	else if (power >= 100 && power < 200)
	{
		BasicUI::increaseAlpha(electTank[0], ALPHA_MAX);
		BasicUI::increaseAlpha(electTank[1], ALPHA_MIN);
	}
	else
	{
		BasicUI::increaseAlpha(electTank[0], ALPHA_MAX);
		BasicUI::increaseAlpha(electTank[1], ALPHA_MAX);
	}
}