//===================================================================================================================================
//�ySoundBase.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/04
// [�X�V��]2019/10/04
//===================================================================================================================================
#include "SoundBase.h"
#include "Sound.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
LIST_BUFFER *SoundBase::bufferList = nullptr;
int	SoundBase::bufferMax = 0;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
SoundBase::SoundBase()
{
	//���X�g�̍쐬
	soundParametersList = new LinkedList <SOUND_PARAMETERS>;
	soundParametersList->insertFront();
	soundParametersList->listUpdate();
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
SoundBase::~SoundBase()
{
	uninitSoundStop();
	soundParametersList->terminate();
	SAFE_DELETE(soundParametersList);
}
//===================================================================================================================================
//�y��~(�S���̃T�E���h)�z
//===================================================================================================================================
void	 SoundBase::uninitSoundStop(void)
{
	SOUND_PARAMETERS *tmpSoundParameters = nullptr;

	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		tmpSoundParameters = soundParametersList->getValue(i);

		if (tmpSoundParameters->isPlaying)		//�Đ����Ă���
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			tmpSoundParameters->stopPoint = (short)voiceState.SamplesPlayed;

			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice)
		}
	}
}

//===================================================================================================================================
//�y�Đ��z
//===================================================================================================================================
void SoundBase::playSound(int soundId, bool loop)
{
	SOUND_PARAMETERS *tmpSoundParameters = nullptr;

	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		tmpSoundParameters = soundParametersList->getValue(i);

		if ((!tmpSoundParameters->isPlaying) &&				//�Đ����Ă��Ȃ�
			(tmpSoundParameters->soundId == soundId))		//ID����v����
		{
			tmpSoundParameters->SourceVoice->Start();
			tmpSoundParameters->isPlaying = true;
			return;
		}
	}

	//���X�g���ɑ��݂��Ȃ��A�Đ����Ă��Ȃ��{�C�X���Ȃ����͐V�����ǉ�����
	MakeSourceVoice(tmpSoundParameters, GetBuffer(soundId, loop));
}

//===================================================================================================================================
//�y��~�z
//===================================================================================================================================
void	 SoundBase::stopSound(int soundId, bool loop)
{
	SOUND_PARAMETERS *tmpSoundParameters = nullptr;

	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		tmpSoundParameters = soundParametersList->getValue(i);

		if ((tmpSoundParameters->isPlaying) &&				//�Đ����Ă���
			(tmpSoundParameters->soundId == soundId))		//ID����v����
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			tmpSoundParameters->stopPoint = (short)voiceState.SamplesPlayed;

			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			return;
		}
	}
}

//===================================================================================================================================
//�y�X�V�����z
//===================================================================================================================================
void	 SoundBase::updateSound(void)
{
	//�Đ���Ԃ��`�F�b�N
	for (int i = 0; i < soundParametersList->nodeNum; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if (tmpSoundParameters != NULL)
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			//�Đ����I�����
			if (voiceState.BuffersQueued == 0)
			{
				SAFE_DELETE(tmpSoundParameters);
				soundParametersList->listUpdate();
			}
		}
	}
}

//===================================================================================================================================
//�y�T�E���h�o�b�t�@���擾�z
//===================================================================================================================================
LIST_BUFFER *SoundBase::GetBuffer(int soundId, bool loop)
{
	if (loop)
	{
		bufferList[soundId].buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	return &bufferList[soundId];
}

//===================================================================================================================================
//�y�\�[�X�{�C�X�̍쐬�z
//===================================================================================================================================
void SoundBase::MakeSourceVoice(SOUND_PARAMETERS *soundParameters, LIST_BUFFER *listBuffer)
{
	//�g�`�̃t�H�[�}�b�g
	WAVEFORMATEX pcm;
	memset(&pcm, 0, sizeof(pcm));

	//�Đ��̃p�����[�^
	SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(soundParametersList->nodeNum - 1);
	tmpSoundParameters->stopPoint = soundParameters->stopPoint;
	tmpSoundParameters->soundId = soundParameters->soundId;
	tmpSoundParameters->loop = soundParameters->loop;
	tmpSoundParameters->isPlaying = false;

	//�g�`�̃t�H�[�}�b�g
	pcm.cbSize = 0;
	pcm.nChannels = listBuffer->wavFile.fmt.fmtChannel;
	pcm.wBitsPerSample = listBuffer->wavFile.fmt.fmtBitPerSample;
	pcm.nSamplesPerSec = listBuffer->wavFile.fmt.fmtSampleRate;
	pcm.wFormatTag = WAVE_FORMAT_PCM;
	pcm.nBlockAlign = (listBuffer->wavFile.fmt.fmtChannel*listBuffer->wavFile.fmt.fmtBitPerSample) / 8;
	pcm.nAvgBytesPerSec = listBuffer->wavFile.fmt.fmtSampleRate*pcm.nBlockAlign;

	//�\�[�X�{�C�X�̍쐬
	SoundInterface::GetXAudio2Interface()->CreateSourceVoice(
		&tmpSoundParameters->SourceVoice,
		&pcm,
		XAUDIO2_VOICE_USEFILTER,
		XAUDIO2_MAX_FREQ_RATIO,
		NULL,
		&SoundInterface::GetSendList(ENDPOINT_VOICE_LIST::ENDPOINT_SE),
		NULL);

	//�o�b�t�@�̒�o
	tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&listBuffer->buffer);

	//�m�[�h�̍X�V����
	soundParametersList->listUpdate();

	//�Đ�
	playSound(tmpSoundParameters->soundId, tmpSoundParameters->loop);
}

//===================================================================================================================================
//�yWAV�t�@�C���̓ǂݍ��ݏ����z
//===================================================================================================================================
WAV_FILE SoundBase::LoadWavChunk(FILE *fp)
{
	WAV_FILE tmpWavFile;	// WAV�t�@�C���\����
	memset(&tmpWavFile, 0, sizeof(tmpWavFile));

	// riff�̓ǂݍ���
	fread(&tmpWavFile.riff, sizeof(RIFF_CHUNK), 1, fp);

	int	chunkFlag = NULL;	// �`�����N��T�����[�v�̃t���O

	// fmt_��data�̓ǂݍ���
	while (chunkFlag != FLAG_CHUNK_END)
	{
		char	 chunk[CHUNK_ID_SIZE];
		long	 size = 0;

		// �`�����N�ƃT�C�Y�̓ǂݍ���
		fread(&chunk, sizeof(chunk), 1, fp);
		fread(&size, sizeof(size), 1, fp);

		// fmt_�`�����N
		if (memcmp(chunk, CHUNK_FMT, CHUNK_ID_SIZE) == 0)
		{
			// �`�����N�ƃT�C�Y��ݒ�
			memcpy(tmpWavFile.fmt.fmtChunk, chunk, CHUNK_ID_SIZE);
			tmpWavFile.fmt.fmtSize = size;

			// �t�H�[�}�b�gID����ǂݍ���
			fread(&tmpWavFile.fmt.fmtFormatTag,
				sizeof(FMT_CHUNK) - (sizeof(tmpWavFile.fmt.fmtChunk) + sizeof(tmpWavFile.fmt.fmtSize)), 1, fp);

			// �t���O����
			chunkFlag += FLAG_CHUNK_FMT;

			continue;
		}

		// data�`�����N
		if (memcmp(chunk, CHUNK_DATA, CHUNK_ID_SIZE) == 0)
		{
			// �`�����N�ƃT�C�Y��ݒ�
			memcpy(tmpWavFile.data.dataChunk, CHUNK_DATA, CHUNK_ID_SIZE);
			tmpWavFile.data.waveSize = size;

			// �f�[�^�̓ǂݍ���
			tmpWavFile.data.waveData = (short *)malloc(tmpWavFile.data.waveSize);
			fread(tmpWavFile.data.waveData, tmpWavFile.data.waveSize, 1, fp);

			// �t���O����
			chunkFlag += FLAG_CHUNK_DATA;

			continue;
		}

		// ���̃`�����N
		else
		{
			// ����Ȃ������X�L�b�v!
			fseek(fp, size, SEEK_CUR);
		}
	}

	fclose(fp);

	return tmpWavFile;
}
