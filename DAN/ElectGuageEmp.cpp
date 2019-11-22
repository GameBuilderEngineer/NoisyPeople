//====================================
//�d�̓Q�[�W(��)����[ElectGuage.cpp]
//����W��
//====================================

//====================================
//�C���N���[�h
//====================================
#include "ElectGuageeEmp.h"

//====================================
// using�錾
//====================================
using namespace electGuageEmpNS;

//====================================
//�R���X�g���N�^
//====================================
ElectGuageEmp::ElectGuageEmp()
{
	electGuageEmp = new Sprite;
}

//====================================
//�f�X�g���N�^
//====================================
ElectGuageEmp::~ElectGuageEmp()
{
	delete electGuageEmp;
}

//====================================
//������
//====================================
void ElectGuageEmp::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		BasicUI::assingPosition(POSITION_HP_GUAGE_EMP);
		BasicUI::assingSize(WIDTH_HP_GUAGE_EMP, HEIGHT_HP_GUAGE_EMP);
		BasicUI::initialize(electGuageEmp, textureNS::reference(textureNS::UI_EN_GUAGE_EMP_P1));
		break;

	case basicUiNS::P2:
		BasicUI::assingPosition(POSITION_HP_GUAGE_EMP_P2);
		BasicUI::assingSize(WIDTH_HP_GUAGE_EMP, HEIGHT_HP_GUAGE_EMP);
		BasicUI::initialize(electGuageEmp, textureNS::reference(textureNS::UI_EN_GUAGE_EMP_P2));
		break;

	}
}

//====================================
//�`��
//====================================
void ElectGuageEmp::render()
{
	electGuageEmp->render();
}

//====================================
//�X�V
//====================================
void ElectGuageEmp::update()
{

}