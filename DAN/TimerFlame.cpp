//==================================
//�^�C�}�[�t���[������[TimerFlame.cpp]
//����@�W��
//==================================

//==================================
//�C���N���[�h
//==================================
#include "TimerFlame.h"

//==================================
// using�錾
//==================================
using namespace timerFlameNS;

//==================================
//�R���X�g���N�^
//==================================
TimerFlame::TimerFlame()
{
	//�X�v���C�g�̍쐬
	timerFlame = new Sprite;
}

//==================================
//�f�X�g���N�^
//==================================
TimerFlame::~TimerFlame()
{
	delete timerFlame;
}

//==================================
//������
//==================================
void TimerFlame::initialize()
{
	timerFlame->initialize(
		*textureNS::reference(textureNS::UI_TIMERFLAME)//�e�N�X�`��
		, SpriteNS::CENTER							//���S
		, WIDTH_TIMERFLAME							//����
		, HEIGHT_TIMERFLAME							//�c��
		, POSITION_TIMERFLAME						//�\���ʒu
		, ROTATION_TIMERFLAME						//��]
		, COLOR_TIMERFLAME);						//�F
}

//=================================
//�`��
//=================================
void TimerFlame::render()
{
	timerFlame->render();
}