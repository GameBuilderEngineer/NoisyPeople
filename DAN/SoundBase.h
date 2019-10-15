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

typedef struct	// �o�b�t�@�\����
{
	int				soundId;
	XAUDIO2_BUFFER	buffer;
	WAV_FILE			wavFile;
}LIST_BUFFER;

typedef struct //�Đ��p�����[�^
{
	int								endpointVoiceId;		//�G���h�|�C���g�{�C�XID
	int								soundId;				//�T�E���hID
	bool								loop;				//���[�v
	float							speed;				//�Đ����x
	bool								filterFlag;			//�t�B���^�[��
	XAUDIO2_FILTER_PARAMETERS		filterParameters;	//�t�B���^�[��
}PLAY_PARAMETERS;

typedef struct //�Ȃ̃p�����[�^
{
	IXAudio2SourceVoice *SourceVoice;	//�\�[�X�{�C�X
	PLAY_PARAMETERS		playParameters;	//�Đ��p�����[�^
	bool					isSpeed;			//�Đ����x�ύX����?
	bool					isPlaying;		//�Đ���?
	long					stopPoint;		//��~�ʒu
}SOUND_PARAMETERS;

//===================================================================================================================================
//�y�T�E���h(XAudio2)�z
//�T�E���h�̃x�[�X�N���X
//===================================================================================================================================
class SoundBase
{
public:
	SoundBase();
	~SoundBase();

	void	 playSound(const PLAY_PARAMETERS playParameters);	//�Đ�
	void	 stopSound(const PLAY_PARAMETERS playParameters);	//��~
	void	 updateSound(void);										//�X�V����

protected:
	static WAV_FILE	LoadWavChunk(FILE *fp);								//WAV�t�@�C���̓ǂݍ��ݏ���
	void				MakeSourceVoice(const PLAY_PARAMETERS playParameters, LIST_BUFFER *listBuffer);
	void				uninitSoundStop(void);								//��~(�S���̃T�E���h)
	LIST_BUFFER		*GetBuffer(int endpointVoiceId, int soundId, bool loop);

	//�o�b�t�@���X�g
	static LIST_BUFFER *SEBufferList;
	static int			SEBufferMax;
	static LIST_BUFFER *BGMBufferList;
	static int			BGMBufferMax;

	//�{�C�X���X�g
	LinkedList <SOUND_PARAMETERS>*soundParametersList;
};