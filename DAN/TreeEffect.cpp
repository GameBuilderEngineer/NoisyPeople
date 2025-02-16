//===================================================================================================================================
//【TreeEffect.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/11/25
// [更新日]2019/12/04
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "TreeEffect.h"
#include "TextureLoader.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace TreeEffectNS;

#pragma region Instance
//===================================================================================================================================
//【マーカーインスタンスクラス：コンストラクタ】
//===================================================================================================================================
MarkerInstance::MarkerInstance(Tree* owner) : InstancingBillboardNS::Instance()
{
	//初期値の設定
	this->tree = owner;
	position = tree->center;
	position.y += tree->size.y / 2;
	
	switch (owner->getTreeData()->size)
	{
	case treeNS::STANDARD:
		scale = D3DXVECTOR2(0.5f, 0.5f);
		break;

	case treeNS::LARGE:
		scale = D3DXVECTOR2(1.5f, 1.5f);
		break;

	case treeNS::VERY_LARGE:
		scale = D3DXVECTOR2(4.75f, 4.75f);
		position.y += 14.0f;
		break;
	}
	rotation = D3DXVECTOR3(0, 0, 0);

	changeColor();
}

//===================================================================================================================================
//【マーカーインスタンスクラス：更新】
//===================================================================================================================================
void MarkerInstance::update(float frameTime)
{
	if (lifeTimer >= limitTime)return;
	changeColor();

	//位置
	position = tree->center;
	position.y += tree->size.y / 2;
	switch (tree->getTreeData()->size)
	{
	case treeNS::STANDARD:
		position.y -= 1.1f;
		break;

	case treeNS::LARGE:
		position.y -= 2.65f;
		break;

	case treeNS::VERY_LARGE:
		position.y += 0.0f;
		break;
	}

	if (tree->getTreeData()->size == treeNS::VERY_LARGE)
	{
		position.y += 14.0f;
	}
}

//===================================================================================================================================
//【マーカーインスタンスクラス：状態別色変更】
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
//【標識インスタンスクラス：コンストラクタ】
//===================================================================================================================================
SignInstance::SignInstance(Tree* owner) : InstancingBillboardNS::Instance()
{
	//初期値の設定
	this->tree = owner;
	position = tree->center;
	rotation = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR2(1.0f*4.0f, 1.125f*4.0f);

	switch (owner->getTreeData()->size)
	{
	case treeNS::STANDARD:
		scale = D3DXVECTOR2(1.0f*4.0f, 1.125f*4.0f);
		position.y -= 1.1f;
		break;

	case treeNS::LARGE:
		scale = D3DXVECTOR2(1.0f*12.0f, 1.125f*12.0f);
		position.y -= 2.65f;
		break;

	case treeNS::VERY_LARGE:
		scale = D3DXVECTOR2(1.0f*38.0f, 1.125f*38.0f);
		position.y += 3.0f;
		break;
	}


	changeColor();
}

//===================================================================================================================================
//【標識インスタンスクラス：更新】
//===================================================================================================================================
void SignInstance::update(float frameTime)
{
	if (lifeTimer >= limitTime)return;

	changeColor();
	
	//位置
	position = tree->center;
	switch (tree->getTreeData()->size)
	{
	case treeNS::STANDARD:
		position.y -= 1.1f;
		break;

	case treeNS::LARGE:
		position.y -= 2.65f;
		break;

	case treeNS::VERY_LARGE:
		position.y += 3.0f;
		break;
	}

}

//===================================================================================================================================
//【標識インスタンスクラス：状態別色変更】
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
//【コンストラクタ】
//===================================================================================================================================
TreeEffect::TreeEffect()
{
	effectCycle = 0.0f;

	//状態マーカー
	marker = new InstancingBillboard();
	marker->initialize(*textureNS::reference(textureNS::UI_TREE_MARKER));
	marker->setRenderType(InstancingBillboardNS::FOREGROUND_PASS);

	//ツリー標識
	sign = new InstancingBillboard();
	sign->initialize(*textureNS::reference(textureNS::UI_TREE_SIGN));
	sign->setRenderType(
		InstancingBillboardNS::FOREGROUND_PASS | 
		InstancingBillboardNS::Y_BILLBOARD_PASS);
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
TreeEffect::~TreeEffect()
{
	SAFE_DELETE(marker);
	SAFE_DELETE(sign);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void TreeEffect::update(float frameTime)
{
	//更新
	marker->update(frameTime);
	sign->update(frameTime);

	resetEffectCycle();
}

//===================================================================================================================================
//【生成】
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
//【描画】
//===================================================================================================================================
void TreeEffect::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//描画
	marker->render(view,projection,cameraPosition);
	sign->render(view,projection,cameraPosition);
}

//===================================================================================================================================
//【描画機能を有効化する】
//===================================================================================================================================
void TreeEffect::enableRender()
{
	marker->enableRender();
	sign->enableRender();
}

//===================================================================================================================================
//【描画機能を無効化する】
//===================================================================================================================================
void TreeEffect::disableRender()
{
	marker->disableRender();
	sign->disableRender();
}

//===================================================================================================================================
//【標準エフェクト】
//===================================================================================================================================
void TreeEffect::resetEffectCycle()
{
	//エフェクトの発生
	if (effectCycle >= EFFECT_CYCLE)
	{
		effectCycle = 0.0f;
	}

}

//===================================================================================================================================
//【標準エフェクト】
//===================================================================================================================================
void TreeEffect::playStandardEffect(D3DXVECTOR3* position)
{

}

//===================================================================================================================================
//【被襲撃エフェクト】
//===================================================================================================================================
void TreeEffect::playRaidEffect(int no,D3DXVECTOR3* position)
{
	TreeEffectNS::DigitTreeRaid* instance = new TreeEffectNS::DigitTreeRaid(position);
	effekseerNS::play(no,instance);
}
