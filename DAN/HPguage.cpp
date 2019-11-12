//====================================
//HP�Q�[�W����[HPguage.cpp]
//����W��
//====================================

//====================================
//�C���N���[�h
//====================================
#include "HPguage.h"

//====================================
// using�錾
//====================================
using namespace hpGuageNS;

//====================================
//�R���X�g���N�^
//====================================
HPguage::HPguage()
{
	hpGuage = new Sprite;
}

//====================================
//�f�X�g���N�^
//====================================
HPguage::~HPguage()
{
	delete hpGuage;
}

//====================================
//������
//====================================
void HPguage::initialize()
{
	BasicUI::assingPosition(POSITION_HP_GUAGE);
	BasicUI::assingSize(WIDTH_HP_GUAGE, HEIGHT_HP_GUAGE);
	BasicUI::initialize(hpGuage, textureNS::reference(textureNS::UI_HP_GUAGE_P1));
}

//====================================
//�`��
//====================================
void HPguage::render()
{
	hpGuage->render();
}

//====================================
//�X�V
//====================================
void HPguage::update()
{

}