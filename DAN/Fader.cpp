//===================================================================================================================================
//�yFader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/11/01
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Fader.h"
#include "ShaderLoader.h"
#include "Direct3D9.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
Fader* pointerFader = NULL;

//===================================================================================================================================
//�y�O���Q�Ɓz
//===================================================================================================================================
Fader* getFader() {
	return pointerFader;
}

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Fader::Fader()
{
	pointerFader = this;

	device			= getDevice();
	inTimer			= 0.0f;
	outTimer		= 0.0f;
	waitTimer		= 0.0f;
	playTimer		= 0.0f;
	shaderRate		= 0.0f;

	shaderState		= NULL;
	targetTexture	= NULL;
	textureZBuffer	= NULL;
	surface			= NULL;

	//�e�N�X�`���I�u�W�F�N�g�̍쐬
	D3DXCreateTexture(getDevice(), WINDOW_WIDTH, WINDOW_HEIGHT, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &targetTexture);
	//�T�[�t�F�[�X�̎擾
	targetTexture->GetSurfaceLevel(0, &surface);

	//�e�N�X�`���������_�����O�^�[�Q�b�g����ۂ�Z�o�b�t�@���쐬
	D3DSURFACE_DESC desc;
	targetTexture->GetLevelDesc(0, &desc);
	getDevice()->CreateDepthStencilSurface(desc.Width, desc.Height,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &textureZBuffer, NULL);

	//�t�F�[�h�`�撸�_�\����
	point[0] = {-1,1,0,0,0 };
	point[1] = {1,1,0,1,0 };
	point[2] = {-1,-1,0,0,1 };
	point[3] = {1,-1,0,1,1 };

	processing = false;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Fader::~Fader()
{
	SAFE_RELEASE(surface);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Fader::update(float frameTime)
{
	this->frameTime = frameTime;
	switch (state)
	{
	case faderNS::FADE_IN:
		fadeIn();
		break;
	case faderNS::FADE_OUT:
		fadeOut();
		break;
	case faderNS::FADE_PLAY:
		play();
		break;
	case faderNS::FADE_WAIT:
		wait();
		break;
	}
}

//===================================================================================================================================
//�y�t�F�[�h�C�������z
//===================================================================================================================================
void Fader::fadeIn()
{
	inTimer += frameTime;
	shaderRate = inTimer / limitInTime;
	//�t�F�[�h�C���I��
	if (inTimer > limitInTime)
	{
		state		= faderNS::FADE_WAIT;//�ҋ@��Ԃ�
		shaderRate	= 1.0f;
		waitTimer	= 0.0f;
	}
}

//===================================================================================================================================
//�y�t�F�[�h�A�E�g�����z
//===================================================================================================================================
void Fader::fadeOut()
{
	outTimer += frameTime;
	shaderRate = 1.0f - (outTimer / limitInTime);
	//�t�F�[�h�A�E�g�I��
	if (outTimer > limitOutTime)
	{
		state		= faderNS::FADE_PLAY;//�Đ���Ԃ�
		shaderRate	= 0.0f;
		playTimer	= 0.0f;
	}
}

//===================================================================================================================================
//�y�Đ������z
//===================================================================================================================================
void Fader::play()
{
	playTimer += frameTime;
	shaderRate = 0.0f;
	if (playTimer > limitPlayTime)
	{
		state = faderNS::FADE_IN;//�t�F�[�h�C����Ԃ�
		shaderRate = 0.0f;
		inTimer = 0.0f;
	}
}

//===================================================================================================================================
//�y�ҋ@�����z
//===================================================================================================================================
void Fader::wait()
{
	waitTimer	+= frameTime;
	shaderRate	= 1.0f;
	processing	= false;
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Fader::render()
{
	if (shaderState == NULL)return;
	if (!processing)return;

	//getDirect3D9()->setRenderBackBuffer(0);
	//device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	D3DSURFACE_DESC desc;
	targetTexture->GetLevelDesc(0, &desc);

	shaderState->setValue(desc);
	shaderState->begin(0);
	shaderState->updateValue(shaderRate);

	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(FADE_VERTEX));

	shaderState->end();
}

//===================================================================================================================================
//�y�e�N�X�`���֕`��Ώۂ�؂�ւ���z
//===================================================================================================================================
void Fader::setRenderTexture()
{
	getDevice()->SetRenderTarget(0, surface);
	getDevice()->SetDepthStencilSurface(textureZBuffer);
}

//===================================================================================================================================
//�y�V�F�[�_�̐ݒ�؂�ւ��z
//===================================================================================================================================
void Fader::setShader(int shaderType)
{
	SAFE_DELETE(shaderState);
	switch (shaderType)
	{
	case faderNS::SHADER_TYPE::NORMAL:
		shaderState = new faderNS::NormalShader(targetTexture);
		break;
	case faderNS::SHADER_TYPE::BLUR:
		shaderState = new faderNS::BlurShader(4, targetTexture);
		break;
	}
}

//===================================================================================================================================
//�y�J�n�z
//===================================================================================================================================
void Fader::start()
{
	if (processing)return;//�������Ȃ珈�����Ȃ�
	processing		= true;
	state			= faderNS::FADE_OUT;
	outTimer		= 0.0f;
	limitOutTime	= shaderState->outTime;
	limitInTime		= shaderState->inTime;
	limitPlayTime	= shaderState->playTime;
	limitWaitTime	= shaderState->waitTime;
}

//===================================================================================================================================
//�y�Đ����z
//===================================================================================================================================
bool Fader::nowPlaying()
{
	return (outTimer > limitOutTime) && (playTimer < limitPlayTime);
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
bool Fader::nowProcessing()
{
	return processing;
}


//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
#ifdef _DEBUG
void Fader::outputGUI()
{
	switch (state)
	{
	case faderNS::FADE_IN:	ImGui::Text("state:FADE_IN");	break;
	case faderNS::FADE_OUT:	ImGui::Text("state:FADE_OUT");	break;
	case faderNS::FADE_PLAY:ImGui::Text("state:FADE_PLAY");	break;
	case faderNS::FADE_WAIT:ImGui::Text("state:FADE_WAIT");	break;
	}
	ImGui::Text("inTimer = %.02f/%.02f", inTimer,limitInTime);
	ImGui::Text("outTimer = %.02f/%.02f", outTimer,limitOutTime);
	ImGui::Text("playTimer = %.02f/%.02f", playTimer,limitPlayTime);
	ImGui::Text("waitTimer = %.02f/%.02f", waitTimer,limitWaitTime);

	//ImGui::SliderFloat("alpha", &alpha, 0, 255);												//���ߒl
	//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
	//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//���x
	//ImGui::Checkbox("onGravity", &onGravity);												//�d�͗L�����t���O

}
#endif // _DEBUG
