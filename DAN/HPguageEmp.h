//======================================
//HP�Q�[�W(��)�N���X[HPguageEmp.h]
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
namespace hpGuageEmpNS
{
	const int WIDTH_HP_GUAGE_EMP = (500 * WINDOW_WIDTH / 1920);	//�^�C�}�[�t���[���̉���
	const int HEIGHT_HP_GUAGE_EMP = (30 * WINDOW_HEIGHT / 1080);	//�^�C�}�[�t���[���̏c��
	const D3DXVECTOR3 POSITION_HP_GUAGE_EMP = D3DXVECTOR3(350 * WINDOW_WIDTH / 1920, 50 * WINDOW_HEIGHT / 1080, 0);	//�^�C�}�[�t���[���̕\���ʒu(��)
}

//======================================
//�N���X��`
//======================================
class HPguageEmp :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * hpGuageEmp;
private:
public://�����o�[�֐�
	HPguageEmp();
	~HPguageEmp();
	void initialize();
	void render();
	void update();
};
