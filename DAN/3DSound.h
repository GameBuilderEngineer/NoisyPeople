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
//�y�}�N����`�z
//===================================================================================================================================
#define S3D_PATH_LIST_TAIL(num)			S3DPathList##[num]

//===================================================================================================================================
//�y�񋓌^�萔�z
//===================================================================================================================================
enum S3D_LIST
{
	S3D_ShiftStart,
	S3D_ShiftFinish,
	S3D_Vision,
	S3D_VisionStart,
	S3D_VisionFinish,
	S3D_SkyVisionStart,
	S3D_SkyVisionFinish,
	S3D_PLAYER_WALK,
	Voice_Male_Start,
	Voice_Male_Finish,
	Voice_Female_Start,
	Voice_Female_Finish,
	S3D_MAX
};

//===================================================================================================================================
//�y�萔��`�z
//===================================================================================================================================
static const char *S3DPathList[] = { "3D_ShiftStart.wav", "3D_ShiftFinish.wav","3D_Vision.wav",
									"3D_VisionStart.wav","3D_VisionFinish.wav",	"3D_SkyVisionStart.wav",
									"3D_SkyVisionFinish.wav","3D_PLAYER_WALK.wav" ,"Voice_Male_Start.wav" ,
									"Voice_Male_Finish.wav","Voice_Female_Start.wav","Voice_Female_Finish.wav" };

//===================================================================================================================================
//�y�T�E���h(XAudio2)�z
//�T�E���h��3DSound�N���X
//===================================================================================================================================
class S3DManager:public SoundBase
{
public:
	S3DManager();
	~S3DManager();

	//��{�@�\
	void SetVolume(const PLAY_PARAMETERS playParameters, float volume);	//�{�����[��

	//debug�p
#if _DEBUG
	void	 outputGUI(void)override;				//ImGUI�ւ̏o��
#endif

private:
#if(XADUIO2_STATE)
	//�~�b�h�|�C���g�{�C�X
	IXAudio2SubmixVoice		*MidpointVoice[gameMasterNS::PLAYER_NUM];	//XAudio2 Midpoint Voice[2 player]
	XAUDIO2_VOICE_SENDS		MidSendList[gameMasterNS::PLAYER_NUM];		//XAudio2 Send List(P1/P2 Midpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR	SendDescriptor[gameMasterNS::PLAYER_NUM];	//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)

	//�\�[�X�{�C�X�̍쐬
	void MakeSourceVoice(PLAY_PARAMETERS *playParameters, LIST_BUFFER *listBuffer)override;

	void loadBuffer(void)override;
#endif
};