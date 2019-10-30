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
#if(_MSC_VER >= GAME_MSC_VER)
LIST_BUFFER *SoundBase::SEBufferList = nullptr;
LIST_BUFFER *SoundBase::BGMBufferList = nullptr;
int	SoundBase::SEBufferMax = 0;
int	SoundBase::BGMBufferMax = 0;
#endif
//===================================================================================================================================
//�y�R���X�g���N�^�z
//�E�m�[�h�̏���
//===================================================================================================================================
SoundBase::SoundBase()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//���X�g�̍쐬
	soundParametersList = new LinkedList <SOUND_PARAMETERS>;
	soundParametersList->insertFront();
	soundParametersList->listUpdate();
#endif
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
SoundBase::~SoundBase()
{
#if(_MSC_VER >= GAME_MSC_VER)
	uninitSoundStop();
	soundParametersList->terminate();
	SAFE_DELETE(soundParametersList);
#endif
}
//===================================================================================================================================
//�y��~(�S���̃T�E���h)�z
//===================================================================================================================================
void SoundBase::uninitSoundStop(void)
{
#if(_MSC_VER >= GAME_MSC_VER)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
		
		if (tmpSoundParameters->isPlaying)		//�Đ����Ă���
		{
			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
		}
	}
#endif
}

//===================================================================================================================================
//�y�Đ��z
//===================================================================================================================================
void SoundBase::playSound(const PLAY_PARAMETERS playParameters)
{
#if(_MSC_VER >= GAME_MSC_VER)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if ((!tmpSoundParameters->isPlaying) &&				//�Đ����Ă��Ȃ�
			(tmpSoundParameters->playParameters.soundId == playParameters.soundId) &&		//ID����v����
			(tmpSoundParameters->playParameters.loop == playParameters.loop))				//���[�v������v����
		{
			tmpSoundParameters->SourceVoice->Start();
			if (tmpSoundParameters->playParameters.filterFlag)
			{
				tmpSoundParameters->SourceVoice->SetFilterParameters(&tmpSoundParameters->playParameters.filterParameters.Parameters);
			}
			tmpSoundParameters->isPlaying = true;
			return;
		}
	}

	//���X�g���ɑ��݂��Ȃ��A�Đ����Ă��Ȃ��{�C�X���Ȃ����͐V�����ǉ�����
	MakeSourceVoice(playParameters,
		GetBuffer(playParameters.endpointVoiceId, playParameters.soundId, playParameters.loop));
#endif
}

//===================================================================================================================================
//�y��~�z
//===================================================================================================================================
void SoundBase::stopSound(const PLAY_PARAMETERS playParameters)
{
#if(_MSC_VER >= GAME_MSC_VER)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if ((tmpSoundParameters->isPlaying) &&							//�Đ����Ă��Ȃ�
			(tmpSoundParameters->playParameters.soundId == playParameters.soundId) &&					//ID
			(tmpSoundParameters->playParameters.loop == playParameters.loop)&&							//���[�v���
			(tmpSoundParameters->playParameters.endpointVoiceId == playParameters.endpointVoiceId))	//�G���h�|�C���g�{�C�XID
		{			
			tmpSoundParameters->SourceVoice->Stop();
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
			soundParametersList->remove(soundParametersList->getNode(i));
			soundParametersList->listUpdate();
			return;
		}
	}
#endif
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void SoundBase::updateSound(void)
{
#if(_MSC_VER >= GAME_MSC_VER)
	//�Đ���Ԃ��`�F�b�N
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		if ((tmpSoundParameters != NULL) && (!tmpSoundParameters->playParameters.loop))
		{
			XAUDIO2_VOICE_STATE voiceState;
			tmpSoundParameters->SourceVoice->GetState(&voiceState);

			//�Đ����I�����
			if (voiceState.BuffersQueued == 0)
			{
				SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
				soundParametersList->remove(soundParametersList->getNode(i));
				soundParametersList->listUpdate();
			}
		}
	}
#endif
}

#if(_MSC_VER >= GAME_MSC_VER)
//===================================================================================================================================
//�y�T�E���h�o�b�t�@���擾�z
//===================================================================================================================================
LIST_BUFFER *SoundBase::GetBuffer(int endpointVoiceId, int soundId, bool loop)
{
	if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{
		if (loop)
		{
			BGMBufferList[soundId].buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		return &BGMBufferList[soundId];
	}
	else if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		if (loop)
		{
			SEBufferList[soundId].buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		return &SEBufferList[soundId];
	}
	return nullptr;
}

//===================================================================================================================================
//�y�\�[�X�{�C�X�̍쐬�z
//===================================================================================================================================
void SoundBase::MakeSourceVoice(const PLAY_PARAMETERS playParameters, LIST_BUFFER *listBuffer)
{
	//�g�`�̃t�H�[�}�b�g
	WAVEFORMATEX pcm;
	memset(&pcm, 0, sizeof(pcm));

	//�Đ��̃p�����[�^
	SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(soundParametersList->nodeNum - 1);
	tmpSoundParameters->playParameters = playParameters;
	tmpSoundParameters->stopPoint = 0;
	tmpSoundParameters->isPlaying = false;
	tmpSoundParameters->isSpeed = false;

	//�Đ����x�̕ύX
	if (tmpSoundParameters->playParameters.speed != NULL)
	{
		//���true�ɂ���
		tmpSoundParameters->isSpeed = false;
	}

	//�g�`�̃t�H�[�}�b�g
	pcm.cbSize = 0;
	pcm.nChannels = listBuffer->wavFile.fmt.fmtChannel;
	pcm.wBitsPerSample = listBuffer->wavFile.fmt.fmtBitPerSample;
	pcm.nSamplesPerSec = listBuffer->wavFile.fmt.fmtSampleRate;
	pcm.wFormatTag = WAVE_FORMAT_PCM;
	pcm.nBlockAlign = (listBuffer->wavFile.fmt.fmtChannel*listBuffer->wavFile.fmt.fmtBitPerSample) / 8;
	pcm.nAvgBytesPerSec = listBuffer->wavFile.fmt.fmtSampleRate*pcm.nBlockAlign;

	//�\�[�X�{�C�X�̍쐬
	if (playParameters.endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{
		SoundInterface::GetXAudio2Interface()->CreateSourceVoice(
			&tmpSoundParameters->SourceVoice,
			&pcm,
			XAUDIO2_VOICE_USEFILTER,
			XAUDIO2_MAX_FREQ_RATIO,
			NULL,
			&SoundInterface::GetSendList(ENDPOINT_VOICE_LIST::ENDPOINT_BGM),
			NULL);
	}
	else if (playParameters.endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		SoundInterface::GetXAudio2Interface()->CreateSourceVoice(
			&tmpSoundParameters->SourceVoice,
			&pcm,
			XAUDIO2_VOICE_USEFILTER,
			XAUDIO2_MAX_FREQ_RATIO,
			NULL,
			&SoundInterface::GetSendList(ENDPOINT_VOICE_LIST::ENDPOINT_SE),
			NULL);
	}
	//�o�b�t�@�̒�o
	tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&listBuffer->buffer);

	//�m�[�h�̍X�V����
	soundParametersList->insertAfter(soundParametersList->getNode(soundParametersList->nodeNum - 1));
	soundParametersList->listUpdate();

	//�Đ�
	playSound(playParameters);
}
#endif

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
			tmpWavFile.data.waveData = new (short[tmpWavFile.data.waveSize / sizeof(short)]);
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

	return tmpWavFile;
}
