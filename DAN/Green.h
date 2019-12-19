//=========================================
//�Ή����N���X[Green.h]
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
namespace greenNS
{
	const int WIDTH_GREEN = (512 * WINDOW_WIDTH / 1920);	//�d�̓^���N�̉���
	const int HEIGHT_GREEN = (64 * WINDOW_HEIGHT / 1080);//�d�̓^���N�̏c��
	const D3DXVECTOR3 GREEN_POSTION = D3DXVECTOR3(220.0f * WINDOW_WIDTH / 1920, 190.0f * WINDOW_HEIGHT / 1080, 0.0f);//�^���N�̃|�W�V����
	const D3DXVECTOR3 GREEN_POSTION_P2 = D3DXVECTOR3(1690.0f * WINDOW_WIDTH / 1920, 190.0f * WINDOW_HEIGHT / 1080, 0.0f);//�^���N�̃|�W�V����
	const float ALPHA_MAX = 255.0f;
	const float ALPHA_MIN = 0.0f;
}

//=========================================
//�N���X��`
//=========================================
class Green : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * green;
	
private:
public://�����o�[�֐�
	Green();
	~Green();
	void initialize(int playerType);
	void render(int state);
	
private:
};