//==========================================
//���ސ���������
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Defeat_chara.h"

//==========================================
// using�錾
//==========================================
using namespace defeat_CharaNS;
using namespace basicUiNS;

//==========================================
//�R���X�g���N�^
//==========================================
Defeat_Chara::Defeat_Chara()
{
	defeat = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Defeat_Chara::~Defeat_Chara()
{
	delete defeat;
}

//==========================================
//������
//==========================================
void Defeat_Chara::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_DEFEAT, HEIGHT_DEFEAT);
		BasicUI::assingPosition(POSITION_DEFEAT_01);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_DEFEAT));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_DEFEAT, HEIGHT_DEFEAT);
		BasicUI::assingPosition(POSITION_DEFEAT_02);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_DEFEAT));
		break;
	}

}

//==========================================
//�`��
//==========================================
void Defeat_Chara::render()
{
	defeat->render();
}

//==========================================
//�X�V
//==========================================
void Defeat_Chara::update(int playerType,float flametime)
{
	if (BasicUI::position.y > END_POS_DEFEAT)
	{
		BasicUI::changePostion(defeat, slideUp,flametime);
	}
}