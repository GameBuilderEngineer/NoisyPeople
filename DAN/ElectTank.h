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
	const int WIDTH_EN_GUAGE = (24 * WINDOW_WIDTH / 1920);	//�d�̓^���N�̉���
	const int HEIGHT_EN_GUAGE = (24 * WINDOW_HEIGHT / 1080);//�d�̓^���N�̏c��
	const D3DXVECTOR3 TANK_POSTION = D3DXVECTOR3(550.0f * WINDOW_WIDTH / 1920, 90.0f * WINDOW_HEIGHT / 1080, 0.0f);//�^���N�̃|�W�V����
	const D3DXVECTOR3 TANK_POSTION_P2 = D3DXVECTOR3(1340.0f * WINDOW_WIDTH / 1920, 90.0f * WINDOW_HEIGHT / 1080, 0.0f);//�^���N�̃|�W�V����
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MIN = 0.0f;
}

//=========================================
//�N���X��`
//=========================================
class ElectTank : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * electTank[electTankNS::TANK_MAX];
	D3DXVECTOR3 postion;
private:
public://�����o�[�֐�
	ElectTank();
	~ElectTank();
	void initialize(int playerType);
	void render();
	void update(int power);
	void renderRestriction(int power);//�^���N�̕`�搧���֐�
private:
};