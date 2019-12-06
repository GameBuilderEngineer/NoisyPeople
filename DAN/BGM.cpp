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
#include "WaveBall.h"

//===================================================================================================================================
//�y�Đ����x�̐ݒ�z
//===================================================================================================================================
void BGMManager::SetSpeed(void)
{
#if(XADUIO2_STATE)

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
#endif
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
#ifdef _DEBUG
void BGMManager::outputGUI(void)
{
#if(XADUIO2_STATE)
	if (!ImGui::CollapsingHeader("BGMInformation"))
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

			//�Đ����Ă���
			if (tmpSoundParameters->isPlaying)
			{
				//�{�C�XID
				ImGui::Text("Voice ID:%d", tmpSoundParameters->playParameters.voiceID);

				//���x�̃o�b�N�A�b�v
				float backUpSpeed = tmpSoundParameters->playParameters.speed;

				//�T�E���h��
				ImGui::Text("%s", BGM_PATH_LIST_TAIL(tmpSoundParameters->playParameters.soundId));

				//�g�`�̕`��
				int saveDataMax = 2756;	//�擾����f�[�^��
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

				//�e�X�g�p@sai
				WaveBall::SetWaveData(dataMax, fData);

				ImVec2 plotextent(ImGui::GetContentRegionAvailWidth(), 100);
				ImGui::PlotLines("", fData, dataMax, 0, "Sound wave", FLT_MAX, FLT_MAX, plotextent);
				SAFE_DELETE_ARRAY(fData);

				//�Đ��ʒu
				ImGui::ProgressBar(playPoint / (float)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel));

				//���x����
				ImGui::Text("%d", ((int)state.SamplesPlayed / tmpBuffer->wavFile.fmt.fmtSampleRate));
				ImGui::SliderFloat("Speed", &tmpSoundParameters->playParameters.speed, 0.1f, 3.0f);

				if (backUpSpeed != tmpSoundParameters->playParameters.speed)
				{
					tmpSoundParameters->isSpeed = true;
				}
				if (tmpSoundParameters->isSpeed)
				{
					SetSpeed();
				}
			}
		}
	}
#endif
}
#endif

//===================================================================================================================================
//�y�K�v�ȃT�E���h�o�b�t�@��p�ӂ���z
//===================================================================================================================================
void BGMManager::loadBuffer(void)
{
#if(XADUIO2_STATE)
	//�T�E���h�f�B���N�g���ɐݒ肷��
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_BGM);

	bufferMax = BGM_LIST::BGM_MAX;
	BGMManager::bufferList = new LIST_BUFFER[bufferMax];
	for (int i = 0; i < bufferMax; i++)
	{
		BGMManager::bufferList[i].buffer = { 0 };

		FILE *fp = nullptr;
		fp = fopen(BGM_PATH_LIST_TAIL(i), "rb");
		BGMManager::bufferList[i].wavFile = LoadWavChunk(fp);
		BGMManager::bufferList[i].soundId = i;
		BGMManager::bufferList[i].buffer.pAudioData = (BYTE*)BGMManager::bufferList[i].wavFile.data.waveData;
		BGMManager::bufferList[i].buffer.AudioBytes = BGMManager::bufferList[i].wavFile.data.waveSize;
		BGMManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

		fclose(fp);
	}
#endif
}
