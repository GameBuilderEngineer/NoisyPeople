//===================================================================================================================================
//�ySound.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/26
// [�X�V��]2019/10/04
//===================================================================================================================================
#include "Sound.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
IXAudio2 *SoundInterface::XAudio2Interface = nullptr;
XAUDIO2_VOICE_SENDS SoundInterface::SendList[ENDPOINT_VOICE_LIST::ENDPOINT_MAX] = { NULL };
SEManager *SoundInterface::SE = new SEManager();

//===================================================================================================================================
//�y�R���X�g���N�^�z
//�E�C���^�t�F�[�X�̏���
//�E�}�X�^�[�{�C�X�̍쐬
//�E�G���h�|�C���g�{�C�X�̍쐬
//�E�G���h�|�C���g�ւ̑��M���X�g�̍쐬
//===================================================================================================================================
SoundInterface::SoundInterface()
{
	//COM���C�u�����̏�����
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return;
	}

	//XAudio2�I�u�W�F�N�g�̍쐬
	if (FAILED(XAudio2Create(&XAudio2Interface)))
	{
		return;
	}

	//�}�X�^�[�{�C�X�̍쐬
	if (FAILED(XAudio2Interface->CreateMasteringVoice(&MasteringVoice)))
	{
		return;
	}

	//�f�o�b�N�c�[��
	XAUDIO2_DEBUG_CONFIGURATION debugConfig{ NULL };
	debugConfig.LogThreadID = true;
	debugConfig.TraceMask = XAUDIO2_LOG_WARNINGS;
	debugConfig.TraceMask = debugConfig.TraceMask | XAUDIO2_LOG_FUNC_CALLS | XAUDIO2_LOG_DETAIL;
	debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
	XAudio2Interface->SetDebugConfiguration(&debugConfig);

	//�G���h�|�C���g�{�C�X�̍쐬(BGM)
	if (FAILED(XAudio2Interface->CreateSubmixVoice(
		&EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_BGM],	//�T�u�~�b�N�X�{�C�X
		ENDPOINT_INPUT_CHANNEL,								//�`�����l����(����)
		ENDPOINT_SAMPLE_RATE,								//�T���v�����O���[�g(����)
		XAUDIO2_VOICE_USEFILTER,								//�t�B���^�[�@�\
		NULL,												//�Ӗ��Ȃ�����
		NULL)))												//���M���X�g(NULL:Mastering Voice�ւ̒P��̏o�͂ƂȂ�)
	{														//�G�t�F�N�g�`�F�[��
		return;
	}
	//�G���h�|�C���g�{�C�X�̍쐬(SE)
	if (FAILED(XAudio2Interface->CreateSubmixVoice(
		&EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_SE],	//�T�u�~�b�N�X�{�C�X
		ENDPOINT_INPUT_CHANNEL,								//�`�����l����(����)
		ENDPOINT_SAMPLE_RATE,								//�T���v�����O���[�g(����)
		XAUDIO2_VOICE_USEFILTER,								//�t�B���^�[�@�\
		NULL,												//�Ӗ��Ȃ�����
		NULL,												//���M���X�g(NULL:Mastering Voice�ւ̒P��̏o�͂ƂȂ�)
		NULL)))												//�G�t�F�N�g�`�F�[��
	{
		return;
	}

	//�G���h�|�C���g�{�C�X�ւ̑��M���X�g�̍쐬
	SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] = { XAUDIO2_SEND_USEFILTER,EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] };
	SendList[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] = { 1,&SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] };
	SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_SE] = { XAUDIO2_SEND_USEFILTER,EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_SE] };
	SendList[ENDPOINT_VOICE_LIST::ENDPOINT_SE] = { 1,&SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_SE] };
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
SoundInterface::~SoundInterface()
{
	//SE
	SAFE_DELETE(SE)

	//�G���h�|�C���g�{�C�X
	SAFE_DESTROY_VOICE(EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_BGM])
	SAFE_DESTROY_VOICE(EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_SE])

	//�}�X�^�[�{�C�X
	SAFE_DESTROY_VOICE(MasteringVoice)

	//�C���^�[�t�F�[�X
	SAFE_RELEASE(XAudio2Interface)

	//COM���C�u����
	CoUninitialize();
}

//===================================================================================================================================
//�y���M���X�g���擾����z
//����:�G���h�|�C���g�{�C�X��ID
//===================================================================================================================================
XAUDIO2_VOICE_SENDS SoundInterface::GetSendList(int endpointVoiceId)
{
	return SendList[endpointVoiceId];
}

//===================================================================================================================================
//�yXAudio2�̃C���^�t�F�[�X���擾����z
//===================================================================================================================================
IXAudio2 *SoundInterface::GetXAudio2Interface(void)
{
	return XAudio2Interface;
}

//===================================================================================================================================
//�y�X�V�����z
//===================================================================================================================================
void SoundInterface::UpdateSound(void)
{
	//SE�̍X�V����
	SE->updateSound();
}

//===================================================================================================================================
//�y�Đ��z
//===================================================================================================================================
void SoundInterface::playSound(int soundType, int soundId, bool loop)
{
	if (soundType == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{

	}
	else if (soundType == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		//SE
		SE->playSound(soundId, loop);
	}
}