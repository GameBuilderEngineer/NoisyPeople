//==================================
//�^�C�}�[�t���[������[TimerFlame.cpp]
//����@�W��
//==================================

//==================================
//�C���N���[�h
//==================================
#include "TimerFrame.h"

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
	//�����\���ʒu�̑��
	BasicUI::assingPosition(POSITION_TIMERFLAME);
	//�����T�C�Y�̑��
	BasicUI::assingSize(WIDTH_TIMERFLAME, HEIGHT_TIMERFLAME);
	//�e�N�X�`���̏�����
	BasicUI::initialize(timerFlame, textureNS::reference(textureNS::UI_TIMERFLAME));
}

//=================================
//�`��
//=================================
void TimerFlame::render()
{
	timerFlame->render();
}
