//=========================================
// PLAYER�����N���X
//����W��
//=========================================
#pragma once

//=========================================
//�C���N���[�h
//=========================================
#include "BasicUI.h"

//=========================================
//���O���
//=========================================
namespace player_CharaNS
{
	//�v���C���[
	const int WIDTH_PLAYER = (345 * WINDOW_WIDTH / 1920);			//��
	const int HEIGHT_PLAYER = (90 * WINDOW_HEIGHT / 1080);			//����
	const D3DXVECTOR3 POSITION_PLAYER = D3DXVECTOR3(3800, 250 * WINDOW_HEIGHT / 1080, 0);//�����ʒu
	const float END_POS_PLAYER_01 = (345.0f* WINDOW_WIDTH / 1920);//�v���C���[�P�̍ŏI�ʒu��x���W
	const float END_POS_PLAYER_02 = (1605 * WINDOW_WIDTH / 1920);	 //�v���C���[�Q�̍ŏI�ʒu��x���W

	//uv���W
	const D3DXVECTOR2 PLAYER1_UV_VERTEX01 = D3DXVECTOR2(0.0, 0.0);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX02 = D3DXVECTOR2(1.0, 0.0);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX03 = D3DXVECTOR2(0.0, 0.5);
	const D3DXVECTOR2 PLAYER1_UV_VERTEX04 = D3DXVECTOR2(1.0, 0.5);

	const D3DXVECTOR2 PLAYER2_UV_VERTEX01 = D3DXVECTOR2(0.0, 0.5);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX02 = D3DXVECTOR2(1.0, 0.5);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX03 = D3DXVECTOR2(0.0, 1.0);
	const D3DXVECTOR2 PLAYER2_UV_VERTEX04 = D3DXVECTOR2(1.0, 1.0);
}

//=========================================
//�N���X��`
//=========================================
class Player_Chara : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * player;
private:
public:
	Player_Chara();		//�R���X�g���N�^
	~Player_Chara();	//�f�X�g���N�^
	void initialize(int playerType);
	void render();
	void update(int playerType);
private:
};