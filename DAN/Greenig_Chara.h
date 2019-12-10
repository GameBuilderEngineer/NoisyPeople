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
namespace greenig_CharaNS
{
	//�Ή��{��(�p��)
	const int WIDTH_GREENIG = (512 * WINDOW_WIDTH / 1920);		//��
	const int HEIGHT_GREENIG = (64 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_GREENIG_01 = D3DXVECTOR3(280 * WINDOW_WIDTH / 1920, 3500, 0);//�v���C���[�P�p�����ʒu
	const D3DXVECTOR3 POSITION_GREENIG_02 = D3DXVECTOR3(1235 * WINDOW_WIDTH / 1920, 3500, 0);//�v���C���[�Q�p�����ʒu
	const float END_POS_GREENING = (353.0f* WINDOW_HEIGHT / 1080);//�ŏI�ʒu��y���W

}

//=========================================
//�N���X��`
//=========================================
class Greening_Chara : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * greening;
private:
public:
	Greening_Chara();		//�R���X�g���N�^
	~Greening_Chara();	//�f�X�g���N�^
	void initialize(int playerType);
	void render();
	void update(int playerType);
private:
};
