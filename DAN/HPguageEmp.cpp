//====================================
//HP�Q�[�W(��)����[HPguage.cpp]
//����W��
//====================================

//====================================
//�C���N���[�h
//====================================
#include "HPguageEmp.h"

//====================================
// using�錾
//====================================
using namespace hpGuageEmpNS;

//====================================
//�R���X�g���N�^
//====================================
HPguageEmp::HPguageEmp()
{
	hpGuageEmp = new Sprite;
}

//====================================
//�f�X�g���N�^
//====================================
HPguageEmp::~HPguageEmp()
{
	delete hpGuageEmp;
}

//====================================
//������
//====================================
void HPguageEmp::initialize()
{
	BasicUI::assingPosition(POSITION_HP_GUAGE_EMP);
	BasicUI::assingSize(WIDTH_HP_GUAGE_EMP, HEIGHT_HP_GUAGE_EMP);
	BasicUI::initialize(hpGuageEmp, textureNS::reference(textureNS::UI_HP_GUAGE_EMP_P1));
}

//====================================
//�`��
//====================================
void HPguageEmp::render()
{
	hpGuageEmp->render();
}

//====================================
//�X�V
//====================================
void HPguageEmp::update()
{

}