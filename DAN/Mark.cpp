//==================================
//�L�����A�C�R������[Mark.cpp]
//����@�W��
//==================================

//==================================
//�C���N���[�h
//==================================
#include "Mark.h"

//==================================
// using�錾
//==================================
using namespace markNS;

//==================================
//�R���X�g���N�^
//==================================
Mark::Mark()
{
	//�X�v���C�g�̍쐬
	mark = new Sprite;
}

//==================================
//�f�X�g���N�^
//==================================
Mark::~Mark()
{
	delete mark;
}

//==================================
//������
//==================================
void Mark::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		//�����\���ʒu�̑��
		BasicUI::assingPosition(POSITION_MARK);
		//�����T�C�Y�̑��
		BasicUI::assingSize(WIDTH_MARK, HEIGHT_MARK);
		//�e�N�X�`���̏�����
		BasicUI::initialize(mark, textureNS::reference(textureNS::UI_EN_MARK));
		break;

	case basicUiNS::P2:
		//�����\���ʒu�̑��
		BasicUI::assingPosition(POSITION_MARK_P2);
		//�����T�C�Y�̑��
		BasicUI::assingSize(WIDTH_MARK, HEIGHT_MARK);
		//�e�N�X�`���̏�����
		BasicUI::initialize(mark, textureNS::reference(textureNS::UI_EN_MARK));
	}

}

//=================================
//�`��
//=================================
void Mark::render()
{
	mark->render();
}
