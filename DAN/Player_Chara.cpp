//==========================================
//���U���g��������
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Player_chara.h"

//==========================================
// using�錾
//==========================================
using namespace player_CharaNS;
using namespace basicUiNS;

//==========================================
//�R���X�g���N�^
//==========================================
Player_Chara::Player_Chara()
{
	player = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Player_Chara::~Player_Chara()
{
	delete player;
}

//==========================================
//������
//==========================================
void Player_Chara::initialize(int playerType)
{
	switch (playerType)
	{
	case P1:
		BasicUI::assingSize(WIDTH_PLAYER, HEIGHT_PLAYER);
		BasicUI::assingPosition(POSITION_PLAYER);
		BasicUI::initialize(player, textureNS::reference(textureNS::RESULT_PLAYER));
		break;

	case P2:
		BasicUI::assingSize(WIDTH_PLAYER, HEIGHT_PLAYER);
		BasicUI::assingPosition(POSITION_PLAYER);
		BasicUI::initialize(player, textureNS::reference(textureNS::RESULT_PLAYER));
		break;

	default:
		break;
	}

}

//==========================================
//�`��
//==========================================
void Player_Chara::render()
{
	player->render();
}

//==========================================
//�X�V
//==========================================
void Player_Chara::update(int playerType, float flametime)
{
	switch (playerType)
	{
	case P1:
		BasicUI::changeUV(player,PLAYER1_UV_VERTEX01, PLAYER1_UV_VERTEX02, PLAYER1_UV_VERTEX03, PLAYER1_UV_VERTEX04);
		if (BasicUI::position.x > END_POS_PLAYER_01)
		{
			BasicUI::changePostion(player, slideLeft,flametime);
		}
		break;

	case P2:
		BasicUI::changeUV(player,PLAYER2_UV_VERTEX01, PLAYER2_UV_VERTEX02, PLAYER2_UV_VERTEX03, PLAYER2_UV_VERTEX04);
		if (BasicUI::position.x > END_POS_PLAYER_02)
		{
			BasicUI::changePostion(player, slideLeft,flametime);
		}
		break;
	}
	
}