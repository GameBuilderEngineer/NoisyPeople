//================================
// �Œ肳�ꂽUI[FixedUI.cpp]
// ����W��
//================================

//================================
//�C���N���[�h
//================================
#include "FixedUI.h"

//================================
// using�錾
//================================
using namespace fixedUiNS;

//================================
// �R���X�g���N�^
//================================
FixedUI::FixedUI()
{
	fixedUI = new Sprite;
}

//================================
// �f�X�g���N�^
//================================
FixedUI::~FixedUI()
{
	delete fixedUI;
}

//================================
// ������
//================================
void FixedUI::initialize()
{
	fixedUI->initialize(
		*textureNS::reference(textureNS::UI_TIMERFLAME)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_TIMERFLAME							//����
		, HEIGHT_TIMERFLAME							//�c��
		, POSITION_TIMERFLAME						//�\���ʒu
		, ROTATION_TIMERFLAME						//��]
		, COLOR_TIMERFLAME);						//�F
}

//================================
// �`��
//================================
void FixedUI::render()
{
	fixedUI->render();
}

//================================
// �X�V
//================================
void FixedUI::update()
{

}

//================================
// �I��
//================================
void FixedUI::uninitialize()
{

}
