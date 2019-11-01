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
		minute[i] = new Sprite;
		second[i] = new Sprite;
	}
}

//====================================
//�f�X�g���N�^
//====================================
Timer::~Timer()
{
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		delete minute[i];
		delete second[i];
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
	keepTime = 300;

	//���^�C�}�[�̏�����
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		minute[i]->initialize(
			*textureNS::reference(textureNS::NUMBER)//�e�N�X�`��
			, SpriteNS::CENTER						//���S
			, WIDTH_TIMER							//����
			, HEIGHT_TIMER							//�c��
			, POSITION_MINUTE						//�\���ʒu
			, ROTATION_TIMER						//��]
			, COLOR_TIMER);							//�F
		posMinute[i] = POSITION_MINUTE;				//�\���ʒu�̑��
	}
	
	//�b�^�C�}�[�̏�����
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		second[i]->initialize(
			*textureNS::reference(textureNS::NUMBER)//�e�N�X�`��
			, SpriteNS::CENTER						//���S
			, WIDTH_TIMER							//����
			, HEIGHT_TIMER							//�c��
			, POSITION_SECOND						//�\���ʒu
			, ROTATION_TIMER						//��]
			, COLOR_TIMER);							//�F
		posSecond[i] = POSITION_SECOND;				//�\���ʒu�̑��
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
		//���̕`��
		posMinute[i].x = -WIDTH_TIMER * i + POSITION_MINUTE.x;
		minute[i]->setPosition(posMinute[i]);
		minute[i]->setVertex();
		minute[i]->render();
		//�b�̕`��
		posSecond[i].x = -WIDTH_TIMER * i + POSITION_SECOND.x;
		second[i]->setPosition(posSecond[i]);
		second[i]->setVertex();
		second[i]->render();
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
		float x = (float)(minuteTime % 10);
		uvCoord01.x = 0.1*x;
		uvCoord02.x = 0.1 *(x + 1);
		uvCoord03.x = 0.1 *x;
		uvCoord04.x = 0.1 *(x + 1);
		minuteTime /= 10;
		minute[i]->setUVCoord(uvCoord01, uvCoord02, uvCoord03, uvCoord04);
	}

	//�b�̐ݒ�
	int secondTime = time % 60;
	for (int i = 0; i < DIGIT_TIMER; i++)
	{
		float x = (float)(secondTime % 10);
		uvCoord01.x = 0.1*x;
		uvCoord02.x = 0.1 *(x + 1);
		uvCoord03.x = 0.1 *x;
		uvCoord04.x = 0.1 *(x + 1);
		secondTime /= 10;
		second[i]->setUVCoord(uvCoord01, uvCoord02, uvCoord03, uvCoord04);
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

