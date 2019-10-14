//===================================================================================================================================
//�ySE.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/04
// [�X�V��]2019/10/04
//===================================================================================================================================
#include "SE.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
const char * const SEManager::splashSEPathList[] = { "SE_Attack.wav"};
const char * const SEManager::titleSEPathList[] = { "SE_Game_Start.wav","SE_Installation_Memory_Pile.wav" };
const char * const SEManager::gameSEPathList[] = { "SE_Game_Start.wav","SE_Installation_Memory_Pile.wav" };
int SEManager::SEScene = SceneList::SPLASH;

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
SEManager::~SEManager()
{
	SAFE_DELETE_ARRAY(SEBufferList);
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void SEManager::outputSEGUI(void)
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("SEInformation"))
	{
		ImGui::Text("Buffer List:%d", SEBufferMax);

		ImGui::Text("Playing List:");
		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
			{
				if (tmpSoundParameters->isPlaying)		//�Đ����Ă���
				{
					switch (SEScene)
					{
					case SceneList::SPLASH:
						ImGui::Text("%s", splashSEPathList[tmpSoundParameters->soundId]);
						break;
					case SceneList::TITLE:
						ImGui::Text("%s", titleSEPathList[tmpSoundParameters->soundId]);
						break;
					case SceneList::TUTORIAL:

						break;
					case SceneList::CREDIT:

						break;
					case SceneList::GAME:
						ImGui::Text("%s", gameSEPathList[tmpSoundParameters->soundId]);
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
void	 SEManager::SwitchAudioBuffer(int scene)
{
	//�T�E���h�f�B���N�g���ɐݒ肷��
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_SE);

	SAFE_DELETE_ARRAY(SEBufferList);

	//�V�[���̍X�V
	SEScene = scene;

	switch (SEScene)
	{
	case SceneList::SPLASH:
		SEManager::SEBufferList = new LIST_BUFFER[SPLASH_SE_LIST::SPLASH_SE_MAX];
		for (int i = 0; i < SPLASH_SE_LIST::SPLASH_SE_MAX; i++)
		{
			memset(&SEManager::SEBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(SEManager::splashSEPathList[i], "rb");
			SEManager::SEBufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::SEBufferList[i].buffer.pAudioData = (BYTE*)SEManager::SEBufferList[i].wavFile.data.waveData;
			SEManager::SEBufferList[i].buffer.AudioBytes = SEManager::SEBufferList[i].wavFile.data.waveSize;
			SEManager::SEBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		SEBufferMax = SPLASH_SE_LIST::SPLASH_SE_MAX;
		break;
	case SceneList::TITLE:
		SEManager::SEBufferList = new LIST_BUFFER[TITLE_SE_LIST::TITLE_SE_MAX];
		for (int i = 0; i < TITLE_SE_LIST::TITLE_SE_MAX; i++)
		{
			memset(&SEManager::SEBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(SEManager::titleSEPathList[i], "rb");
			SEManager::SEBufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::SEBufferList[i].buffer.pAudioData = (BYTE*)SEManager::SEBufferList[i].wavFile.data.waveData;
			SEManager::SEBufferList[i].buffer.AudioBytes = SEManager::SEBufferList[i].wavFile.data.waveSize;
			SEManager::SEBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		SEBufferMax = TITLE_SE_LIST::TITLE_SE_MAX;
		break;
	case SceneList::TUTORIAL:
		break;
	case SceneList::CREDIT:
		break;
	case SceneList::GAME:
		SEManager::SEBufferList = new LIST_BUFFER[GAME_SE_LIST::GAME_SE_MAX];
		for (int i = 0; i < GAME_SE_LIST::GAME_SE_MAX; i++)
		{
			memset(&SEManager::SEBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(SEManager::gameSEPathList[i], "rb");
			SEManager::SEBufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::SEBufferList[i].buffer.pAudioData = (BYTE*)SEManager::SEBufferList[i].wavFile.data.waveData;
			SEManager::SEBufferList[i].buffer.AudioBytes = SEManager::SEBufferList[i].wavFile.data.waveSize;
			SEManager::SEBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		SEBufferMax = GAME_SE_LIST::GAME_SE_MAX;
		break;
	case SceneList::RESULT:	
		break;
	case SceneList::NONE_SCENE:
		break;
	default:
		break;
	}
}