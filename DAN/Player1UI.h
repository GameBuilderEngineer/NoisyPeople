//==================================
//�v���C���[��UI�N���X[PlayerUI.h]
//����W��
//==================================
#pragma once

//==================================
//�C���N���[�h
//==================================
#include "ButtonUI.h"
#include "Player.h"
#include "HPguage.h"
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
	enum STATE {
		NONE,
		NORMAL,
		VISION,
		SKY_VISION,
		DIGITAL_SHIFT,
		DEATH,
		STATE_NUM
	};
}

//==================================
//�N���X��`
//==================================
class Player1UI:public GameMaster
{
public://�����o�[�ϐ�
	ButtonUI * buttonUiP1;
	ButtonUI * buttonUiP2;
	HPguage  * hpGuage;
	HPguageEmp *hpGuageEmp;
	CharaIcon * charaIcon;
	ElectGuageEmp* electGuageEmp;
	ElectGuage* electGuage;
	Green* green;
	Mark* mark;
	GreenRate* greenRate;
	Player *player;			//P1�̏��
	bool	skyVisionFlag;	//�X�J�C�r�W�����t���O
	bool	shotFlag;		//�V���b�g�t���O
	bool	shiftFlag;		//�V�t�g�t���O
	bool	visionFlag;		//�r�B�W�����t���O
	bool	jumpFlag;		//�W�����v�t���O
	int		hp;				//hp�ۑ�
	int		power;			//�d�͕ۑ�
	int     state;			//�v���C���[�̏��
private:
public://�����o�[�֐�
	Player1UI();
	~Player1UI();
	void initialize(Player *player);
	void render();
	void update(float neRate);
	void getFlag();

private:

};