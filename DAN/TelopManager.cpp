//-----------------------------------------------------------------------------
// �e���b�v�}�l�[�W���[�N���X [TelopManager.cpp]
// Author : �V���@���m
// �쐬�J�n�� : 2019/10/31
//-----------------------------------------------------------------------------
// �X�V�� : 2020/01/16 �y���� ���z
//-----------------------------------------------------------------------------
#include "TelopManager.h"

using namespace telopManagerNS;


TelopManager::TelopManager()
{
	//�T�E���h�Đ�
	announceTelop = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_AnnounceTelop,false,NULL,false,NULL };
}


TelopManager::~TelopManager()
{
	uninitialize();
}

//=============================================================================
// ������
//=============================================================================
void TelopManager::initialize()
{

	playFlag = false;

	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i] = new Telop;
		telop[i]->setManagerFlag(&playFlag);
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
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v3�̏�����
	telop[TELOP_TYPE3]->initialize(
		*textureNS::reference(textureNS::UI_INFO_ASSULT),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v4�̏�����
	telop[TELOP_TYPE4]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT1),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v5�̏�����
	telop[TELOP_TYPE5]->initialize(
		*textureNS::reference(textureNS::UI_INFO_LIMIT2),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	//�e���b�v6�̏�����
	telop[TELOP_TYPE6]->initialize(
		*textureNS::reference(textureNS::UI_INFO_MISSION),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// ������j�󃍃{�����ꂽ�I
	telop[BOSS_ENTRY]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BOSS),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// ���̂܂܂ł̓f�W�^���c���[���󂳂�Ă��܂�
	telop[BOSS_ENTRY2]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BOSS2),
		SpriteNS::CENTER,
		telopNS::WIDTH,
		telopNS::MIN_HEIGHT,
		telopNS::POSITION,
		telopNS::ROTATION,
		telopNS::COLOR
	);

	// �o������I
	telop[BOSS_ENTRY3]->initialize(
		*textureNS::reference(textureNS::UI_INFO_BOSS3),
		SpriteNS::CENTER,
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
	//�Đ����łȂ�
	if (!playFlag)
	{
		//�Đ����߂̗v�����m�F
		if (orderList[orderNum].request)
		{
			//�Đ�
			play(orderList[orderNum].telopType);
		}
	}
	//�e�e���b�v�̍X�V
	for (int i = 0; i < MAX_TELOP; i++)
	{
		telop[i]->update(_frameTime);
	}

	//�e���b�v�̍X�V��Đ�����������Ă��āA���ݎw�����߂��v�����
	if (!playFlag && orderList[orderNum].request)
	{
		//�Đ�����Ă����e���b�v���߂��Đ��ς݂ɂ���
		orderList[orderNum].played = true;
		//���߃��X�g����i�߂�
		orderNum = (orderNum+1) % 100;//���b�v0�`99
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
	if (playFlag == false)
	{
		SoundInterface::SE->playSound(&announceTelop);
		telop[type]->playTelop();
		telop[TELOP_INFO_BAR]->playTelopBar();
		playFlag = true;
	}
}

//=============================================================================
// �Đ��v������
//=============================================================================
void TelopManager::playOrder(int type)
{
	orderList[requestNum].request = true;
	orderList[requestNum].telopType = type;
	requestNum++;
	if (requestNum >= 100)
	{
		//�z��̃��t���b�V��
		for (int i = 0; i < 100; i++)
		{
			if (orderList[i].played)
			{
				orderList[i].played = false;
				orderList[i].request = false;
				orderList[i].telopType = -1;
			}
		}
		requestNum = 0;
	}

}
