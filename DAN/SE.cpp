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
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
#ifdef _DEBUG
void SEManager::outputGUI(void)
{
#if(XADUIO2_STATE)
	if (!ImGui::CollapsingHeader("SEInformation"))
	{
		//�g�p���̃o�b�t�@��
		ImGui::Text("Number of buffers:%d", bufferMax);
		
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

			if (tmpSoundParameters->isPlaying)		//�Đ����Ă���
			{
				//�{�C�XID
				ImGui::Text("Voice ID:%d", tmpSoundParameters->playParameters.voiceID);

				//�T�E���h��
				switch (scene)
				{
				case SceneList::TITLE:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(tmpSoundParameters->playParameters.soundId));
					break;
				case SceneList::TUTORIAL:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(tmpSoundParameters->playParameters.soundId));
					break;
				case SceneList::CREDIT:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(tmpSoundParameters->playParameters.soundId));
					break;
				case SceneList::GAME:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(tmpSoundParameters->playParameters.soundId));
					break;
				case SceneList::RESULT:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(tmpSoundParameters->playParameters.soundId));
					break;
				default:
					break;
				}

				//�g�`�̕`��
				int saveDataMax = 5512;	//�擾����f�[�^��
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

						fData[wtPos] += ((float)tmpBuffer->wavFile.data.waveData[((playPoint - j) * tmpBuffer->wavFile.fmt.fmtChannel) - k] / (float)tmpBuffer->wavFile.fmt.fmtChannel);
					}
					wtPos--;
				}
				ImVec2 plotextent(ImGui::GetContentRegionAvailWidth(), 100);
				ImGui::PlotLines("", fData, dataMax, 0, "Sound wave", FLT_MAX, FLT_MAX, plotextent);

				SAFE_DELETE_ARRAY(fData);

				//�Đ��ʒu
				ImGui::ProgressBar(playPoint / (float)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel));
			}
		}
	}
#endif
}
#endif

//===================================================================================================================================
//�y�X�e�[�W�J�ڂɍ��킹�ĕK�v�ȃT�E���h�o�b�t�@��p�ӂ���z
//===================================================================================================================================
void SEManager::SwitchAudioBuffer(int scene)
{
#if(XADUIO2_STATE)
	//�T�E���h�f�B���N�g���ɐݒ肷��
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_SE);

#if _DEBUG
	//�V�[���̍X�V
	SEManager::scene = scene;
#endif

	//�������
	for (int i = 0; i < SEManager::bufferMax; i++)
	{
		SAFE_DELETE_ARRAY(SEManager::bufferList[i].wavFile.data.waveData);
	}
	SAFE_DELETE_ARRAY(SEManager::bufferList);

	switch (scene)
	{
	case SceneList::SPLASH:
		bufferMax = 0;
		break;

	case SceneList::TITLE:
		bufferMax = sizeof(titleSEPathList) / sizeof(int);
		SEManager::bufferList = new LIST_BUFFER[bufferMax];
		for (int i = 0; i < bufferMax; i++)
		{
			
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(titleSEPathList[i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);
			SEManager::bufferList[i].soundId = titleSEPathList[i];
			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		break;

	case SceneList::TUTORIAL:
		bufferMax = sizeof(tutorialSEPathList) / sizeof(int);
		SEManager::bufferList = new LIST_BUFFER[bufferMax];
		for (int i = 0; i < bufferMax; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(tutorialSEPathList[i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);
			SEManager::bufferList[i].soundId = tutorialSEPathList[i];
			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		break;

	case SceneList::CREDIT:
		bufferMax = sizeof(creditSEPathList)/sizeof(int);
		SEManager::bufferList = new LIST_BUFFER[bufferMax];
		for (int i = 0; i < bufferMax; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(creditSEPathList[i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);
			SEManager::bufferList[i].soundId = creditSEPathList[i];
			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		break;

	case SceneList::GAME:
		bufferMax = sizeof(gameSEPathList) / sizeof(int);
		SEManager::bufferList = new LIST_BUFFER[bufferMax];
		for (int i = 0; i < bufferMax; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(gameSEPathList[i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);
			SEManager::bufferList[i].soundId = gameSEPathList[i];
			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		break;

	case SceneList::RESULT:	
		bufferMax = sizeof(resultSEPathList) / sizeof(int);
		SEManager::bufferList = new LIST_BUFFER[bufferMax];
		for (int i = 0; i < bufferMax; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(resultSEPathList[i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);
			SEManager::bufferList[i].soundId = resultSEPathList[i];
			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		break;

	case SceneList::CREATE:
		bufferMax = 0;
		break;

	case SceneList::NONE_SCENE:
		bufferMax = 0;
		break;

	default:
		break;
	}
#endif
}