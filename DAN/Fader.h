//===================================================================================================================================
//�yFader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/11/01
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//�y�t�F�[�h�����T�v�z
//�@�t�F�[�h�A�E�g
//�A�t�F�[�h�A�j���[�V�����̍Đ�
//�B�t�F�[�h�C��
//�C���s���ߑҋ@
//Start(�C)-���߁��@���A���B���C-���߁��@�E�E�E�ȍ~���[�v
//===================================================================================================================================

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
		NORMAL,
		BLUR,
		MAX_NUM
	};

	enum STATE
	{
		FADE_IN,
		FADE_OUT,
		FADE_WAIT,
		FADE_PLAY,
		STATE_NUM
	};

	//���ۃV�F�[�_�[�C���^�[�t�F�[�X
	class ShaderState
	{
	public:
		LPD3DXEFFECT	effect;		//�V�F�[�_�[�G�t�F�N�g
		float			inTime;		//�t�F�[�h�C������
		float			outTime;	//�t�F�[�h�A�E�g����
		float			waitTime;	//�ҋ@����
		float			playTime;	//�Đ�����
	public:
		//�R���X�g���N�^
		ShaderState()
		{
			effect = NULL;
		}
		//�V�F�[�_�[�J�n
		void begin(UINT i){
			effect->Begin(NULL, 0);
			effect->BeginPass(i);
		}
		//�V�F�[�_�[�I��
		void end() {
			effect->EndPass();
			effect->End();
		};
		//�V�F�[�_�[�֒l���Z�b�g
		virtual void setValue(D3DSURFACE_DESC desc) = 0;
		//�V�F�[�_�[�̒l�̍X�V
		virtual void updateValue(float rate) = 0;
	};

	//�m�[�}���V�F�[�_�[
	class NormalShader:public ShaderState
	{
	public:
		LPDIRECT3DTEXTURE9	targetTexture;		//�^�[�Q�b�g�e�N�X�`��
		float				alpha;				//���l
		//�R���X�g���N�^
		NormalShader(LPDIRECT3DTEXTURE9 _targetTexture)
			:targetTexture(_targetTexture),ShaderState()
		{
			effect		= *shaderNS::reference(shaderNS::NORMAL_FADE);
			inTime		= 1.0f;
			outTime		= 1.0f;
			waitTime	= 1.0f;
			playTime	= 1.0f;
		}
		//�l�̐ݒ�
		virtual void setValue(D3DSURFACE_DESC desc) override
		{
			effect->SetTechnique("main");
			effect->SetFloat("TexWidth",	(float)desc.Width);
			effect->SetFloat("TexHeight",	(float)desc.Height);
		}
		//�l�̍X�V
		virtual void updateValue(float rate) override
		{
			effect->SetTexture("Tex", targetTexture);
			effect->SetFloat("rate", rate);
			effect->CommitChanges();
		}
	};
	//�u���[�V�F�[�_�[
	class BlurShader:public ShaderState
	{
	public:
		float				level;				//�u���[���x��
		LPDIRECT3DTEXTURE9	targetTexture;		//�^�[�Q�b�g�e�N�X�`��
		//�R���X�g���N�^
		BlurShader(float _level, LPDIRECT3DTEXTURE9 _targetTexture)
			:level(_level),targetTexture(_targetTexture),ShaderState()
		{
			effect		= *shaderNS::reference(shaderNS::BLUR);
			inTime		= 3.0f;
			outTime		= 1.0f;
			waitTime	= 3.0f;
			playTime	= 3.0f;
		}
		//�l�̐ݒ�
		virtual void setValue(D3DSURFACE_DESC desc) override
		{
			effect->SetTechnique("tecBlur");
			effect->SetFloat("TexWidth", (float)desc.Width);
			effect->SetFloat("TexHeight", (float)desc.Height);
			effect->SetFloat("Level", level);
		}
		//�l�̍X�V
		virtual void updateValue(float rate) override
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
private:
	LPDIRECT3DDEVICE9		device;			//�f�o�C�X�|�C���^�ۑ�
	LPDIRECT3DTEXTURE9		targetTexture;	//�`��Ώۃe�N�X�`��
	LPDIRECT3DSURFACE9		surface;		//�`��e�N�X�`���T�[�t�F�[�X
	LPDIRECT3DSURFACE9		textureZBuffer;	//���o�b�t�@�`��e�N�X�`��
	FADE_VERTEX				point[4 + 1];	//�e�N�X�`���\��t���|���S��
	faderNS::ShaderState*	shaderState;	//�V�F�[�_�[�؂�փN���X

	int						state;			//�t�F�[�_�[�̏��
	float					frameTime;		//�t���[�����Ԃ̕ۑ��ϐ�

	//�^�C�}�[
	float					shaderRate;		//�V�F�[�_�[���[�g
	float					inTimer;		//�t�F�[�h�C���^�C�}�[
	float					outTimer;		//�t�F�[�h�A�E�g�^�C�}�[
	float					waitTimer;		//�ҋ@�^�C�}�[
	float					playTimer;		//�Đ��^�C�}�[

	//�J�ڎ��Ԃ̍ő�l�E�E�E�s�x�ݒ肷��B
	float					limitInTime;	//�t�F�[�h�C������
	float					limitOutTime;	//�t�F�[�h�A�E�g����
	float					limitWaitTime;	//�ҋ@����
	float					limitPlayTime;	//�Đ�����

	bool					processing;		//�t�F�[�h������

public:
	Fader();								//�R���X�g���N�^
	~Fader();								//�f�X�g���N�^
	void update(float frameTime);			//�X�V����
	void render();							//�`�揈��
	void start();							//�t�F�[�h�J�n����

	//���상�\�b�h
	void setShader(int shaderType);			//�V�F�[�_�[�̐ݒ�
	void setRenderTexture();				//�����_�[�Ώۂ��t�F�[�h�p�e�N�X�`���֐؂�ւ���
	bool nowPlaying();						//�Đ������ǂ���
	bool nowProcessing();					//���������ǂ���

#ifdef _DEBUG
	bool showGUI;							//GUI�̉��t���O
	void outputGUI();						//GUI�o��
	bool* getShowGUI(){ return &showGUI; };	//GUI�̉��t���Ogetter
#endif // _DEBUG


private:
	//�����������\�b�h
	void fadeIn();							//�t�F�[�h�C������
	void play();							//�Đ�����
	void fadeOut();							//�t�F�[�h�A�E�g����
	void wait();							//�ҋ@����
};

//===================================================================================================================================
//�y�O���Q�Ɓz
//===================================================================================================================================
Fader* getFader();