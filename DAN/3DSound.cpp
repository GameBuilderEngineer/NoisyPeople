//===================================================================================================================================
//�y3DSound.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/29
// [�X�V��]2019/10/29
//===================================================================================================================================
#include "3DSound.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"
#include "Player.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
X3DSoundManager::X3DSoundManager()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//�~�b�h�|�C���g�{�C�X�̍쐬(P1)
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&MidpointVoice[playerNS::PLAYER1],									//�T�u�~�b�N�X�{�C�X
		ENDPOINT_INPUT_CHANNEL,												//�`�����l����(����)
		ENDPOINT_SAMPLE_RATE,												//�T���v�����O���[�g(����)
		XAUDIO2_VOICE_USEFILTER,												//�t�B���^�[�@�\
		NULL,																//�Ӗ��Ȃ�����
		SendList,															//���M���X�g(S3D)
		NULL)))																//�G�t�F�N�g�`�F�[��
	{
		return;
	}
	//�~�b�h�|�C���g�{�C�X�̍쐬(P2)
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&MidpointVoice[playerNS::PLAYER2],									//�T�u�~�b�N�X�{�C�X
		ENDPOINT_INPUT_CHANNEL,												//�`�����l����(����)
		ENDPOINT_SAMPLE_RATE,												//�T���v�����O���[�g(����)
		XAUDIO2_VOICE_USEFILTER,												//�t�B���^�[�@�\
		NULL,																//�Ӗ��Ȃ�����
		SendList,															//���M���X�g(S3D)
		NULL)))																//�G�t�F�N�g�`�F�[��
	{
		return;
	}

	//�~�b�h�|�C���g�{�C�X�ւ̑��M���X�g�̍쐬
	SendDescriptor[playerNS::PLAYER1] = { XAUDIO2_SEND_USEFILTER,MidpointVoice[playerNS::PLAYER1] };
	SendList[playerNS::PLAYER1] = { 1,&SendDescriptor[playerNS::PLAYER1] };
	SendDescriptor[playerNS::PLAYER2] = { XAUDIO2_SEND_USEFILTER,MidpointVoice[playerNS::PLAYER2] };
	SendList[playerNS::PLAYER2] = { 1,&SendDescriptor[playerNS::PLAYER2] };

	////�~�b�h�|�C���g�{�C�X�̏�����
	//MidpointVoice[playerNS::PLAYER2]->SetOutputMatrix()

#endif
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
X3DSoundManager::~X3DSoundManager()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//�~�b�h�|�C���g�{�C�X
	SAFE_DESTROY_VOICE(MidpointVoice[playerNS::PLAYER1])
	SAFE_DESTROY_VOICE(MidpointVoice[playerNS::PLAYER2])
#endif
}

