//===================================================================================================================================
//�yBGM.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/14
// [�X�V��]2019/10/14
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "LinkedList.h"
#include "SoundBase.h"

//===================================================================================================================================
//�y�񋓌^�萔�z
//===================================================================================================================================
enum SPLASH_BGM_LIST
{
	SPLASH_BGM_MAX
};

enum TITLE_BGM_LIST
{
	BGM_Title,
	TITLE_BGM_MAX
};

enum CREDIT_BGM_LIST
{
	BGM_Credit,
	CREDIT_BGM_MAX
};

enum GAME_BGM_LIST
{
	BGM_Game,
	GAME_BGM_MAX
};

enum TUTORIAL_BGM_LIST
{
	BGM_Tutorial,
	TUTORIAL_BGM_MAX
};

enum CREATE_BGM_LIST
{
	CREATE_BGM_MAX
};

enum RESULT_BGM_LIST
{
	BGM_Clear,
	BGM_Failed,
	RESULT_BGM_MAX
};

//===================================================================================================================================
//�y�萔��`�z
//===================================================================================================================================
static const char *creditBGMPathList[] = { "BGM_Credit.wav" };
static const char *titleBGMPathList[] = { "BGM_Title.wav" }; 
static const char *tutorialBGMPathList[] = { "BGM_Tutorial.wav" };
static const char *gameBGMPathList[] = { "BGM_Game.wav" };
static const char *resultBGMPathList[] = { "BGM_Clear.wav","BGM_Failed.wav" };
//===================================================================================================================================
//�y�T�E���h(XAudio2)�z
//�T�E���h��BGM�N���X
//===================================================================================================================================
class BGMManager : public SoundBase
{
public:
	BGMManager();
	~BGMManager();

	//�֐�
	void	 SwitchAudioBuffer(int scene)override;	//�X�e�[�W�J�ڂɍ��킹�ĕK�v�ȃT�E���h�o�b�t�@��p�ӂ���
	void	 SetSpeed(void);							//�Đ����x�̐ݒ�
	
	//debug�p
#if _DEBUG
	void	 outputGUI(void)override;				//ImGUI�ւ̏o��
#endif
};