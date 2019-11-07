//====================================
//�^�C�}�[����[Timer.cpp]
//����W��
//====================================

//====================================
//�C���N���[�h
//====================================
#include "Timer.h"

//====================================
//	[using�錾]
//====================================
using namespace timerNS;

//====================================
//�R���X�g���N�^
//====================================
Timer::Timer()
{
	//Sprite�̍쐬
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		timer[i] = new Sprite;
	}
}

//====================================
//�f�X�g���N�^
//====================================
Timer::~Timer()
{
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		delete timer[i];
	}
}

//====================================
//������
//====================================
void Timer::initialize()
{
	//�\���ʒu�̑��
	position = POSITION_TIMER;

	//�^�C�}�[�̏����l
	keepTime = GAME_TIME;
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		timer[i]->initialize(
			*textureNS::reference(textureNS::NUMBER)//�e�N�X�`��
			, SpriteNS::CENTER						//���S
			, WIDTH_TIMER							//����
			, HEIGHT_TIMER							//�c��
			, POSITION_TIMER						//�\���ʒu
			, ROTATION_TIMER						//��]
			, COLOR_TIMER);							//�F

	}
}

//====================================
//�`�揈��
//====================================
void Timer::render()
{

	setUV((int)keepTime);

	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		position.x = -WIDTH_TIMER * i + POSITION_TIMER.x;
		timer[i]->setPosition(position);
		timer[i]->setVertex();
		timer[i]->render();
	}
}

//====================================
//�X�V����
//====================================
void Timer::update()
{
	//60�t���[���łP����
	keepTime -= 1.0f / 60.0f;
}

//====================================
//�I������
//====================================
void Timer::uninitialize()
{
	
}

//====================================
// uv���W�̐ݒ�
//====================================
void Timer::setUV(int time)
{
	//���̐ݒ�
	int minuteTime = time / 60;

	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		float x = (float)(time % 10);
		uvCoord01.x = 0.1*x;
		uvCoord02.x = 0.1 *(x + 1);
		uvCoord03.x = 0.1 *x;
		uvCoord04.x = 0.1 *(x + 1);
		time /= 10;
		timer[i]->setUVCoord(uvCoord01, uvCoord02, uvCoord03, uvCoord04);
	}
}


