//===================================================================================================================================
//�yFader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/11/01
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "ShaderLoader.h"

//===================================================================================================================================
//�y�t�F�[�h���_�\���́z
//===================================================================================================================================
struct FADE_VERTEX
{
	float x, y, z;
	float u, v;
};

//===================================================================================================================================
//�y�t�F�[�_�[���O��ԁz
//===================================================================================================================================
namespace faderNS
{
	enum SHADER_TYPE
	{
		BLUR,
		MAX_NUM
	};

	//���ۃV�F�[�_�[�C���^�[�t�F�[�X
	class ShaderState
	{
	public:
		LPD3DXEFFECT effect;
		float outTime;
		float inTime;
		float stopTime;
	public:
		ShaderState()
		{
			effect = NULL;
		}


		void begin(UINT i){
			effect->Begin(NULL, 0);
			effect->BeginPass(i);
		}

		void end() {
			effect->EndPass();
			effect->End();
		};

		virtual void setValue(D3DSURFACE_DESC desc) = 0;
		virtual void updateValue() = 0;
	};

	//�u���[�V�F�[�_�[
	class BlurShader:public ShaderState
	{
	public:
		float level;
		LPDIRECT3DTEXTURE9 targetTexture;
		BlurShader(float _level, LPDIRECT3DTEXTURE9 _targetTexture)
			:level(_level),targetTexture(_targetTexture),ShaderState()
		{
			effect = *shaderNS::reference(shaderNS::BLUR);
			inTime = 3.0f;
			outTime = 1.0f;
			stopTime = 3.0f;
		}

		virtual void setValue(D3DSURFACE_DESC desc) override
		{
			effect->SetTechnique("tecBlur");
			effect->SetFloat("TexWidth", desc.Width);
			effect->SetFloat("TexHeight", desc.Height);
			effect->SetFloat("Level", level);
		}

		virtual void updateValue() override
		{
			effect->SetTexture("BlurTex", targetTexture);
			effect->CommitChanges();
		}
	};
}

//===================================================================================================================================
//�y�t�F�[�_�[�N���X�z
//===================================================================================================================================
class Fader :public Base
{
public:
	bool playing;
	LPDIRECT3DTEXTURE9 targetTexture;
	LPDIRECT3DSURFACE9 textureZBuffer;
	FADE_VERTEX point[4 + 1];
	LPDIRECT3DDEVICE9 device;

	float inTimer;
	float outTimer;
	float stopTimer;

	faderNS::ShaderState* shaderState;

public:
	Fader();
	~Fader();

	void setShader(int shaderType);
	void render();
	void play();
	void stop();
	void setRenderTexture();
	
};

//===================================================================================================================================
//�y�O���Q�Ɓz
//===================================================================================================================================
Fader* getFader();