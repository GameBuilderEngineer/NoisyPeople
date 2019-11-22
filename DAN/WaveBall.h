//===================================================================================================================================
//�yWaveBall.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/11/15
// [�X�V��]2019/11/15
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "Sound.h"

//===================================================================================================================================
//�y�萔�z
//===================================================================================================================================
#define LINE_NUM_VERTEX (2)
// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���ˌ�)
#define	FVF_VERTEX_LINE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

#define ON_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
#define OFF_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))
#define MOUSE_IN_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
#define MOUSE_OUT_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))

//===================================================================================================================================
//�g�`�{�[���N���X
//===================================================================================================================================
class WaveBall
{
public:
	WaveBall();
	~WaveBall();

	void draw(void);
	void setMouseInCol(bool flag) 
	{
		if ((flag) && (!switchFlag))diffuse = MOUSE_IN_COL_IN;
		else diffuse = MOUSE_OUT_COL_IN;
	}
	void setOnCol(bool flag)
	{
		if (flag)diffuse = ON_COL_IN;
		else diffuse = OFF_COL_IN;
		switchFlag = flag;
	}
	void setVolume(float inVolume)
	{
		volume = inVolume;
		SoundInterface::SE->setEndPointVoiceVolume(volume);
		SoundInterface::BGM->setEndPointVoiceVolume(volume);
		SoundInterface::S3D->setEndPointVoiceVolume(volume);
		if (inVolume == NULL) { setOnCol(false); }
		else setOnCol(true);
	}

	static int dataMax;
	static float *fData;

	static void SetWaveData(int inDataMax, float *inFdata)
	{
		SAFE_DELETE_ARRAY(fData);
		dataMax = inDataMax - 1;
		fData = new float[dataMax];
		memset(fData, 0, dataMax * sizeof(float));
		memcpy(fData, inFdata, dataMax * sizeof(float));
	};

private:

	bool switchFlag;
	float volume;
	LPD3DXLINE pLine;
	D3DCOLOR	 diffuse;		// ���ˌ�
};