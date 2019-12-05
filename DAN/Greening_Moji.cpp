//==========================================
//�Ή��{����������
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Greening_Moji.h"

//==========================================
// using�錾
//==========================================
using namespace greenig_MojiNS;
using namespace basicUiNS;

//==========================================
//�R���X�g���N�^
//==========================================
Greening_Moji::Greening_Moji()
{
	greening = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Greening_Moji::~Greening_Moji()
{
	delete greening;
}

//==========================================
//������
//==========================================
void Greening_Moji::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_RYOKUKA, HEIGHT_RYOKUKA);
		BasicUI::assingPosition(POSITION_RYOKUKA_01);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_RYOKUKA_KAZU));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_RYOKUKA, HEIGHT_RYOKUKA);
		BasicUI::assingPosition(POSITION_RYOKUKA_02);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_RYOKUKA_KAZU));
		break;
	}

}

//==========================================
//�`��
//==========================================
void Greening_Moji::render()
{
	greening->render();
}

//==========================================
//�X�V
//==========================================
void Greening_Moji::update(int playerType)
{
	if (BasicUI::position.y > END_POS_RYOKUKA)
	{
		BasicUI::changePostion(greening, slideUp);
	}
}