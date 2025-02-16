////===================================================================================================================================
////yWaveBall.hz
//// [์ฌา]HALGP12A332 16 ไ๏ F
//// [์ฌ๚]2019/11/15
//// [XV๚]2019/11/15
////===================================================================================================================================
//#pragma once
//
////===================================================================================================================================
////yCN[hz
////===================================================================================================================================
//#include "Base.h"
//#include "Sprite.h"
//#include "Sound.h"
//
////===================================================================================================================================
////y่z
////===================================================================================================================================
//#define LINE_NUM_VERTEX (2)
//// Qc|Sธ_tH[}bg( ธ_ภW[2D] / ฝห๕)
//#define	FVF_VERTEX_LINE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
//
//#define ON_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
//#define OFF_COL_IN			D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))
//#define MOUSE_IN_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,255))
//#define MOUSE_OUT_COL_IN		D3DCOLOR(D3DCOLOR_RGBA(255,0,0,50))
//
////===================================================================================================================================
////g`{[NX
////===================================================================================================================================
//class WaveBall
//{
//public:
//	WaveBall();
//	~WaveBall();
//
//	void draw(void);
//	void setMouseInCol(bool flag) 
//	{
//#if(XADUIO2_STATE)
//		if ((flag) && (!switchFlag))diffuse = MOUSE_IN_COL_IN;
//		else diffuse = MOUSE_OUT_COL_IN;
//#endif
//	}
//	void setOnCol(bool flag)
//	{
//#if(XADUIO2_STATE)
//		if (flag)diffuse = ON_COL_IN;
//		else diffuse = OFF_COL_IN;
//		switchFlag = flag;
//#endif
//	}
//	void setVolume(float inVolume)
//	{
//#if(XADUIO2_STATE)
//		volume = inVolume;
//		SoundInterface::SE->setEndPointVoiceVolume(volume);
//		SoundInterface::BGM->setEndPointVoiceVolume(volume);
//		SoundInterface::S3D->setEndPointVoiceVolume(volume);
//		if (inVolume == NULL) { setOnCol(false); }
//		else setOnCol(true);
//#endif
//	}
//
//	static int dataMax;
//	static float *fData;
//
//	static void SetWaveData(int inDataMax, float *inFdata)
//	{
//#if(XADUIO2_STATE)
//		SAFE_DELETE_ARRAY(fData);
//		dataMax = inDataMax - 1;
//		fData = new float[dataMax];
//		memset(fData, 0, dataMax * sizeof(float));
//		memcpy(fData, inFdata, dataMax * sizeof(float));
//#endif
//	};
//
//private:
//
//	bool switchFlag;
//	float volume;
//	LPD3DXLINE pLine;
//	D3DCOLOR	 diffuse;		// ฝห๕
//};