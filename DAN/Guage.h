//=====================================
//��{�Q�[�W�N���X[BasicGuage.h]
//����W��
//=====================================
#pragma once

//=====================================
//�C���N���[�h
//=====================================
#include "HPguage.h"
#include "HPguageEmp.h"
#include "Player.h"
#include "ElectGuageeEmp.h"

//=====================================
//���O���
//=====================================
namespace guageNS
{

}

//=====================================
//�N���X��`
//=====================================
class Guage:public Player
{
public://�����o�[�ϐ�
	ElectGuageEmp *electGuageEmp;
	
private:
public://�����o�[�֐�
	Guage();
	~Guage();
	void initialize();
	void render();
	void update();

};