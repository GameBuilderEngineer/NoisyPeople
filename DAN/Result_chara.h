//=========================================
// RESULT�����N���X
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
namespace result_CharaNS
{
	//���U���g���S
	const int WIDTH_RESULT = (330 * WINDOW_WIDTH / 1920);			//��
	const int HEIGHT_RESULT = (170 * WINDOW_HEIGHT / 1080);		//����
	const D3DXVECTOR3 POSITION_RESULT = D3DXVECTOR3(2100, 70 * WINDOW_HEIGHT / 1080, 0);//�����ʒu
	const float	END_POS_RESULT = (215.0f * WINDOW_WIDTH / 1920);	//�ŏI�ʒu��x���W
}

//=========================================
//�N���X��`
//=========================================
class Result_Chara : public BasicUI
{
public://�����o�[�ϐ�
	Sprite * result;
private:
public:
	Result_Chara();		//�R���X�g���N�^
	~Result_Chara();	//�f�X�g���N�^
	void initialize();
	void render();
	void update(float flametime);
private:
};