//==================================
//�v���C���[��UI�N���X[Player2UI.h]
//����W��
//==================================
#pragma once

//==================================
//�C���N���[�h
//==================================
#include "ButtonUI.h"
#include "Player.h"
#include "HPguageP2.h"
#include "CharaIcon.h"
#include "HPguageEmp.h"
#include "ElectGuageeEmp.h"
#include "ElectGuage.h"
#include "Green.h"
#include "Mark.h"
#include "GreenRate.h"

//==================================
//���O���
//==================================
namespace playerUiNS
{

}

//==================================
//�N���X��`
//==================================
class Player2UI
{
public://�����o�[�ϐ�
	ButtonUI* buttonUi;
	HPguageP2* hpGuage;
	CharaIcon* charaIcon;
	HPguageEmp*  hpGuageEmp;
	ElectGuageEmp* electGuageEmp;
	ElectGuage*	electGuage;
	Green* green;
	Mark* mark;
	GreenRate* greenRate;
	Player *player;			//P2�̏��
	bool	skyVisionFlag;	//�X�J�C�r�W�����t���O
	bool	shotFlag;		//�V���b�g�t���O
	bool	shiftFlag;		//�V�t�g�t���O
	bool	visionFlag;		//�r�B�W�����t���O
	bool	jumpFlag;		//�W�����v�t���O
	int		hp;				//hp�ۑ�
	int     power;			//�d��
	int		state;			//�v���C���[�̏��

private:
public://�����o�[�֐�
	Player2UI();
	~Player2UI();
	void initialize(Player *player);
	void render(float time);
	void update(float neRate);
	void getFlag();

private:

};