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
	//uv���W�̑��
	assingUV();

	//�^�C�}�[�̏����l
	keepTime = 400;

	//�^�C�}�[�̏�����
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		timer[i]->initialize(
			*textureNS::reference(textureNS::NUMBER)//�e�N�X�`��
			, SpriteNS::CENTER							//���S
			, WIDTH_TIMER								//����
			, HEIGHT_TIMER								//�c��
			, POSITION_TIMER							//�\���ʒu
			, ROTATION_TIMER							//��]
			, COLOR_TIMER);								//�F
		pos[i] = POSITION_TIMER;						//�\���ʒu�̑��
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
		pos[i].x = -WIDTH_TIMER * i + POSITION_TIMER.x;
		timer[i]->setPosition(pos[i]);
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

//===================================
// uv���W�̑��
//===================================
void Timer::assingUV()
{
	uvCoord01 = D3DXVECTOR2(0.0, 0.0);
	uvCoord02 = D3DXVECTOR2(1.0, 0.0);
	uvCoord03 = D3DXVECTOR2(0.0, 1.0);
	uvCoord04 = D3DXVECTOR2(1.0, 1.0);
}

