//====================================
//�Ή�������[GreenRate.cpp]
//����W��
//====================================

//====================================
//�C���N���[�h
//====================================
#include "GreenRate.h"


//====================================
//	[using�錾]
//====================================
using namespace greenRateNS;

//====================================
//�R���X�g���N�^
//====================================
GreenRate::GreenRate()
{
	//Sprite�̍쐬
	for (int i = 0; i < DIGIT_GREEN_RATE; i++)
	{
		timer[i] = new Sprite;
	}
}

//====================================
//�f�X�g���N�^
//====================================
GreenRate::~GreenRate()
{
	for (int i = 0; i < DIGIT_GREEN_RATE; i++)
	{
		delete timer[i];
	}
}

//====================================
//������
//====================================
void GreenRate::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		//�����\���ʒu�̑��
		BasicUI::assingPosition(POSITION_GREEN_RATE);

		//�����T�C�Y�̑��
		BasicUI::assingSize(WIDTH_GREEN_RATE, HEIGHT_GREEN_RATE);

		keepRate = 0;

		//�e�N�X�`���̏�����
		for (int i = 0; i < DIGIT_GREEN_RATE; i++)
		{
			BasicUI::initialize(timer[i], textureNS::reference(textureNS::UI_TIMER_NUMBER));
		}
	break;
	case basicUiNS::P2:
		//�����\���ʒu�̑��
		BasicUI::assingPosition(POSITION_GREEN_RATE_P2);

		//�����T�C�Y�̑��
		BasicUI::assingSize(WIDTH_GREEN_RATE, HEIGHT_GREEN_RATE);

		keepRate = 0;

		//�e�N�X�`���̏�����
		for (int i = 0; i < DIGIT_GREEN_RATE; i++)
		{
			BasicUI::initialize(timer[i], textureNS::reference(textureNS::UI_TIMER_NUMBER));
		}
	
	}
}

//====================================
//�`�揈��
//====================================
void GreenRate::render(int playerType,int state,float time)
{

	float gametime = time;
	setUV((int)keepRate);
	//�r�W�����A�X�J�C�r�W�������̂ݕ`��
	if (gametime > 60)//�c�莞�Ԃ�60�ȏ�̎��̂ݕ\��
	{
		switch (state)
		{
		case 2:
			if (playerType == basicUiNS::P1)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			else if (playerType == basicUiNS::P2)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE_P2.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			break;

		case 3:
			if (playerType == basicUiNS::P1)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			else if (playerType == basicUiNS::P2)
			{
				for (int i = 0; i < DIGIT_GREEN_RATE; i++)
				{
					position.x = -WIDTH_GREEN_RATE * i + POSITION_GREEN_RATE_P2.x;
					timer[i]->setPosition(position);
					timer[i]->setVertex();
					timer[i]->render();
				}
			}
			break;
		}
	}
	
}

//====================================
//�X�V����
//====================================
void GreenRate::update(float gametime)
{
	//�^�C�}�[�̏����l
	keepRate = gametime;
}

//====================================
//�I������
//====================================
void GreenRate::uninitialize()
{

}

//====================================
// uv���W�̐ݒ�
//====================================
void GreenRate::setUV(int time)
{
	for (int i = 0; i < DIGIT_GREEN_RATE; i++)
	{
		float x = (float)(time % 10);
		uvCoord01.x = 0.1f *x;
		uvCoord02.x = 0.1f *(x + 1);
		uvCoord03.x = 0.1f *x;
		uvCoord04.x = 0.1f *(x + 1);
		time /= 10;
		timer[i]->setUVCoord(uvCoord01, uvCoord02, uvCoord03, uvCoord04);
	}
}


