//-----------------------------------------------------------------------------
// �e���b�v�}�l�[�W���[�N���X [TelopManager.h]
// Author : �V���@���m
// �쐬�J�n�� : 2019/10/31
//-----------------------------------------------------------------------------
// �X�V�� : 2020/01/16 �y���� ���z
//-----------------------------------------------------------------------------

#pragma once
#include "Telop.h"
#include "Sprite.h"
#include "Sound.h"
#include "SoundBase.h"

//=============================================================================
// ���O���
//=============================================================================
namespace telopManagerNS
{
	// ! �V�������
	// ! ����������enum�Ƀi���o�����O�����Ă������ɐ�����U���Ă��邾����
	// ! ���O�ɂȂ��Ă��Ȃ��̂ő���cpp�t�@�C������Q�Ƃ���Ƃ��ɉ��������s���Ȃ̂�
	// ! ���Ԃ�����Ƃ��ɓK���Ȗ��O�ɒ����Ă����Ă��炦��Ƃ�������܂�

	// �e���b�v�^�C�v
	enum TYPE
	{
		TELOP_INFO_BAR,
		TELOP_TYPE0,	// �Ή���10%
		TELOP_TYPE1,	// �Ή���30%
		TELOP_TYPE2,	// �Ή���50%
		TELOP_TYPE3,	// �f�W�^���c���[�P��
		TELOP_TYPE4,	// �������E�`
		TELOP_TYPE5,	// �`���m�ł��Ȃ��Ȃ��Ă��܂���
		TELOP_TYPE6,	// �������Ԉȓ��ɓ��̌͂�؂�Ή�����I
		BOSS_ENTRY,		// ������j�󃍃{�����ꂽ�I
		BOSS_ENTRY2,	// �f�W�^���c���[�����킳��Ă��܂�
		BOSS_ENTRY3,	// �o������

		MAX_TELOP
	};

	struct OrderItem
	{
		bool request	= false;	//�v���m�F
		bool played		= false;	//�Đ�������������
		int telopType	= -1;		//�Đ�����e���b�v�^�C�v
	};

}

class TelopManager
{
private:
	Telop* telop[telopManagerNS::MAX_TELOP];

	bool playFlag;

	telopManagerNS::OrderItem	orderList[100];//���߃��X�g
	int							orderNum	= 0;//���݂̍Đ��Ώۃi���o�[
	int							requestNum	= 0;//���ߗv���V�[�N


	PLAY_PARAMETERS announceTelop;

public:
	TelopManager();
	~TelopManager();

	void initialize();
	void uninitialize();
	void update(float _frameTime);
	void render();
	void play(int type);

	void playOrder(int type);//�Đ��v��

};

