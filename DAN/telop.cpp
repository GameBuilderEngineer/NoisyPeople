//-----------------------------------------------------------------------------
// �e���b�v�N���X [Telop.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/17
//-----------------------------------------------------------------------------
#include "Telop.h"
using namespace telopNS;


//=============================================================================
// ������
//=============================================================================
void Telop::initialize()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Sprite;
	}

	telop[TELOP_TYPE0]->initialize(
		*textureNS::reference(textureNS::TITLE_LOGO),
		SpriteNS::CENTER,
		WIDTH,
		HEIGHT,
		POSITION,
		ROTATION,
		COLOR
	);
	//telop[TELOP_TYPE1]->initialize
	/*telop[TELOP_TYPE2]->initialize
	telop[TELOP_TYPE3]->initialize
	telop[TELOP_TYPE4]->initialize
	telop[TELOP_TYPE5]->initialize*/

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
void Telop::update(float frameTime)
{

}


//=============================================================================
// �`�揈��
//=============================================================================
void Telop::render()
{
	telop[TELOP_TYPE0]->render();
}
