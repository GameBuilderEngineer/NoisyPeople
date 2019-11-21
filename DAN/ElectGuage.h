#pragma once
//==================================
//�d�̓Q�[�W�N���X[electGuage.h]
//����W��
//==================================

//========================================================================================================
//�v�f
//�E�^���N������
//�E�Q�[�W���}�b�N�X�ɂȂ�ƃ^���N��1����
//�E���Ԍo�߂ŏ��X�ɃQ�[�W��
//�E�A�C�e�����E���Ɖ�
//�E���B�W�����g�p���̓Q�[�W������
//�E�Ƃ������Ƃ͓d�͂��Q�Ƃ��ăQ�[�W�𓮂����ƕ֗��H
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
	const int WIDTH_EN_GUAGE = (360 * WINDOW_WIDTH / 1920);	//�d�̓Q�[�W�̉���
	const int HEIGHT_EN_GUAGE = (30 * WINDOW_HEIGHT / 1080);//�d�̓Q�[�W�̏c��
	const D3DXVECTOR3 POSITION_EN_GUAGE = D3DXVECTOR3(102 * WINDOW_WIDTH / 1920, 35 * WINDOW_HEIGHT / 1080, 0);	//�d�̓Q�[�W�̕\���ʒu(��)
	const D3DXVECTOR3 POSITION_EN_GUAGE_P2 = D3DXVECTOR3(1872 * WINDOW_WIDTH / 1920, 35 * WINDOW_HEIGHT / 1080, 0);	//�d�̓Q�[�W�̕\���ʒu(��)
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
	void update();
	void changeGuage(int elect);//�Q�[�W�̕ω�����
private:

};
	