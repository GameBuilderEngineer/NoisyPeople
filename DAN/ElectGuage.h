#pragma once
//==================================
//�d�̓Q�[�W�N���X[electGuage.h]
//����W��
//==================================

//========================================================================================================
//�v�f
//�E�^���N������
//�E�Q�[�W���}�b�N�X�ɂȂ�ƃ^���N��1����
//========================================================================================================

//===================================
//�C���N���[�h
//===================================
#include "BasicUI.h"

//===================================
//���O���
//===================================
namespace electGuageNS
{
	const int WIDTH_EN_GUAGE = (392 * WINDOW_WIDTH / 1920);	//�d�̓Q�[�W�̉���
	const int HEIGHT_EN_GUAGE = (30 * WINDOW_HEIGHT / 1080);//�d�̓Q�[�W�̏c��
	const D3DXVECTOR3 POSITION_EN_GUAGE = D3DXVECTOR3(135 * WINDOW_WIDTH / 1920, 75 * WINDOW_HEIGHT / 1080, 0);	//�d�̓Q�[�W�̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_EN_GUAGE_P2 = D3DXVECTOR3(1790 * WINDOW_WIDTH / 1920, 75 * WINDOW_HEIGHT / 1080, 0);	//�d�̓Q�[�W�̕\���ʒu(��)
}

//===================================
//�N���X��`
//===================================
class ElectGuage :public BasicUI
{
public://�����o�[�ϐ�
	Sprite * electGuage;
	int width;
private:
public://�����o�[�֐�
	ElectGuage();
	~ElectGuage();
	void initialize(int playerType);
	void render();
	void update(int power);
	void changeGuage(int power);//�Q�[�W�̕ω�����
private:

};
	