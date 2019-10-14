//===================================================================================================================================
//�ySound.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/26
// [�X�V��]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "SE.h"
#include "BGM.h"

//===================================================================================================================================
//�y�}�N����`�z
//===================================================================================================================================
//�T�E���h���
#define ENDPOINT_INPUT_CHANNEL	(2)			//�G���h�|�C���g�{�C�X�̃`�����l����(����)
#define ENDPOINT_SAMPLE_RATE		(44100)		//�G���h�|�C���g�{�C�X�̃T���v�����O���g��(����)

//�{�C�X�������
#define SAFE_DESTROY_VOICE(p)	if(p){ p->DestroyVoice(); p=NULL; }

//===================================================================================================================================
//�y�񋓌^�萔�z
//===================================================================================================================================
enum ENDPOINT_VOICE_LIST
{
	ENDPOINT_BGM,
	ENDPOINT_SE,
	ENDPOINT_MAX
};

//===================================================================================================================================
//�y�T�E���h(XAudio2)�z
//�T�E���h�C���^�t�F�[�X�N���X
//===================================================================================================================================
class SoundInterface
{
private:
	//�ϐ�
	IXAudio2MasteringVoice				*MasteringVoice;											//XAudio2 Mastering Voice
	IXAudio2SubmixVoice					*EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_MAX];		//XAudio2 Submix Vice(Endpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR				SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_MAX];		//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)

public:
	SoundInterface();
	~SoundInterface();

	//�N���X
	static SEManager *SE;
	static BGMManager *BGM;
	
	//�ϐ�
	static IXAudio2						*XAudio2Interface;										//XAudio2 COM interface
	static XAUDIO2_VOICE_SENDS			SendList[ENDPOINT_VOICE_LIST::ENDPOINT_MAX];				//XAudio2 Send List(BGM/SE Endpoint Voice)

	//�֐�
	static XAUDIO2_VOICE_SENDS			GetSendList(int endpointVoiceId);						//���M���X�g���擾����
	static IXAudio2						*GetXAudio2Interface(void);								//XAudio2�̃C���^�t�F�[�X���擾����
	static void							playSound(int endpointVoiceId, int soundId, bool loop);	//�Đ�
	static void							stopSound(int endpointVoiceId, int soundId, bool loop);	//��~
	void									UpdateSound(void);										//�X�V����
	void									outputSoundGUI(void);									//ImGUI�ւ̏o��
};