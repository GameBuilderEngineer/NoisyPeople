//===================================================================================================================================
//�ySoundBase.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/04
// [�X�V��]2019/10/04
//===================================================================================================================================
#include "SoundBase.h"
#include "Sound.h"
#include "AbstractScene.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//�E�m�[�h�̏���
//===================================================================================================================================
SoundBase::SoundBase()
{
#if(XADUIO2_STATE)
	//���X�g�̍쐬
	soundParametersList = new LinkedList <SOUND_PARAMETERS>;
	soundParametersList->insertFront();
	soundParametersList->listUpdate();
	voiceIDCnt = 0;

	//�G���h�|�C���g�{�C�X�̍쐬
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&EndpointVoice,										//�T�u�~�b�N�X�{�C�X
		ENDPOINT_INPUT_CHANNEL,								//�`�����l����(����)
		ENDPOINT_SAMPLE_RATE,								//�T���v�����O���[�g(����)
		XAUDIO2_VOICE_USEFILTER,								//�t�B���^�[�@�\
		1,													//�v���Z�X�X�e�[�W
		NULL)))												//���M���X�g(NULL:Mastering Voice�ւ̒P��̏o�͂ƂȂ�)
	{														//�G�t�F�N�g�`�F�[��
		return;
	}

	//�G���h�|�C���g�{�C�X�ւ̑��M���X�g�̍쐬
	SendDescriptor = { XAUDIO2_SEND_USEFILTER,EndpointVoice };
	SendList = { 1,&SendDescriptor };

#if _DEBUG
	//�V�[���̏�����
	scene = SceneList::SPLASH;
#endif
#endif
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
SoundBase::~SoundBase()
{
#if(XADUIO2_STATE)
	uninitSoundStop();
	soundParametersList->terminate();
	SAFE_DELETE(soundParametersList);

	SAFE_DESTROY_VOICE(EndpointVoice)
#endif
}
//===================================================================================================================================
//�y��~(�S���̃T�E���h)�z
//===================================================================================================================================
void SoundBase::uninitSoundStop(void)
{
#if(XADUIO2_STATE)
	int backup = soundParametersList->nodeNum - 1;
	for (int i = 0; i < backup; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
		
		if (tmpSoundParameters->isPlaying)		//�Đ����Ă���
		{
			tmpSoundParameters->SourceVoice->Stop();
			tmpSoundParameters->isPlaying = false;
			SAFE_DESTROY_VOICE(tmpSoundParameters->SourceVoice);
			soundParametersList->remove(soundParametersList->getNode(i));
		}
	}
	soundParametersList->listUpdate();
#endif
}

//===================================================================================================================================
//�y�Đ��z
//===================================================================================================================================
void SoundBase::playSound(PLAY_PARAMETERS *playParameters)
{
#if(XADUIO2_STATE)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		//if ((!tmpSoundParameters->isPlaying) &&											//�Đ����Ă��Ȃ�
		//	(tmpSoundParameters->playParameters.soundId == playParameters->soundId) &&	//ID����v����
		//	(tmpSoundParameters->playParameters.loop == playParameters->loop)&&			//���[�v������v����
		//	(tmpSoundParameters->playParameters.playerID == playParameters->playerID))	//�v���C���[ID
		if (tmpSoundParameters->playParameters.voiceID == playParameters->voiceID)
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
		GetBuffer(playParameters->endpointVoiceId, playParameters->soundId, playParameters->loop));
#endif
}

//===================================================================================================================================
//�y��~�z
//===================================================================================================================================
void SoundBase::stopSound(const PLAY_PARAMETERS playParameters)
{
#if(XADUIO2_STATE)
	for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
	{
		SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

		//if ((tmpSoundParameters->isPlaying) &&											//�Đ����Ă��Ȃ�
		//	(tmpSoundParameters->playParameters.soundId == playParameters.soundId) &&	//ID
		//	(tmpSoundParameters->playParameters.loop == playParameters.loop)&&			//���[�v���
		//	(tmpSoundParameters->playParameters.playerID == playParameters.playerID))	//�v���C���[ID
		if (tmpSoundParameters->playParameters.voiceID == playParameters.voiceID)
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
#if(XADUIO2_STATE)
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

#if(XADUIO2_STATE)
//===================================================================================================================================
//�y�T�E���h�o�b�t�@���擾�z
//===================================================================================================================================
LIST_BUFFER *SoundBase::GetBuffer(int endpointVoiceId, int soundId, bool loop)
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
void SoundBase::MakeSourceVoice(PLAY_PARAMETERS *playParameters, LIST_BUFFER *listBuffer)
{
	//�g�`�̃t�H�[�}�b�g
	WAVEFORMATEX pcm;
	memset(&pcm, 0, sizeof(pcm));

	//�Đ��̃p�����[�^
	SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(soundParametersList->nodeNum - 1);
	playParameters->voiceID = voiceIDCnt;
	tmpSoundParameters->playParameters = *playParameters;
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
	SoundInterface::GetXAudio2Interface()->CreateSourceVoice(
		&tmpSoundParameters->SourceVoice,
		&pcm,
		XAUDIO2_VOICE_USEFILTER,
		XAUDIO2_MAX_FREQ_RATIO,
		NULL,
		&SendList,
		NULL);

	//�o�b�t�@�̒�o
	tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&listBuffer->buffer);

	//�m�[�h�̍X�V����
	soundParametersList->insertAfter(soundParametersList->getNode(soundParametersList->nodeNum - 1));
	soundParametersList->listUpdate();

	//voiceID
	voiceIDCnt++;
	if (voiceIDCnt > LONG_MAX)
	{
		voiceIDCnt -= LONG_MAX;
	}

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
