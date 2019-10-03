//===================================================================================================================================
//�yMemoryViewer.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Sound.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
SOUND::SOUND()
{
	//COM���C�u�����̏�����
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return;
	}

	//XAudio2�I�u�W�F�N�g�̍쐬
	if (FAILED(XAudio2Create(&xAudio2)))
	{
		return ;
	}

	//�}�X�^�[�{�C�X�̍쐬
	if (FAILED(xAudio2->CreateMasteringVoice(&masteringVoice)))
	{
		return;
	}

	//�f�o�b�N�c�[��
	XAUDIO2_DEBUG_CONFIGURATION debugConfig{ NULL };
	debugConfig.LogThreadID = true;
	debugConfig.TraceMask = XAUDIO2_LOG_WARNINGS;
	debugConfig.TraceMask = debugConfig.TraceMask | XAUDIO2_LOG_FUNC_CALLS | XAUDIO2_LOG_DETAIL;
	debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
	xAudio2->SetDebugConfiguration(&debugConfig);

	//3D�T�E���h�̏�����
	DWORD channelMask;
	masteringVoice->GetChannelMask(&channelMask);
	if (FAILED(X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, x3DAudio)))
	{
		return;
	}
	memset(&emitter, 0, sizeof(X3DAUDIO_EMITTER));
	memset(&listener, 0, sizeof(X3DAUDIO_LISTENER));

	emitter.ChannelCount = INPUT_CHANNEL;					//�`�����l����
	emitter.CurveDistanceScaler = 1.0f;						//�{�����[���̃��x�������J�[�u
	emitter.pChannelAzimuths = emitterAzimuths;

	emitter.OrientFront = X3DAUDIO_VECTOR(0.0f,0.0f,1.0f);	//�O�����̌���
	emitter.OrientTop = X3DAUDIO_VECTOR(0.0f,1.0f,0.0f);	//������̌���
	emitter.Position = X3DAUDIO_VECTOR(0.0f,457.0f,0.0f);	//���̔�����(�ʒu)
	emitter.Velocity = X3DAUDIO_VECTOR(0.0f, 0.0f, 0.0f);	//���[���h�P��/�b�̃x���V�e�B

	emitter.InnerRadius = 2.0f;
	emitter.InnerRadiusAngle = X3DAUDIO_PI / 4.0f;;

	emitter.pCone = &emitterCone;
	emitter.pCone->InnerAngle = 0.0f;
	emitter.pCone->OuterAngle = 0.0f;
	emitter.pCone->InnerVolume = 0.0f;
	emitter.pCone->OuterVolume = 1.0f;
	emitter.pCone->InnerLPF = 0.0f;
	emitter.pCone->OuterLPF = 1.0f;
	emitter.pCone->InnerReverb = 0.0f;
	emitter.pCone->OuterReverb = 1.0f;

	const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
	const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

	emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	emitter.pLPFDirectCurve = nullptr;
	emitter.pLPFReverbCurve = nullptr;
	emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	emitter.CurveDistanceScaler = 14.0f;
	emitter.DopplerScaler = 1.0f;

	// �A�E�g�v�b�g�{�C�X�̍쐬
	masteringVoice->GetVoiceDetails(&details);

	xAudio2->CreateSubmixVoice(
		&submixVoice,				// �쐬����{�C�X(BGM)
		INPUT_CHANNEL,				// �`�����l����(IN)
		details.InputSampleRate,	// �S�̂̃T���v�����O���g��
		XAUDIO2_VOICE_USEFILTER);	// �@�\(�t�B���^)

	// ���M���X�g
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2] = {
		XAUDIO2_SEND_USEFILTER,masteringVoice,
		XAUDIO2_SEND_USEFILTER,submixVoice };
	const XAUDIO2_VOICE_SENDS sendlist = { 2, sendDescriptors };
	sourceVoice = setSourceVoice(sendlist);

	//�Đ�
	sourceVoice->Start();
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
SOUND::~SOUND()
{
	//���̒�~
	sourceVoice->Stop();
	
	//�\�[�X�{�C�X
	if (sourceVoice != NULL)
	{
		sourceVoice->DestroyVoice();
		sourceVoice = NULL;
	}

	//�T�u�~�b�N�X
	if (submixVoice != NULL)
	{
		submixVoice->DestroyVoice();
		submixVoice = NULL;
	}


	//�o�b�t�@�̉��
	free(wavFile.data.waveDataMain);

	//COM���C�u�����̏I������
	CoUninitialize();

	//�}�X�^�[�{�C�X
	if (masteringVoice != NULL)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = NULL;
	}

	//�C���^�[�t�F�[�X
	if (xAudio2 != NULL)
	{
		xAudio2->Release();
		xAudio2 = NULL;
	}
}

//===================================================================================================================================
//�y3D�T�E���h�̍X�V�����z
//===================================================================================================================================
void SOUND::updateSound(D3DXVECTOR3 pos, D3DXVECTOR3 front)
{
	const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI*5.0f / 6.0f, X3DAUDIO_PI*11.0f / 6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

	listener.OrientFront = (X3DAUDIO_VECTOR)front;			//���X�i�[�̌���
	listener.OrientTop = X3DAUDIO_VECTOR(0.0f, 1.0f, 0.0f);//������̌���
	listener.Position = (X3DAUDIO_VECTOR)pos;				//���X�i�[�̈ʒu
	listener.Velocity = X3DAUDIO_VECTOR(0.0f, 0.0f, 0.0f);	//���[���h�P��/�b�̃x���V�e�B
	listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;

	X3DAUDIO_DSP_SETTINGS DSPSettings = { 0 };
	FLOAT32 matrix[OUTPUT_CHANNEL];
	memset(matrix, 0, sizeof(matrix));
	DSPSettings.SrcChannelCount = INPUT_CHANNEL;
	DSPSettings.DstChannelCount = details.InputChannels;
	DSPSettings.pMatrixCoefficients = matrix;

	X3DAudioCalculate(x3DAudio, &listener, &emitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
		| X3DAUDIO_CALCULATE_LPF_DIRECT,&DSPSettings);

	sourceVoice->SetFrequencyRatio(DSPSettings.DopplerFactor);

	sourceVoice->SetOutputMatrix(masteringVoice, INPUT_CHANNEL, details.InputChannels,DSPSettings.pMatrixCoefficients);
	sourceVoice->SetOutputMatrix(submixVoice, INPUT_CHANNEL, 1, &DSPSettings.ReverbLevel);

	XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
	sourceVoice->SetOutputFilterParameters(masteringVoice, &FilterParametersDirect);
	XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFReverbCoefficient), 1.0f };
	sourceVoice->SetOutputFilterParameters(submixVoice, &FilterParametersReverb);

	updateSoundIMGUI();
}

//===================================================================================================================================
//�yIMGui�ւ̑��M�z
//===================================================================================================================================
void SOUND::updateSoundIMGUI(void)
{
#ifdef _DEBUG
	ImGui::Begin("PlayerInformation(Sound)");

	if (ImGui::CollapsingHeader("soundInformation"))
	{
		float limitTop = 1000;
		float limitBottom = -1000;

		//�T�E���h���
		if (ImGui::CollapsingHeader("soundState"))
		{
			float outputMatrix[OUTPUT_CHANNEL];
			memset(outputMatrix, 0, sizeof(outputMatrix));
			sourceVoice->GetOutputMatrix(masteringVoice, INPUT_CHANNEL, details.InputChannels, outputMatrix);
			for (int i = 0; i < OUTPUT_CHANNEL; i++)
			{
				ImGui::Text("source->master outputMatrix[%d]:%f", i, outputMatrix[i]);
			}

			ImGui::Text("--------------------------------------------------------");

			ImGui::Text("1.:LowPassFilter           2.:BandPassFilter");
			ImGui::Text("3.:HighPassFilter          4.:NotchFilter");
			ImGui::Text("5.:LowPassOnePoleFilter    6.:HighPassOnePoleFilter");

			ImGui::Text("--------------------------------------------------------");

			XAUDIO2_FILTER_PARAMETERS FilterParametersDirect;
			sourceVoice->GetOutputFilterParameters(masteringVoice, &FilterParametersDirect);
			XAUDIO2_FILTER_PARAMETERS FilterParametersReverb;
			sourceVoice->GetOutputFilterParameters(submixVoice, &FilterParametersReverb);

			ImGui::Text("filter parameters(Direct)    filter parameters(Reverb)");
			ImGui::Text("type:%d                       type:%d", FilterParametersDirect.Type, FilterParametersReverb.Type);
			ImGui::Text("oneOverQ:%.3f               oneOverQ:%.3f", FilterParametersDirect.OneOverQ, FilterParametersReverb.OneOverQ);
			ImGui::Text("frequency:%.3f              frequency:%.3f", FilterParametersDirect.Frequency, FilterParametersReverb.Frequency);
		}

		//�G�~�b�^�[
		if (ImGui::CollapsingHeader("emitter"))
		{
			if (ImGui::CollapsingHeader("soundCone"))
			{
				ImGui::SliderFloat3("position", (float *)&emitter.Position, limitBottom, limitTop);							//�ʒu

				if (ImGui::CollapsingHeader("soundCone"))
				{
					ImGui::SliderFloat("innerAngle", &emitter.pCone->InnerAngle, 0, X3DAUDIO_2PI);							//innerAngle
					ImGui::SliderFloat("outerAngle", &emitter.pCone->OuterAngle, emitter.pCone->InnerAngle, X3DAUDIO_2PI);	//outerAngle

					ImGui::SliderFloat("innerVolume", &emitter.pCone->InnerVolume, 0, 2.0f);								//innerVolume
					ImGui::SliderFloat("outerVolume", &emitter.pCone->OuterVolume, 0, 2.0f);								//outerVolume

					ImGui::SliderFloat("innerLPF", &emitter.pCone->InnerLPF, 0, 1.0f);										//innerLPF
					ImGui::SliderFloat("outerLPF", &emitter.pCone->OuterLPF, 0, 1.0f);										//outerLPF

					ImGui::SliderFloat("innerReverb", &emitter.pCone->InnerReverb, 0, 1.0f);								//innerReverb
					ImGui::SliderFloat("outerReverb", &emitter.pCone->OuterReverb, 0, 1.0f);								//outerReverb
				}
			}
		}

		//���X�i�[
		if (ImGui::CollapsingHeader("listener"))
		{
			ImGui::SliderFloat3("position", (float *)&listener.Position, limitBottom, limitTop);							//�ʒu

			if (ImGui::CollapsingHeader("soundCone"))
			{
				ImGui::SliderFloat("innerAngle", &listener.pCone->InnerAngle, 0, X3DAUDIO_2PI);								//innerAngle
				ImGui::SliderFloat("outerAngle", &listener.pCone->OuterAngle, listener.pCone->InnerAngle, X3DAUDIO_2PI);	//outerAngle

				ImGui::SliderFloat("innerVolume", &listener.pCone->InnerVolume, 0, 2.0f);									//innerVolume
				ImGui::SliderFloat("outerVolume", &listener.pCone->OuterVolume, 0, 2.0f);									//outerVolume

				ImGui::SliderFloat("innerLPF", &listener.pCone->InnerLPF, 0, 1.0f);											//innerLPF
				ImGui::SliderFloat("outerLPF", &listener.pCone->OuterLPF, 0, 1.0f);											//outerLPF

				ImGui::SliderFloat("innerReverb", &listener.pCone->InnerReverb, 0, 1.0f);									//innerReverb
				ImGui::SliderFloat("outerReverb", &listener.pCone->OuterReverb, 0, 1.0f);									//outerReverb
			}
		}


		//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
		//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
		//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//�X�P�[��
		//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//���a
		//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//���ߒl
		//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//���x
		//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
		//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//�d��

		//ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		//ImGui::Checkbox("onActive", &onActive);										//�A�N�e�B�u���t���O
		//ImGui::Checkbox("onRender", &onRender);										//�`��L�����t���O
		//ImGui::Checkbox("onLighting", &onLighting);									//���������t���O
		//ImGui::Checkbox("onTransparent", &onTransparent);								//���߃t���O
		//ImGui::Checkbox("operationAlpha", &operationAlpha);							//���ߒl�̑���L���t���O
		//ImGui::Checkbox("sound", &onSound);											//�T�E���h

		//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//���ߒl�̑���L���t���O
	}

	ImGui::End();

#endif
}

//===================================================================================================================================
//�y�\�[�X�{�C�X�̍쐬�z
//===================================================================================================================================
IXAudio2SourceVoice *SOUND::setSourceVoice(const XAUDIO2_VOICE_SENDS sendList)
{
	IXAudio2SourceVoice *tmpSourceVoice = NULL;
	
	WAVEFORMATEX		pcm;	// WAVEFORMATEX�\����
	memset(&pcm, 0, sizeof(WAVEFORMATEX));

	XAUDIO2_BUFFER		buffer;	// �o�b�t�@
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	//�t�@�C���I�[�v��
	setSoundDirectory();
	fp = fopen(SOUND_PATH_TEST, "rb");
	wavFile = LoadWavChunk(fp);

	pcm.cbSize = 0;
	pcm.nChannels = wavFile.fmt.fmtChannel;
	pcm.wBitsPerSample = wavFile.fmt.fmtBitPerSample;
	pcm.nSamplesPerSec = wavFile.fmt.fmtSampleRate;
	pcm.wFormatTag = WAVE_FORMAT_PCM;
	pcm.nBlockAlign = (wavFile.fmt.fmtChannel*wavFile.fmt.fmtBitPerSample) / 8;
	pcm.nAvgBytesPerSec = wavFile.fmt.fmtSampleRate*pcm.nBlockAlign;

	// �o�b�t�@�̐ݒ�
	buffer.pAudioData = (BYTE*)wavFile.data.waveDataMain;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = wavFile.data.waveSize;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	// �\�[�X�{�C�X�̍쐬
	if (FAILED(xAudio2->CreateSourceVoice(&tmpSourceVoice, &pcm, XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO,nullptr, &sendList)))
	{
		return NULL;
	}

	// �o�b�t�@���o
	if (FAILED(tmpSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		return NULL;
	}

	return tmpSourceVoice;
}

//=============================================================================
// WAV�t�@�C���̃`�����N����
// ����:FILE�\����(fopen�ς�) �߂�l:WAV�t�@�C���̍\����(WAV�̒��g)
// �g�p���@:
// �p�X�͂���Ȃ��Afopen�I������炱���ɓ���
//=============================================================================
WAV_FILE SOUND::LoadWavChunk(FILE *inFp)
{
	WAV_FILE tmpWavFile;	// WAV�t�@�C���\����
	memset(&tmpWavFile, 0, sizeof(tmpWavFile));

	// riff�̓ǂݍ���
	fread(&tmpWavFile.riff, sizeof(RIFF_CHUNK), 1, inFp);

	int	chunkFlag = NULL;	// �`�����N��T�����[�v�̃t���O

	// fmt_��data�̓ǂݍ���
	while (chunkFlag != FLAG_CHUNK_END)
	{
		char	chunk[CHUNK_ID_SIZE];
		long	size = 0;

		// �`�����N�ƃT�C�Y�̓ǂݍ���
		fread(&chunk, sizeof(chunk), 1, inFp);
		fread(&size, sizeof(size), 1, inFp);

		// fmt_�`�����N
		if (memcmp(chunk, CHUNK_FMT, CHUNK_ID_SIZE) == 0)
		{
			// �`�����N�ƃT�C�Y��ݒ�
			memcpy(tmpWavFile.fmt.fmtChunk, chunk, CHUNK_ID_SIZE);
			tmpWavFile.fmt.fmtSize = size;

			// �t�H�[�}�b�gID����ǂݍ���
			fread(&tmpWavFile.fmt.fmtFormatTag,
				sizeof(FMT_CHUNK) - (sizeof(tmpWavFile.fmt.fmtChunk) + sizeof(tmpWavFile.fmt.fmtSize)), 1, inFp);

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
			tmpWavFile.data.waveDataMain = (short *)malloc(tmpWavFile.data.waveSize);
			fread(tmpWavFile.data.waveDataMain, tmpWavFile.data.waveSize, 1, inFp);

			// �t���O����
			chunkFlag += FLAG_CHUNK_DATA;

			continue;
		}

		// ���̃`�����N
		else
		{
			// ����Ȃ������X�L�b�v!
			fseek(inFp, size, SEEK_CUR);
		}
	}

	fclose(inFp);

	return tmpWavFile;
}
