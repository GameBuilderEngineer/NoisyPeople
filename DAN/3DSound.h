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
	S3D_Wolf_Foot_Step,
	S3D_Bear_Foot_Step,
	S3D_Tiger_Foot_Step,
	Voice_Male_Start,
	Voice_Male_Finish,
	Voice_Female_Start,
	Voice_Female_Finish,
	Voice_Male_Shift1,
	Voice_Male_Shift2,
	Voice_Male_Jump1,
	Voice_Male_Jump2,
	Voice_Male_Damage1,
	Voice_Male_Damage2,
	Voice_Female_Damage1,
	Voice_Female_Damage2,
	Voice_Female_Jump1,
	Voice_Female_Jump2,
	Voice_Female_Shift1,
	Voice_Female_Shift2,
	S3D_MAX
};

//===================================================================================================================================
//�y�萔��`�z
//===================================================================================================================================
static const char *S3DPathList[] = { "3D_ShiftStart.wav", "3D_ShiftFinish.wav","3D_Vision.wav",
									"3D_VisionStart.wav","3D_VisionFinish.wav",	"3D_SkyVisionStart.wav",
									"3D_SkyVisionFinish.wav","3D_PLAYER_WALK.wav" ,"3D_Wolf_Foot_Step.wav",
									"3D_Bear_Foot_Step.wav","3D_Tiger_Foot_Step.wav","Voice_Male_Start.wav" ,
									"Voice_Male_Finish.wav","Voice_Female_Start.wav","Voice_Female_Finish.wav",
									"Voice_Male_Shift1.wav","Voice_Male_Shift2.wav","Voice_Male_Jump1.wav",
									"Voice_Male_Jump2.wav","Voice_Male_Damage1.wav","Voice_Male_Damage2.wav",
									"Voice_Female_Damage1.wav","Voice_Female_Damage2.wav","Voice_Female_Jump1.wav",
									"Voice_Female_Jump2.wav","Voice_Female_Shift1.wav","Voice_Female_Shift2.wav" };

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