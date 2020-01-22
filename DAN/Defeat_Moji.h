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
namespace defeat_MojiNS
{
	//����
	const int WIDTH_GEKITAI = (255 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_GEKITAI = (86 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_GEKITAI_01 = D3DXVECTOR3(200 * WINDOW_WIDTH / 1920, 6500, 0);//�v���C���[�P�p�����ʒu
	const D3DXVECTOR3 POSITION_GEKITAI_02 = D3DXVECTOR3(1150 * WINDOW_WIDTH / 1920, 6500, 0);//�v���C���[�Q�p�����ʒu
	const float END_POS_GEKITAI = (595.0f* WINDOW_HEIGHT / 1080);	//�ŏI�ʒu��y���W

}

//=========================================
//�N���X��`
//=========================================
class Defeat_Moji : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * defeat;
private:
public:
	Defeat_Moji();		//�R���X�g���N�^
	~Defeat_Moji();	//�f�X�g���N�^
	void initialize(int playerType);
	void render();
	void update(int playerType, float flametime);
private:
};

