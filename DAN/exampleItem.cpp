//-----------------------------------------------------------------------------
// �N���X [exampleItem.cpp]
// Author : �V��
// �쐬�J�n�� : 2019/ 10 / 17 
//-----------------------------------------------------------------------------
#include "exampleItem.h"
#include "ImguiManager.h"
using namespace itemNS;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
exampleItem::exampleItem(StaticMesh* _staticMesh, ItemData _itemData): Item(_staticMesh, _itemData)
{

}


//exampleItem::~exampleItem()
//{
//}

//=============================================================================
// �X�V����
//=============================================================================
void exampleItem::update(float frameTime)
{
	Item::update(frameTime);
}

//=============================================================================
// �`�揈��
//=============================================================================
void exampleItem::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//Item::render(view, projection, cameraPosition);
}