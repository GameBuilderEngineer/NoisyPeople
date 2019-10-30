//===================================================================================================================================
//�y3DSound.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/29
// [�X�V��]2019/10/29
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "GameMaster.h"
#include "LinkedList.h"
#include "SoundBase.h"

//===================================================================================================================================
//�y�T�E���h(XAudio2)�z
//�T�E���h��3DSound�N���X
//===================================================================================================================================
class X3DSoundManager:public SoundBase
{
public:
	X3DSoundManager();
	~X3DSoundManager();

private:
#if(_MSC_VER >= GAME_MSC_VER)
	IXAudio2SubmixVoice		*MidpointVoice[gameMasterNS::PLAYER_NUM];	//XAudio2 Midpoint Voice[2 player]
	XAUDIO2_VOICE_SENDS		SendList[gameMasterNS::PLAYER_NUM];			//XAudio2 Send List(P1/P2 Midpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR	SendDescriptor[gameMasterNS::PLAYER_NUM];	//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)
#endif
};