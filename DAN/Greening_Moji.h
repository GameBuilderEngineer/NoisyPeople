//=========================================
// �Ή��{�������N���X
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
namespace greenig_MojiNS
{
	//�Ή�
	const int WIDTH_RYOKUKA = (512 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_RYOKUKA = (128 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_RYOKUKA_01 = D3DXVECTOR3(340 * WINDOW_WIDTH / 1920, 4500, 0);//�v���C���[�P�p�����ʒu
	const D3DXVECTOR3 POSITION_RYOKUKA_02 = D3DXVECTOR3(1290 * WINDOW_WIDTH / 1920, 4500, 0);//�v���C���[�Q�p�����ʒu
	const float END_POS_RYOKUKA = (425.0f * WINDOW_HEIGHT / 1080);	//�ŏI�ʒu��y���W

}

//=========================================
//�N���X��`
//=========================================
class Greening_Moji : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * greening;
private:
public:
	Greening_Moji();		//�R���X�g���N�^
	~Greening_Moji();	//�f�X�g���N�^
	void initialize(int playerType);
	void render();
	void update(int playerType);
private:
};
