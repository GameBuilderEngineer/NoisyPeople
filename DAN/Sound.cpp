//===================================================================================================================================
//�ySound.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/26
// [�X�V��]2019/10/04
//===================================================================================================================================
#include "Sound.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
#if(_MSC_VER >= GAME_MSC_VER)
IXAudio2 *SoundInterface::XAudio2Interface = nullptr;
SEManager *SoundInterface::SE = nullptr;
BGMManager *SoundInterface::BGM = nullptr;
#endif

//===================================================================================================================================
//�y�R���X�g���N�^�z
//�E�C���^�t�F�[�X�̏���
//�E�}�X�^�[�{�C�X�̍쐬
//�E�G���h�|�C���g�{�C�X�̍쐬
//�E�G���h�|�C���g�ւ̑��M���X�g�̍쐬
//===================================================================================================================================
SoundInterface::SoundInterface()
{
#if(_MSC_VER >= GAME_MSC_VER)
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

	//�}�l�[�W���[
	SE = new SEManager();
	BGM = new BGMManager();

#endif
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
SoundInterface::~SoundInterface()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//SE
	SAFE_DELETE(SE);
	
	//BGM
	SAFE_DELETE(BGM);

	//�}�X�^�[�{�C�X
	SAFE_DESTROY_VOICE(MasteringVoice);

	//�C���^�[�t�F�[�X
	SAFE_RELEASE(XAudio2Interface);

	//COM���C�u����
	CoUninitialize();
#endif
}

//===================================================================================================================================
//�yXAudio2�̃C���^�t�F�[�X���擾����z
//===================================================================================================================================
#if(_MSC_VER >= GAME_MSC_VER)
IXAudio2 *SoundInterface::GetXAudio2Interface(void)
{
	return XAudio2Interface;
}
#endif

//===================================================================================================================================
//�y�V�[���̍X�V�z
//===================================================================================================================================
void SoundInterface::SwitchAudioBuffer(int scene)
{
#if(_MSC_VER >= GAME_MSC_VER)
	//�V�[���̍X�V
	BGM->SwitchAudioBuffer(scene);
	SE->SwitchAudioBuffer(scene);
#endif
}

//===================================================================================================================================
//�y�X�V�����z
//===================================================================================================================================
void SoundInterface::UpdateSound(void)
{
#if(_MSC_VER >= GAME_MSC_VER)
	//SE�̍X�V����
	SE->updateSound();

	//BGM�̍X�V����
	BGM->updateSound();
	BGM->SetSpeed();

	//ImGUI
#ifdef _DEBUG
	outputSoundGUI();
#endif
#endif
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void SoundInterface::outputSoundGUI(void)
{
#ifdef _DEBUG
#if(_MSC_VER >= GAME_MSC_VER)
	ImGui::Begin("SoundInformation");
	
	//SE
	SE->outputGUI();
	BGM->outputGUI();

	ImGui::End();
#endif
#endif
}
