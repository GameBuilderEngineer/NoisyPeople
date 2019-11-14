//==================================
//�L�����A�C�R������[CharaIcon.cpp]
//����@�W��
//==================================

//==================================
//�C���N���[�h
//==================================
#include "CharaIcon.h"

//==================================
// using�錾
//==================================
using namespace charaIconNS;

//==================================
//�R���X�g���N�^
//==================================
CharaIcon::CharaIcon()
{
	//�X�v���C�g�̍쐬
	charaIcon = new Sprite;
}

//==================================
//�f�X�g���N�^
//==================================
CharaIcon::~CharaIcon()
{
	delete charaIcon;
}

//==================================
//������
//==================================
void CharaIcon::initialize()
{
	//�����\���ʒu�̑��
	BasicUI::assingPosition(POSITION_ICON);
	//�����T�C�Y�̑��
	BasicUI::assingSize(WIDTH_ICON, HEIGHT_ICON);
	//�e�N�X�`���̏�����
	BasicUI::initialize(charaIcon, textureNS::reference(textureNS::UI_ICON_P1));
}

//=================================
//�`��
//=================================
void CharaIcon::render()
{
	charaIcon->render();
}
