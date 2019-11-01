//-----------------------------------------------------------------------------
// �e���b�v�N���X [Telop.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/17
//-----------------------------------------------------------------------------
#include "Telop.h"
#include "UtilityFunction.h"
using namespace telopNS;


//=============================================================================
// ������
//=============================================================================
void Telop::initialize()
{
	int sinValue = 0;
	input = getInput();
	telopFlag = false;
	state = CLOSE;
	telopTimer = 0.0f;
	displayTimer = 0.0f;
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Sprite;
	}
	
	//�e���b�v0�̏�����
	telop[TELOP_TYPE0]->initialize(
		*textureNS::reference(textureNS::UI_INFO_10),
		SpriteNS::CENTER,
		WIDTH,
		MIN_HEIGHT,
		POSITION,
		ROTATION,
		COLOR
	);

	//�e���b�v1�̏�����
	telop[TELOP_TYPE1]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO),
		SpriteNS::TOP_LEFT,
		WIDTH,
		MIN_HEIGHT,
		POSITION,
		ROTATION,
		COLOR
	);

	//�e���b�v2�̏�����
	/*telop[TELOP_TYPE2]->initialize

	//�e���b�v3�̏�����
	telop[TELOP_TYPE3]->initialize
	
	//�e���b�v4�̏�����
	telop[TELOP_TYPE4]->initialize
	
	//�e���b�v5�̏�����
	telop[TELOP_TYPE5]->initialize*/

	telop[TELOP_INFO_BAR]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BAR),
		SpriteNS::CENTER,
		WIDTH_BAR,
		HEIGHT_BAR,
		POSITION,
		ROTATION,
		COLOR
	);

}


//=============================================================================
// �I������
//=============================================================================
void Telop::uninitialize()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		SAFE_DELETE(telop[i]);
	}
}


//=============================================================================
// �X�V����
//=============================================================================
void Telop::update(float _frameTime)
{
	//frameTime = _frameTime;							//�t���[���^�C���̕ۑ�
	//float rate = sceneTimer / SCENE_TIME;
	
	float rate = 0.0f;
	switch (state)
	{
	case OPEN://�I�[�v��
		telopTimer += _frameTime;						//�V�[���^�C���̍X�V
		//�����̍X�V
		 rate = telopTimer / OPEN_TIME;
		heightValue = (int)((float)MAX_HEIGHT * rate);
		if (telopTimer > OPEN_TIME)
		{
			state = DISPLAY;
			displayTimer = DISPLAY_TIME;
			telopTimer = CLOSE_TIME;
		}
		break;
	case DISPLAY:	
		//�\���ҋ@
		displayTimer -= _frameTime;
		if (displayTimer < 0)
		{
			state = CLOSE;
			displayTimer = 0.0f;
		}
		break;
	case CLOSE:		
		//�N���[�Y
		if (telopTimer > 0)
		{
			telopTimer -= _frameTime;
			//�����̍X�V
			rate = telopTimer / CLOSE_TIME;
			heightValue = (int)((float)MAX_HEIGHT * rate);
		}
		//�I�[�v���֑J��
		if (input->wasKeyPressed('L'))
		{
			state = OPEN;
			telopTimer = 0.0f;
		}
		break;
		
	}

	/*if (telopFlag)
	{
		telopFlag = false;
		state = OPEN;
		telopTimer = 0.0f;
	}*/



	//�T�C�Y�̍X�V
	telop[TELOP_TYPE0]->setSize(WIDTH, heightValue);
	telop[TELOP_TYPE0]->setVertex();
}


//=============================================================================
// �`�揈��
//=============================================================================
void Telop::render()
{
	if (telopTimer>0)
	{
		telop[TELOP_INFO_BAR]->render();
		telop[TELOP_TYPE0]->render();
	}
	//telop[TELOP_TYPE1]->render();

}
