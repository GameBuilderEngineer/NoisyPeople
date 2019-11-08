//-----------------------------------------------------------------------------
// �e���b�v�}�l�[�W���[�N���X [TelopManager.cpp]
// Author : �V���@���m
// �쐬�J�n�� : 2019/10/31
//-----------------------------------------------------------------------------
#include "TelopManager.h"

using namespace telopManagerNS;


TelopManager::TelopManager()
{
}


TelopManager::~TelopManager()
{
}

//=============================================================================
// ������
//=============================================================================
void TelopManager::initialize()
{

	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Telop;
	}

	//�e���b�v�o�[�̏�����
	telop[TELOP_INFO_BAR]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BAR),
		SpriteNS::CENTER,
		telopNS::WIDTH_BAR,
		telopNS::HEIGHT_BAR,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v0�̏�����
	telop[TELOP_TYPE0]->initialize(
		*textureNS::reference(textureNS::UI_INFO_10),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v1�̏�����
	telop[TELOP_TYPE1]->initialize(
		*textureNS::reference(textureNS::UI_INFO_30),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v2�̏�����
	telop[TELOP_TYPE2]->initialize(
		*textureNS::reference(textureNS::UI_INFO_50),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v3�̏�����
	telop[TELOP_TYPE3]->initialize(
		*textureNS::reference(textureNS::UI_INFO_ASSULT),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v4�̏�����
	telop[TELOP_TYPE4]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT1),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v5�̏�����
	telop[TELOP_TYPE5]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT2),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v6�̏�����
	telop[TELOP_TYPE6]->initialize(
		*textureNS::reference(textureNS::UI_INFO_MISSION),
		SpriteNS::TOP_LEFT,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);


}

//=============================================================================
// �I������
//=============================================================================
void TelopManager::uninitialize()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		SAFE_DELETE(telop[i]);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void TelopManager::update(float _frameTime)
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i]->update(_frameTime);
	}
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void TelopManager::render()
{
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i]->render();
	}
}


//=============================================================================
// �Đ�����
//=============================================================================
void TelopManager::play(int type)
{
	telop[type]->playTelop();
}