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

//==================================
//���O���
//==================================
namespace playerUiNS
{

}

//==================================
//�N���X��`
//==================================
class Player1UI
{
public://�����o�[�ϐ�
	ButtonUI * buttonUiP1;
	ButtonUI * buttonUiP2;
	HPguage  * hpGuage;
	HPguageEmp *hpGuageEmp;
	CharaIcon * charaIcon;
	Player *player;			//P1�̏��
	bool	skyVisionFlag;	//�X�J�C�r�W�����t���O
	bool	shotFlag;		//�V���b�g�t���O
	bool	shiftFlag;		//�V�t�g�t���O
	bool	visionFlag;		//�r�B�W�����t���O
	int		hp;				//hp�ۑ�
private:
public://�����o�[�֐�
	Player1UI();
	~Player1UI();
	void initialize(Player *player);
	void render();
	void update();
	
private:

};