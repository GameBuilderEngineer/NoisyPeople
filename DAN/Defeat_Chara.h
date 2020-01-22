//=========================================
// ���ސ������N���X
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
namespace defeat_CharaNS
{
	//����(�p��)
	const int WIDTH_DEFEAT = (390 * WINDOW_WIDTH / 1920);			//��
	const int HEIGHT_DEFEAT = (53 * WINDOW_HEIGHT / 1080);			//����
	const D3DXVECTOR3 POSITION_DEFEAT_01 = D3DXVECTOR3(273 * WINDOW_WIDTH / 1920, 5500, 0);//�v���C���[�P�p�����ʒu
	const D3DXVECTOR3 POSITION_DEFEAT_02 = D3DXVECTOR3(1218 * WINDOW_WIDTH / 1920, 5500, 0);//�v���C���[�Q�p�����ʒu
	const float END_POS_DEFEAT = (523.0f * WINDOW_HEIGHT / 1080);	//�ŏI�ʒu��y���W

}

//=========================================
//�N���X��`
//=========================================
class Defeat_Chara : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * defeat;
private:
public:
	Defeat_Chara();		//�R���X�g���N�^
	~Defeat_Chara();	//�f�X�g���N�^
	void initialize(int playerType);
	void render();
	void update(int playerType,float flametime);
private:
};
