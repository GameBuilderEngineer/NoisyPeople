//===================================================================================================================================
//�yBGM.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/14
// [�X�V��]2019/10/14
//===================================================================================================================================
#include "BGM.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
const char * const BGMManager::splashBGMPathList[] = { "BGM_Splash.wav" };
const char * const BGMManager::titleBGMPathList[] = { "BGM_Title.wav" };
const char * const BGMManager::gameBGMPathList[] = { "BGM_Game.wav" };
int BGMManager::BGMScene = SceneList::SPLASH;
float BGMManager::startTime = 0;
float BGMManager::currentTime = 0;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
BGMManager::BGMManager()
{

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
BGMManager::~BGMManager()
{
	SAFE_DELETE_ARRAY(BGMBufferList);
}

//===================================================================================================================================
//�y�Đ����x�̐ݒ�z
//===================================================================================================================================
void	 BGMManager::SetSpeed(float speed)
{
	//�^�[�Q�b�g���
	SOUND_PARAMETERS targetSoundParameters;
	targetSoundParameters.playParameters.endpointVoiceId = ENDPOINT_VOICE_LIST::ENDPOINT_BGM;
	targetSoundParameters.playParameters.soundId = GAME_BGM_LIST::GAME_BGM_01;
	targetSoundParameters.playParameters.loop = true;

	if (BGMScene == SceneList::GAME)
	{
		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
			if ((tmpSoundParameters->playParameters.endpointVoiceId == targetSoundParameters.playParameters.endpointVoiceId) &&
				(tmpSoundParameters->playParameters.soundId == targetSoundParameters.playParameters.soundId) &&
				(tmpSoundParameters->isSpeed))
			{
				//�o�b�t�@
				LIST_BUFFER *tmpBuffer = GetBuffer(targetSoundParameters.playParameters.endpointVoiceId,
					targetSoundParameters.playParameters.soundId,
					targetSoundParameters.playParameters.loop);

				//��~����
				tmpSoundParameters->SourceVoice->Stop(XAUDIO2_PLAY_TAILS);
				XAUDIO2_VOICE_STATE state = { 0 };
				tmpSoundParameters->SourceVoice->Discontinuity();
				tmpSoundParameters->SourceVoice->FlushSourceBuffers();
				tmpSoundParameters->SourceVoice->GetState(&state);

				//�L���[�ɉ��ł��Ȃ������
				if (state.BuffersQueued == 0)
				{
					//�Đ����x
					tmpSoundParameters->SourceVoice->SetSourceSampleRate(
						tmpBuffer->wavFile.fmt.fmtSampleRate * tmpSoundParameters->playParameters.speed);

					//��~�ʒu
					tmpSoundParameters->stopPoint = state.SamplesPlayed;
					while (tmpSoundParameters->stopPoint >= (int)(tmpBuffer->wavFile.data.waveSize/(int)sizeof(long)))
					{
						tmpSoundParameters->stopPoint -= (int)(tmpBuffer->wavFile.data.waveSize / (int)sizeof(long));
					}

					//�Đ��ʒu
					tmpBuffer->buffer.PlayBegin = tmpSoundParameters->stopPoint;

					//�o�b�t�@�̒�o
					tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&tmpBuffer->buffer);
					
					//�Đ�����
					tmpSoundParameters->SourceVoice->Start();

					//�Đ����x
					tmpSoundParameters->isSpeed = false;
				}
			}			
		}
	}
}

//===================================================================================================================================
//�y�Đ����x�̐ݒ�(On)�z
//===================================================================================================================================
void BGMManager::SetSpeedOn(void)
{

}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void BGMManager::outputBGMGUI(void)
{
#ifdef _DEBUG
	if (!ImGui::CollapsingHeader("BGMInformation"))
	{
		ImGui::Text("Buffer List:%d", BGMBufferMax);

		ImGui::Text("Playing List:");
		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			XAUDIO2_VOICE_STATE state = { 0 };
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
			{
				if (tmpSoundParameters->isPlaying)		//�Đ����Ă���
				{
					switch (BGMScene)
					{
					case SceneList::SPLASH:
						tmpSoundParameters->SourceVoice->GetState(&state);
						ImGui::Text("%s", splashBGMPathList[tmpSoundParameters->playParameters.soundId]);
						ImGui::Text("%d", ((int)state.SamplesPlayed / 44100));
						break;
					case SceneList::TITLE:
						tmpSoundParameters->SourceVoice->GetState(&state);
						ImGui::Text("%s", titleBGMPathList[tmpSoundParameters->playParameters.soundId]);
						ImGui::Text("%d", ((int)state.SamplesPlayed / 44100));
						break;
					case SceneList::TUTORIAL:

						break;
					case SceneList::CREDIT:

						break;
					case SceneList::GAME:
						tmpSoundParameters->SourceVoice->GetState(&state);
						ImGui::Text("%s", gameBGMPathList[tmpSoundParameters->playParameters.soundId]);
						ImGui::Text("%d", ((int)state.SamplesPlayed/44100));

						ImGui::Checkbox("Speed Up��", &tmpSoundParameters->isSpeed);
						break;
					case SceneList::RESULT:

						break;
					case SceneList::NONE_SCENE:

						break;
					default:
						break;
					}

				}
			}
		}
	}
#endif
}

//===================================================================================================================================
//�y�X�e�[�W�J�ڂɍ��킹�ĕK�v�ȃT�E���h�o�b�t�@��p�ӂ���z
//===================================================================================================================================
void	 BGMManager::SwitchAudioBuffer(int scene)
{
	//�T�E���h�f�B���N�g���ɐݒ肷��
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_BGM);

	SAFE_DELETE_ARRAY(BGMBufferList);

	//�V�[���̍X�V
	BGMScene = scene;

	switch (BGMScene)
	{
	case SceneList::SPLASH:
		BGMManager::BGMBufferList = new LIST_BUFFER[SPLASH_BGM_LIST::SPLASH_BGM_MAX];
		for (int i = 0; i < SPLASH_BGM_LIST::SPLASH_BGM_MAX; i++)
		{
			memset(&BGMManager::BGMBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(BGMManager::splashBGMPathList[i], "rb");
			BGMManager::BGMBufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::BGMBufferList[i].soundId = i;
			BGMManager::BGMBufferList[i].buffer.pAudioData = (BYTE*)BGMManager::BGMBufferList[i].wavFile.data.waveData;
			BGMManager::BGMBufferList[i].buffer.AudioBytes = BGMManager::BGMBufferList[i].wavFile.data.waveSize;
			BGMManager::BGMBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		BGMBufferMax = SPLASH_BGM_LIST::SPLASH_BGM_MAX;
		break;
	case SceneList::TITLE:
		BGMManager::BGMBufferList = new LIST_BUFFER[TITLE_BGM_LIST::TITLE_BGM_MAX];
		for (int i = 0; i < TITLE_BGM_LIST::TITLE_BGM_MAX; i++)
		{
			memset(&BGMManager::BGMBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(BGMManager::titleBGMPathList[i], "rb");
			BGMManager::BGMBufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::BGMBufferList[i].soundId = i;
			BGMManager::BGMBufferList[i].buffer.pAudioData = (BYTE*)BGMManager::BGMBufferList[i].wavFile.data.waveData;
			BGMManager::BGMBufferList[i].buffer.AudioBytes = BGMManager::BGMBufferList[i].wavFile.data.waveSize;
			BGMManager::BGMBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		BGMBufferMax = TITLE_BGM_LIST::TITLE_BGM_MAX;
		break;
	case SceneList::TUTORIAL:
		break;
	case SceneList::CREDIT:
		break;
	case SceneList::GAME:
		BGMManager::BGMBufferList = new LIST_BUFFER[GAME_BGM_LIST::GAME_BGM_MAX];
		for (int i = 0; i < GAME_BGM_LIST::GAME_BGM_MAX; i++)
		{
			memset(&BGMManager::BGMBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(BGMManager::gameBGMPathList[i], "rb");
			BGMManager::BGMBufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::BGMBufferList[i].soundId = i;
			BGMManager::BGMBufferList[i].buffer.pAudioData = (BYTE*)BGMManager::BGMBufferList[i].wavFile.data.waveData;
			BGMManager::BGMBufferList[i].buffer.AudioBytes = BGMManager::BGMBufferList[i].wavFile.data.waveSize;
			BGMManager::BGMBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		BGMBufferMax = GAME_BGM_LIST::GAME_BGM_MAX;
		break;
	case SceneList::RESULT:
		break;
	case SceneList::NONE_SCENE:
		break;
	default:
		break;
	}
}