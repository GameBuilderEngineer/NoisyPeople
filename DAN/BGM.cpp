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
void	 BGMManager::SetSpeed(void)
{
	////�^�[�Q�b�g���
	//SOUND_PARAMETERS targetSoundParameters;
	//targetSoundParameters.playParameters.endpointVoiceId = ENDPOINT_VOICE_LIST::ENDPOINT_BGM;
	//targetSoundParameters.playParameters.soundId = GAME_BGM_LIST::GAME_BGM_01;
	//targetSoundParameters.playParameters.loop = true;

	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
		if(tmpSoundParameters->isSpeed)
		{
			//�o�b�t�@
			LIST_BUFFER *tmpBuffer = GetBuffer(tmpSoundParameters->playParameters.endpointVoiceId, tmpSoundParameters->playParameters.soundId, tmpSoundParameters->playParameters.loop);

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
					(UINT32)(tmpBuffer->wavFile.fmt.fmtSampleRate * tmpSoundParameters->playParameters.speed));

				//��~�ʒu
				tmpSoundParameters->stopPoint = (long)state.SamplesPlayed;
				while (tmpSoundParameters->stopPoint >= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel))
				{
					tmpSoundParameters->stopPoint -= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel);
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

//===================================================================================================================================
//�y�Đ����x�̐ݒ�(On)�z
//===================================================================================================================================
void BGMManager::SetSpeedOn(void)
{

}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
#ifdef _DEBUG
void BGMManager::outputBGMGUI(void)
{
	if (!ImGui::CollapsingHeader("BGMInformation"))
	{
		//�g�p���̃o�b�t�@��
		ImGui::Text("Number of buffers:%d", BGMBufferMax);
		
		//�g�p���̃{�C�X��
		ImGui::Text("Number of voice:%d", soundParametersList->nodeNum - 1);

		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			//�T�E���h�̃p�����[�^�[
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

			//�o�b�t�@
			LIST_BUFFER *tmpBuffer = GetBuffer(
				tmpSoundParameters->playParameters.endpointVoiceId,
				tmpSoundParameters->playParameters.soundId,
				tmpSoundParameters->playParameters.loop);

			//�Đ��ʒu&���擾
			XAUDIO2_VOICE_STATE state = { 0 };
			tmpSoundParameters->SourceVoice->GetState(&state);
			long playPoint = (long)state.SamplesPlayed;
			while (playPoint >= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel))
			{
				playPoint -= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel);
			}

			//�Đ����Ă���
			if (tmpSoundParameters->isPlaying)
			{
				//���x�̃o�b�N�A�b�v
				float backUpSpeed = tmpSoundParameters->playParameters.speed;

				switch (BGMScene)
				{
				case SceneList::SPLASH:
					//�T�E���h��
					ImGui::Text("%s", splashBGMPathList[tmpSoundParameters->playParameters.soundId]);
					break;
				case SceneList::TITLE:
					//�T�E���h��
					ImGui::Text("%s", titleBGMPathList[tmpSoundParameters->playParameters.soundId]);
					break;
				case SceneList::TUTORIAL:

					break;
				case SceneList::CREDIT:

					break;
				case SceneList::GAME:
					//�T�E���h��
					ImGui::Text("%s", gameBGMPathList[tmpSoundParameters->playParameters.soundId]);
					break;
				case SceneList::RESULT:

					break;
				case SceneList::NONE_SCENE:

					break;
				default:
					break;
				}

				//�g�`�̕`��
				int saveDataMax = 11024;	//�擾����f�[�^��
				int dataMax = (saveDataMax / tmpBuffer->wavFile.fmt.fmtChannel) + 2;	 //�Z�[�u�������f�[�^�̐�/�`�����l���� + 2
				float *fData = new (float[dataMax]);
				memset(fData, 0, sizeof(float)*dataMax);
				fData[0] = (float)SHRT_MIN;
				fData[dataMax - 1] = (float)SHRT_MAX;
				int wtPos = dataMax - 2;
				for (int j = saveDataMax - 1; j > 0; j -= tmpBuffer->wavFile.fmt.fmtChannel)
				{
					for (int k = 0; k < tmpBuffer->wavFile.fmt.fmtChannel; k++)
					{
						if (playPoint - j < 0)
						{
							fData[wtPos] = 0.0f;
							continue;
						}

						fData[wtPos] += ((float)tmpBuffer->wavFile.data.waveData[playPoint - j - k] / (float)tmpBuffer->wavFile.fmt.fmtChannel);
					}
					wtPos--;
				}
				ImVec2 plotextent(ImGui::GetContentRegionAvailWidth(), 100);
				ImGui::PlotLines("", fData, dataMax, 0, "Sound wave", FLT_MAX, FLT_MAX, plotextent);

				SAFE_DELETE_ARRAY(fData);

				//for (int j = 11023; j > 0; j--)
//{
//	if (playPoint - j < 0)
//	{
//		lines[j] = 0.0f;
//		continue;
//	}
//	lines[j] = (float)tmpBuffer->wavFile.data.waveData[playPoint - j];
//}
//for (int j = 11024 / tmpBuffer->wavFile.fmt.fmtChannel; j > 0; j--)
//{
//	for (int k = 0; k < tmpBuffer->wavFile.fmt.fmtChannel; k++)
//	{
//		if (playPoint - j * tmpBuffer->wavFile.fmt.fmtChannel < 0)
//		{
//			lines[j * tmpBuffer->wavFile.fmt.fmtChannel - k] = 0.0f;
//			continue;
//		}
//		lines[j * tmpBuffer->wavFile.fmt.fmtChannel - k] = (float)tmpBuffer->wavFile.data.waveData[playPoint - (j * tmpBuffer->wavFile.fmt.fmtChannel - k)] / tmpBuffer->wavFile.fmt.fmtChannel;
//	}
//}
				////test
				//short data[10] = { 0,1,2,3,4,5,6,7,8,9 }; 
				//int tmpChannel = 1;
				//int saveDataMax = 10;
				//int dataMax = (saveDataMax / tmpChannel) + 2;	 //�Z�[�u�������f�[�^�̐�/�`�����l���� + 2
				//float *fData = new (float[dataMax]);
				//memset(fData, 0, sizeof(float)*dataMax);
				//fData[0] = (float)SHRT_MIN;
				//fData[dataMax - 1] = (float)SHRT_MAX;
				//int wtPos = dataMax - 2;
				//for (int j = saveDataMax - 1; j > 0; j -= tmpChannel)
				//{
				//	for (int k = 0; k < tmpChannel; k++)
				//	{
				//		fData[wtPos] += ((float)data[j - k] / (float)tmpChannel);
				//	}
				//	wtPos--;
				//}
				//ImGui::PlotLines("", fData, dataMax, 0, "tmp wave", FLT_MAX, FLT_MAX, plotextent);
				//ImGui::Text("0.5f 2.5f 4.5f 6.5f 8.5f");
				//SAFE_DELETE_ARRAY(fData);

				//�Đ��ʒu
				ImGui::ProgressBar(playPoint / (float)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel));

				//���x����
				ImGui::Text("%d", ((int)state.SamplesPlayed / 44100));
				ImGui::SliderFloat("Speed", &tmpSoundParameters->playParameters.speed, 0.1f, 3.0f);

				if (backUpSpeed != tmpSoundParameters->playParameters.speed)
				{
					tmpSoundParameters->isSpeed = true;
					BGMManager::SetSpeed();
				};
			}

		}
	}
}
#endif

//===================================================================================================================================
//�y�X�e�[�W�J�ڂɍ��킹�ĕK�v�ȃT�E���h�o�b�t�@��p�ӂ���z
//===================================================================================================================================
void	 BGMManager::SwitchAudioBuffer(int scene)
{
	//�T�E���h�f�B���N�g���ɐݒ肷��
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_BGM);

	//�V�[���̍X�V
	BGMManager::BGMScene = scene;

	//���
	for (int i = 0; i < BGMManager::BGMBufferMax; i++)
	{
		SAFE_DELETE_ARRAY(BGMManager::BGMBufferList[i].wavFile.data.waveData);
	}
	SAFE_DELETE_ARRAY(BGMManager::BGMBufferList);

	switch (BGMManager::BGMScene)
	{
	case SceneList::SPLASH:
		BGMManager::BGMBufferList = new LIST_BUFFER[SPLASH_BGM_LIST::SPLASH_BGM_MAX];
		for (int i = 0; i < SPLASH_BGM_LIST::SPLASH_BGM_MAX; i++)
		{
			BGMManager::BGMBufferList[i].buffer = { 0 };

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
			BGMManager::BGMBufferList[i].buffer = { 0 };

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
			BGMManager::BGMBufferList[i].buffer = { 0 };

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
	case SceneList::CREATE:
		BGMBufferMax = CREATE_BGM_LIST::CREATE_BGM_MAX;
		break;
	default:
		break;
	}
}