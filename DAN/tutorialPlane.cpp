//===================================================================================================================================
//�ytutorialPlane.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/11/05
// [�X�V��]2019/11/05
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "tutorialPlane.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "TutorialUI.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace tutorialPlaneNS;

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�R���X�g���N�^�z
//===================================================================================================================================
PlaneInstance::PlaneInstance(D3DXVECTOR3 position) : InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	this->position = position;
	this->rotation = D3DXVECTOR3(0, 0, 0);
	this->scale = D3DXVECTOR2(5.0f, 4.0f);
	this->limitTime = 1.0f;
	this->uv.x = (1.0f / 4.0f) * 0.0f;
	this->uv.y = (1.0f / 2.0f) * 0.0f;

	this->tmpCnt = 0;
}

//===================================================================================================================================
//�y�C���X�^���X�N���X�F�X�V�z
//===================================================================================================================================
void PlaneInstance::update(float frameTime)
{
	if (lifeTimer >= limitTime)return;
}

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TutorialPlane::TutorialPlane(const D3DXVECTOR3 position) :InstancingBillboard::InstancingBillboard()
{
	//������
	planeStep = TUTORIAL_PLANE_ID::PLANE_ENEMY;

	InstancingBillboard::initialize(*textureNS::reference(textureNS::TUTORIAL_UI_PLANE), 3, 1);
	setRenderType(InstancingBillboardNS::FOREGROUND_PASS);

	generateInstance(new PlaneInstance(position));
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TutorialPlane::~TutorialPlane()
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void TutorialPlane::update(float frameTime, int inPlaneStep)
{
	InstancingBillboard::update(frameTime);

	switch (inPlaneStep)
	{
	case TUTORIAL_PLANE_ID::PLANE_ENEMY:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv = 
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 0.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_TREE:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 1.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_DIGITAL:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 2.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_SKY:
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 3.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 0.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_SHIFT_0:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 0.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 1.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_SHIFT_1:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 1.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 1.0f);
		InstancingBillboard::updateUV();
		break;
	case TUTORIAL_PLANE_ID::PLANE_CLEAR:
		//UV
		(*InstancingBillboard::getList().getValue(0))->uv =
			D3DXVECTOR2((1.0f / ARRAY_TEX_DIVIDE_X) * 2.0f, (1.0f / ARRAY_TEX_DIVIDE_Y) * 1.0f);
		InstancingBillboard::updateUV();
		break;
	default:
		break;
	}

	//�R�s�[
	planeStep = inPlaneStep;
}
