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
#define S3D_PATH_LIST_TAIL(scene,num)			scene##S3DPathList##num

//===================================================================================================================================
//�y�񋓌^�萔�z
//===================================================================================================================================
enum SPLASH_S3D_LIST
{
	SPLASH_S3D_MAX
};

enum TITLE_S3D_LIST
{
	TITLE_S3D_MAX
};

enum CREDIT_S3D_LIST
{
	CREDIT_S3D_MAX
};

enum GAME_S3D_LIST
{
	S3D_GAME_FOOTSTEP_01,
	S3D_GAME_FOOTSTEP_02,
	S3D_GAME_FOOTSTEP_03,
	GAME_S3D_MAX
};

enum TUTORIAL_S3D_LIST
{
	S3D_TUTORIAL_FOOTSTEP_01,
	S3D_TUTORIAL_FOOTSTEP_02,
	S3D_TUTORIAL_FOOTSTEP_03,
	TUTORIAL_S3D_MAX
};

enum CREATE_S3D_LIST
{
	CREATE_S3D_MAX
};

enum RESULT_S3D_LIST
{
	RESULT_S3D_MAX
};

//===================================================================================================================================
//�y�萔��`�z
//===================================================================================================================================
static const char *gameS3DPathList[] = { "3D_ADD_TREE01.wav","3D_Enemy_Walk00.wav","3D_Enemy_Walk01.wav" };
static const char *tutorialS3DPathList[] = { "3D_ADD_TREE01.wav","3D_Enemy_Walk00.wav","3D_Enemy_Walk01.wav" };

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
	//void	 SwitchAudioBuffer(int scene)override;							//�X�e�[�W�J�ڂɍ��킹�ĕK�v�ȃT�E���h�o�b�t�@��p�ӂ���
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
#endif
};