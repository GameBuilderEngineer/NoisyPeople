//======================================
//�d�̓Q�[�W(��)�N���X[ElectGuageEmp.h]
//����W��
//======================================
#pragma once

//======================================
//�C���N���[�h
//======================================
#include "BasicUI.h"
#include "VirtualController.h"

//======================================
//���O���
//======================================
namespace electGuageEmpNS
{
	const int WIDTH_HP_GUAGE_EMP = (512 * WINDOW_WIDTH / 1920);	//�Ή����̉���
	const int HEIGHT_HP_GUAGE_EMP = (32 * WINDOW_HEIGHT / 1080);//�Ή����̏c��
	const D3DXVECTOR3 POSITION_HP_GUAGE_EMP = D3DXVECTOR3(348 * WINDOW_WIDTH / 1920, 90 * WINDOW_HEIGHT / 1080, 0);	//�Ή����̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_HP_GUAGE_EMP_P2 = D3DXVECTOR3(1550 * WINDOW_WIDTH / 1920, 90 * WINDOW_HEIGHT / 1080, 0);	//�Ή����̕\���ʒu(��)
}

//======================================
//�N���X��`
//======================================
class ElectGuageEmp :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * electGuageEmp;
private:
public://�����o�[�֐�
	ElectGuageEmp();
	~ElectGuageEmp();
	void initialize(int playerType);
	void render();
	void update();
};

