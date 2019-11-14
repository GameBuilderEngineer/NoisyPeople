//==================================
//�v���C���[��UI�N���X[PlayerUI.h]
//����W��
//==================================
#pragma once

//==================================
//�C���N���[�h
//==================================
#include "Player.h"
#include "ButtonUI.h"

//==================================
//���O���
//==================================
namespace playerUiNS
{

}

//==================================
//�N���X��`
//==================================
class PlayerUI :public Player
{
public://�����o�[�ϐ�
	ButtonUI * buttonUiP1;
	ButtonUI * buttonUiP2;
	bool	skyVisionFlag;
	bool	shotFlag;
	bool	jumpFlag;
	bool	visionFlag;
private:
public://�����o�[�֐�
	PlayerUI();
	~PlayerUI();
	void initialize();
	void render();
	void update();
	
private:

};