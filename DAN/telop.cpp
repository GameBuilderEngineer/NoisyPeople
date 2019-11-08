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
void Telop::initialize(LPDIRECT3DTEXTURE9 _texture, int _pivot,
	int _width, int _height, D3DXVECTOR3 _position, D3DXVECTOR3 _rotation, D3DCOLOR color)
{

	input = getInput();
	telopFlag = false;
	state = CLOSE;
	telopTimer = 0.0f;
	displayTimer = 0.0f;
	

	sprite = new Sprite;
	sprite->initialize(_texture, _pivot,
		_width,  _height, _position, _rotation, color);
	
	
}


//=============================================================================
// �I������
//=============================================================================
void Telop::uninitialize()
{
	SAFE_DELETE(sprite);
}


//=============================================================================
// �X�V����
//=============================================================================
void Telop::update(float _frameTime)
{
	frameTime = _frameTime;				//�t���[���^�C���̕ۑ�
	
	rate = 0.0f;
	switch (state)
	{
	case OPEN://�I�[�v��
		
		telopTimer += _frameTime;		//�V�[���^�C���̍X�V

		//�����̍X�V
		Telop::open();

		break;
	case DISPLAY:	

		//�\���ҋ@�@
		displayTimer -= _frameTime;
		Telop::display();
		
		break;
	case CLOSE:		

		//�N���[�Y
		Telop::close();
		
		//�I�[�v���֑J��
		if (telopFlag)
		{
			telopFlag = false;
			state = OPEN;
			telopTimer = 0.0f;
		}
		
		break;
	}


	//�T�C�Y�̍X�V
	sprite->setSize(WIDTH, heightValue);
	sprite->setVertex();
}


//=============================================================================
// �`�揈��
//=============================================================================
void Telop::render()
{
	if (telopTimer > 0)
	{
		sprite->render();
	}
	
}

//=============================================================================
// �I�[�v������
//=============================================================================
void Telop::open()
{
	//�����̍X�V
	rate = telopTimer / telopNS::OPEN_TIME;
	heightValue = (int)((float)telopNS::MAX_HEIGHT * rate);
	if (telopTimer > telopNS::OPEN_TIME)
	{
		state = telopNS::DISPLAY;
		displayTimer = telopNS::DISPLAY_TIME;
		telopTimer = telopNS::CLOSE_TIME;
	}
}

//=============================================================================
// �ҋ@����
//=============================================================================
void Telop::display()
{
	if (displayTimer < 0)
	{
		state = CLOSE;
		displayTimer = 0.0f;
		closeTimer = 0.0f;
	}
}
//=============================================================================
// �N���[�Y����
//=============================================================================
void Telop::close()
{
	if (telopTimer > 0)
	{
		telopTimer -= frameTime;
		closeTimer += frameTime;
		//�����̍X�V
		rate = telopTimer / CLOSE_TIME;
		heightValue = (int)((float)MAX_HEIGHT * rate);
		if (closeTimer >= CLOSE_TIME)
		{
			*playFlag = false;
		}
	}
}


//=============================================================================
// �e���b�v�Đ�����
//=============================================================================
void Telop::playTelop()
{
	telopFlag = true;
}

void Telop::setManagerFlag(bool* managerFlag)
{
	playFlag = managerFlag;
}