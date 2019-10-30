//===================================================================================================================================
//�ySoundbase.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/04
// [�X�V��]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "LinkedList.h"

//===================================================================================================================================
//�y�}�N����`�z
//===================================================================================================================================
//�`�����N����
#define FLAG_CHUNK_FMT			(1)			//fmt�̃t���O
#define FLAG_CHUNK_DATA			(1<<1)		//data�̃t���O
#define FLAG_CHUNK_END			(3)			//end�|�C���g
#define CHUNK_ID_SIZE			(4)			//�`�����NID�̃o�C�g��(4 byte)
#define CHUNK_FMT				("fmt ")		//fmt�̃`�����N
#define CHUNK_DATA				("data")		//data�̃`�����N

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct	// RIFF�`�����N 
{
	char		riffChunk[CHUNK_ID_SIZE];
	long		riffSize;
	char		waveChunk[CHUNK_ID_SIZE];
}RIFF_CHUNK;

typedef struct	// fmt �`�����N 
{
	char		fmtChunk[CHUNK_ID_SIZE];
	long		fmtSize;
	short	fmtFormatTag;
	short	fmtChannel;
	long		fmtSampleRate;
	long		fmtAvgBytesPerSec;
	short	fmtBlockAlign;
	short	fmtBitPerSample;
}FMT_CHUNK;

typedef struct	// data�`�����N 
{
	char		dataChunk[CHUNK_ID_SIZE];
	long		waveSize;
	short	*waveData;
}DATA_CHUNK;

typedef struct	// WAV�t�@�C�� 
{
	RIFF_CHUNK	riff;
	FMT_CHUNK	fmt;
	DATA_CHUNK	data;
}WAV_FILE;


#if(_MSC_VER >= GAME_MSC_VER)
typedef struct	// �o�b�t�@�\����
{
	int				soundId;
	XAUDIO2_BUFFER	buffer;
	WAV_FILE			wavFile;
}LIST_BUFFER;
#endif

#if(_MSC_VER < GAME_MSC_VER)
typedef struct //�t�B���^�[(�k�\����)
{
	int Type;
	float Frequency;
	float OneOverQ;
}
FAKE_FILTER_PARAMETERS;
#endif

typedef struct
{
#if(_MSC_VER >= GAME_MSC_VER)
	XAUDIO2_FILTER_PARAMETERS Parameters;
#else
	FAKE_FILTER_PARAMETERS Parameters;
#endif
}FILTER_PARAMETERS;

typedef enum //�t�B���^�[�̎��
{
	SAI_LowPassFilter,
	SAI_BandPassFilter,
	SAI_HighPassFilter,
	SAI_NotchFilter,
	SAI_LowPassOnePoleFilter,
	SAI_HighPassOnePoleFilter
}FILTER_TYPE;

typedef struct //�Đ��p�����[�^
{
	int						endpointVoiceId;		//�G���h�|�C���g�{�C�XID
	int						soundId;				//�T�E���hID
	bool						loop;				//���[�v
	float					speed;				//�Đ����x
	bool						filterFlag;			//�t�B���^�[��
	FILTER_PARAMETERS		filterParameters;	//�t�B���^�[��
}PLAY_PARAMETERS;

#if(_MSC_VER >= GAME_MSC_VER)
typedef struct //�Ȃ̃p�����[�^
{
	IXAudio2SourceVoice		*SourceVoice;	//�\�[�X�{�C�X
	PLAY_PARAMETERS			playParameters;	//�Đ��p�����[�^
	bool						isSpeed;			//�Đ����x�ύX����?
	bool						isPlaying;		//�Đ���?
	long						stopPoint;		//��~�ʒu
}SOUND_PARAMETERS;
#endif

//===================================================================================================================================
//�y�T�E���h(XAudio2)�z
//�T�E���h�̃x�[�X�N���X
//===================================================================================================================================
class SoundBase
{
public:
	SoundBase();
	~SoundBase();

	//�T�E���h�@�\
	void playSound(const PLAY_PARAMETERS playParameters);	//�Đ�
	void stopSound(const PLAY_PARAMETERS playParameters);	//��~
	void updateSound(void);									//�X�V

protected:
#if(_MSC_VER >= GAME_MSC_VER)
	//�G���h�|�C���g�{�C�X
	IXAudio2SubmixVoice			*EndpointVoice;				//XAudio2 Submix Vice(Endpoint Voice)
	XAUDIO2_SEND_DESCRIPTOR		SendDescriptor;				//XAudio2 Send Descriptor(BGM/SE Endpoint Voice)
	XAUDIO2_VOICE_SENDS			SendList;					//XAudio2 Send List(BGM/SE Endpoint Voice)

	//�o�b�t�@�Ǘ�
	LIST_BUFFER	*GetBuffer(int endpointVoiceId, int soundId, bool loop);
	void MakeSourceVoice(const PLAY_PARAMETERS playParameters, LIST_BUFFER *listBuffer);
	virtual void	 SwitchAudioBuffer(int scene) {};			//�X�e�[�W�J�ڂɍ��킹�ĕK�v�ȃT�E���h�o�b�t�@��p�ӂ���
#endif

	//��{�@�\(�ǂݍ��݁E��~)
	static WAV_FILE	LoadWavChunk(FILE *fp);					//WAV�t�@�C���̓ǂݍ��ݏ���
	void uninitSoundStop(void);								//��~(�S���̃T�E���h)

	//���X�g
#if(_MSC_VER >= GAME_MSC_VER)
	LinkedList <SOUND_PARAMETERS>*soundParametersList;		//�p���[���[�^���X�g
	LIST_BUFFER *bufferList;									//�o�b�t�@���X�g
	int			bufferMax;									//�o�b�t�@�̍ő吔
#endif

	//debug
#if _DEBUG
	virtual	void	 outputGUI(void) {};							//ImGUI�ւ̏o��
	int	scene;												//�V�[��
#endif

};