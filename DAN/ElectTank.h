//=========================================
//�d�̓^���N�N���X[ElectTank.h]
//����W��
//=========================================
#pragma once

//=========================================
//�C���N���[�h
//=========================================
#include "BasicUI.h"

//=========================================
//���O���
//=========================================
namespace electTankNS
{
	const int TANK_MAX = 2;
}

//=========================================
//�N���X��`
//=========================================
class ElectTank : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * electTank[electTankNS::TANK_MAX];
private:
public://�����o�[�֐�
	ElectTank();
	~ElectTank();
	void initialize();
	void render();
	void update();
private:
};