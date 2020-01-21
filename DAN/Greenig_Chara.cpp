//==========================================
//�Ή��{����������
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Greenig_chara.h"

//==========================================
// using�錾
//==========================================
using namespace greenig_CharaNS;
using namespace basicUiNS;

//==========================================
//�R���X�g���N�^
//==========================================
Greening_Chara::Greening_Chara()
{
	greening = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Greening_Chara::~Greening_Chara()
{
	delete greening;
}

//==========================================
//������
//==========================================
void Greening_Chara::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_GREENIG, HEIGHT_GREENIG);
		BasicUI::assingPosition(POSITION_GREENIG_01);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_GREEN_NUM));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_GREENIG, HEIGHT_GREENIG);
		BasicUI::assingPosition(POSITION_GREENIG_02);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_GREEN_NUM));
		break;
	}

}

//==========================================
//�`��
//==========================================
void Greening_Chara::render()
{
	greening->render();
}

//==========================================
//�X�V
//==========================================
void Greening_Chara::update(int playerType, float flametime)
{
	if (BasicUI::position.y > END_POS_GREENING)
	{
		BasicUI::changePostion(greening, slideUp,flametime);
	}
}