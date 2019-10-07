//===================================================================================================================================
//�ySound.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/26
// [�X�V��]2019/09/26
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y���C�u�����̃��[�h�z
//===================================================================================================================================
#include <xaudio2.h>
#include <x3daudio.h>

//===================================================================================================================================
//�y�K�v�ȃ��C�u�����t�@�C���̃��[�h�z
//===================================================================================================================================
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"x3daudio.lib")

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//�y�}�N����`�z
//===================================================================================================================================
//�`�����N��T�����[�v�p
#define FLAG_CHUNK_FMT		(1)			//fmt�̃t���O
#define FLAG_CHUNK_DATA		(1<<1)		//data�̃t���O
#define FLAG_CHUNK_END		(3)			//end�|�C���g

//�`�����N
#define CHUNK_ID_SIZE		(4)			//�`�����NID�̃o�C�g��(4 byte)
#define CHUNK_FMT			("fmt ")	//fmt�̃`�����N
#define CHUNK_DATA			("data")	//data�̃`�����N

//�T�E���h�̐���
#define SOUND_SAMPLE_RATE	(44100)		//���ʂ̃T���v�����O���g��
#define INPUT_CHANNEL		(2)			//���ʂ̃`�����l����(IN)
#define OUTPUT_CHANNEL		(8)			//���ʂ̃`�����l����(OUT)
#define SOUND_SE_MAX		(10)		//�����Đ��̍ő吔[SE]

//�{�C�X�̃{�C�X�������
#define SAFE_DESTROY_VOICE(p) if(p){ p->DestroyVoice(); p=NULL; }

//�e�X�g
#define SOUND_PATH_TEST		("BGM_Character_Select.wav")
//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct	// RIFF�`�����N 
{
	char	riffChunk[CHUNK_ID_SIZE];
	long	riffSize;
	char	waveChunk[CHUNK_ID_SIZE];
}RIFF_CHUNK;

typedef struct	// fmt �`�����N 
{
	char	fmtChunk[CHUNK_ID_SIZE];
	long	fmtSize;
	short	fmtFormatTag;
	short	fmtChannel;
	long	fmtSampleRate;
	long	fmtAvgBytesPerSec;
	short	fmtBlockAlign;
	short	fmtBitPerSample;
}FMT_CHUNK;

typedef struct	// data�`�����N 
{
	char	dataChunk[CHUNK_ID_SIZE];
	long	waveSize;
	short	*waveDataMain;
	//short	*waveDataSub;
}DATA_CHUNK;

typedef struct	// WAV�t�@�C�� 
{
	RIFF_CHUNK	riff;
	FMT_CHUNK	fmt;
	DATA_CHUNK	data;
}WAV_FILE;

//===================================================================================================================================
//�y�T�E���h�N���X(XAudio2)�z
//3D�T�E���h�̃e�X�g�p�N���X
//===================================================================================================================================
class SOUND
{
public:
	SOUND();
	~SOUND();

	void updateSound(D3DXVECTOR3 pos, D3DXVECTOR3 front);

private:
	IXAudio2MasteringVoice	*masteringVoice;	//�}�X�^�[�{�C�X
	IXAudio2				*xAudio2;			//XAudio2�̃C���^�[�t�F�C�X
	XAUDIO2_VOICE_DETAILS	details;			//�{�C�X���

	X3DAUDIO_HANDLE			x3DAudio;			//3DAudio�̃n���h��
	X3DAUDIO_LISTENER		listener;			//���X�i�[
	X3DAUDIO_EMITTER		emitter;			//�I�[�f�B�I�\�[�X
	X3DAUDIO_CONE			emitterCone;		//�I�[�f�B�I�\�[�X�̃R��
	FLOAT32					emitterAzimuths[INPUT_CHANNEL];

	IXAudio2SourceVoice		*sourceVoice;		//�\�[�X�{�C�X
	IXAudio2SubmixVoice		*submixVoice;		//�T�u�~�b�N�X�{�C�X

	FILE					*fp;				//�t�@�C��
	WAV_FILE				wavFile;			//���̃t�@�C��

	//�֐�
	WAV_FILE				LoadWavChunk(FILE *fp);
	IXAudio2SourceVoice		*setSourceVoice(const XAUDIO2_VOICE_SENDS sendList);
	void					updateSoundIMGUI(void);
};
