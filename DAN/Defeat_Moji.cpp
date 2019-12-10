//==========================================
//���ސ���������
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Defeat_Moji.h"

//==========================================
// using�錾
//==========================================
using namespace defeat_MojiNS;
using namespace basicUiNS;

//==========================================
//�R���X�g���N�^
//==========================================
Defeat_Moji::Defeat_Moji()
{
	defeat = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Defeat_Moji::~Defeat_Moji()
{
	delete defeat;
}

//==========================================
//������
//==========================================
void Defeat_Moji::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_GEKITAI, HEIGHT_GEKITAI);
		BasicUI::assingPosition(POSITION_GEKITAI_01);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_GEKITAI));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_GEKITAI, HEIGHT_GEKITAI);
		BasicUI::assingPosition(POSITION_GEKITAI_02);
		BasicUI::initialize(defeat, textureNS::reference(textureNS::RESULT_GEKITAI));
		break;
	}

}

//==========================================
//�`��
//==========================================
void Defeat_Moji::render()
{
	defeat->render();
}

//==========================================
//�X�V
//==========================================
void Defeat_Moji::update(int playerType)
{
	if (BasicUI::position.y > END_POS_GEKITAI)
	{
		BasicUI::changePostion(defeat, slideUp);
	}
}