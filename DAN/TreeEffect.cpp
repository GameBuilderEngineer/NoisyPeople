//===================================================================================================================================
//�yTreeEffect.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/25
// [�X�V��]2019/12/04
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "TreeEffect.h"
#include "TextureLoader.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace TreeEffectNS;

#pragma region Instance
//===================================================================================================================================
//�y�}�[�J�[�C���X�^���X�N���X�F�R���X�g���N�^�z
//===================================================================================================================================
MarkerInstance::MarkerInstance(Tree* owner) : InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	//this->position	= position + D3DXVECTOR3((float)(rand() % 6 - 3), 0.0f, (float)(rand() % 6 - 3));
	this->tree = owner;
	position = tree->center;
	position.y += tree->size.y / 2;
	scale = D3DXVECTOR2(0.5f, 0.5f);
	//rotation = D3DXVECTOR3((float)(rand() % 20 - 10), (float)(rand() % 20 - 10), (float)(rand() % 20 - 10));
	rotation = D3DXVECTOR3(0, 0, 0);
	//instance.scale				= D3DXVECTOR2((float)(rand() % 20 + 1), (float)(rand() % 20 + 1));
	//speed = D3DXVECTOR3(0, 3.0f, 0.0f);

	changeColor();
	//limitTime = (float)(rand() % 30);
	//uv.x = 0.25f*sinf(limitTime * 4);
	//uv.y = 0.25f*cosf(limitTime * 4);
}
//===================================================================================================================================
//�y�}�[�J�[�C���X�^���X�N���X�F�X�V�z
//===================================================================================================================================
void MarkerInstance::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
	//�J���[
	//color.r = sinf(lifeTimer * 8);
	//color.g = cosf(lifeTimer * 4);
	//color.b = sinf(limitTime);
	//color.a = 1.0f - (lifeTimer / limitTime);
	changeColor();

	//UV
	//float time = limitTime - lifeTimer;
	//uv.x = 0.25f*sinf(time * 1.5);
	//uv.y = 0.25f*cosf(time * 0.5);

	//�ʒu
	position = tree->center;
	position.y += tree->size.y / 2;
	//position += speed * frameTime;
}
//===================================================================================================================================
//�y�}�[�J�[�C���X�^���X�N���X�F��ԕʐF�ύX�z
//===================================================================================================================================
void MarkerInstance::changeColor()
{
	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	{
		color = D3DCOLOR_RGBA(0, 0, 255, 196);
		return;
	}

	if (tree->getTreeData()->greenState == treeNS::GREEN)
	{
		color = D3DCOLOR_RGBA(0, 255, 0, 196);
	}
	else {
		color = D3DCOLOR_RGBA(255, 0, 0, 196);
	}
}

//===================================================================================================================================
//�y�W���C���X�^���X�N���X�F�R���X�g���N�^�z
//===================================================================================================================================
SignInstance::SignInstance(Tree* owner) : InstancingBillboardNS::Instance()
{
	//�����l�̐ݒ�
	//this->position	= position + D3DXVECTOR3((float)(rand() % 6 - 3), 0.0f, (float)(rand() % 6 - 3));
	this->tree = owner;
	position = tree->center;
	//rotation = D3DXVECTOR3((float)(rand() % 20 - 10), (float)(rand() % 20 - 10), (float)(rand() % 20 - 10));
	rotation = D3DXVECTOR3(0, 0, 0);
	//instance.scale				= D3DXVECTOR2((float)(rand() % 20 + 1), (float)(rand() % 20 + 1));
	scale = D3DXVECTOR2(1.0f*4.0f, 1.125f*4.0f);
	//speed = D3DXVECTOR3(0, 3.0f, 0.0f);
	changeColor();
	//limitTime = (float)(rand() % 30);
	//uv.x = 0.25f*sinf(limitTime * 4);
	//uv.y = 0.25f*cosf(limitTime * 4);
}
//===================================================================================================================================
//�y�W���C���X�^���X�N���X�F�X�V�z
//===================================================================================================================================
void SignInstance::update(float frameTime)
{
	//lifeTimer += frameTime;
	if (lifeTimer >= limitTime)return;
	//�J���[
	//color.r = sinf(lifeTimer * 8);
	//color.g = cosf(lifeTimer * 4);
	//color.b = sinf(limitTime);
	//color.a = 1.0f - (lifeTimer / limitTime);
	changeColor();
	//UV
	//float time = limitTime - lifeTimer;
	//uv.x = 0.25f*sinf(time * 1.5);
	//uv.y = 0.25f*cosf(time * 0.5);
	
	//�ʒu
	position = tree->center;
	//position += speed * frameTime;
}

//===================================================================================================================================
//�y�W���C���X�^���X�N���X�F��ԕʐF�ύX�z
//===================================================================================================================================
void SignInstance::changeColor()
{
	if (tree->getTreeData()->type == treeNS::DIGITAL_TREE)
	{
		color = D3DCOLOR_RGBA(0, 0, 255, 196);
		return;
	}

	if (tree->getTreeData()->greenState == treeNS::GREEN)
	{
		color = D3DCOLOR_RGBA(0, 255, 0, 196);
	}
	else {
		color = D3DCOLOR_RGBA(255, 0, 0, 196);
	}
}

#pragma endregion


//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TreeEffect::TreeEffect()
{
	effectCycle = 0.0f;

	//��ԃ}�[�J�[
	marker = new InstancingBillboard();
	marker->initialize(*textureNS::reference(textureNS::UI_TREE_MARKER));
	marker->setRenderType(InstancingBillboardNS::FOREGROUND_PASS);

	//�c���[�W��
	sign = new InstancingBillboard();
	sign->initialize(*textureNS::reference(textureNS::UI_TREE_SIGN));
	sign->setRenderType(
		InstancingBillboardNS::FOREGROUND_PASS | 
		InstancingBillboardNS::Y_BILLBOARD_PASS);
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TreeEffect::~TreeEffect()
{
	SAFE_DELETE(marker);
	SAFE_DELETE(sign);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void TreeEffect::update(float frameTime)
{
	//�G�t�F�N�g�̔���
	//effectCycle += frameTime;
	//�X�V
	marker->update(frameTime);
	sign->update(frameTime);

	resetEffectCycle();

}

//===================================================================================================================================
//�y�����z
//===================================================================================================================================
void TreeEffect::generateInstance(int type,Tree* tree)
{
	switch (type)
	{
	case INSTANCE_MARKER:	marker->generateInstance(new MarkerInstance(tree));		break;
	case INSTANCE_SIGN:		sign->generateInstance(new SignInstance(tree));						break;
	}
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void TreeEffect::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//�`��
	marker->render(view,projection,cameraPosition);
	sign->render(view,projection,cameraPosition);
}

//===================================================================================================================================
//�y�`��@�\��L��������z
//===================================================================================================================================
void TreeEffect::enableRender()
{
	marker->enableRender();
	sign->enableRender();
}

//===================================================================================================================================
//�y�`��@�\�𖳌�������z
//===================================================================================================================================
void TreeEffect::disableRender()
{
	marker->disableRender();
	sign->disableRender();
}



//===================================================================================================================================
//�y�W���G�t�F�N�g�z
//===================================================================================================================================
void TreeEffect::resetEffectCycle()
{
	//�G�t�F�N�g�̔���
	if (effectCycle >= EFFECT_CYCLE)
	{
		effectCycle = 0.0f;
	}

}

//===================================================================================================================================
//�y�W���G�t�F�N�g�z
//===================================================================================================================================
void TreeEffect::playStandardEffect(D3DXVECTOR3* position)
{
	//TreeEffectNS::DigitTree* instance = new TreeEffectNS::DigitTree(position);
	//effekseerNS::play(instance);
}

//===================================================================================================================================
//�y��P���G�t�F�N�g�z
//===================================================================================================================================
void TreeEffect::playRaidEffect(int no,D3DXVECTOR3* position)
{
	TreeEffectNS::DigitTreeRaid* instance = new TreeEffectNS::DigitTreeRaid(position);
	effekseerNS::play(no,instance);
}
