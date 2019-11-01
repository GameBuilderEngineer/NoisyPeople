//-----------------------------------------------------------------------------
// �A�C�e�����N���X [Item.cpp]
// Author : 
// �쐬�J�n�� : 2019/  /
//-----------------------------------------------------------------------------
#include "Item.h"
#include "ImguiManager.h"
using namespace itemNS;


int Item::numOfItem = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Item::Item(StaticMesh* _staticMesh, ItemData _itemData)
{
	numOfItem++;

	onGravity = true;
	itemData = _itemData;
	position = itemData.defaultPosition;
	Object::initialize(&position);
	sphereCollider.initialize(&position, _staticMesh->mesh);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
Item::~Item()
{
	numOfItem--;
}


//=============================================================================
// �X�V����
//=============================================================================
void Item::update(float frameTime)
{
	Object::update();
}


//=============================================================================
// �d�͔������b�V���i�ڒn���b�V���j�̐ݒ�
//=============================================================================
void Item::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// Getter
//=============================================================================
int Item::getNumOfItem(){ return numOfItem; }
ItemData* Item::getItemData() { return &itemData; }


//=============================================================================
// Setter
//=============================================================================
void Item::setDataToItem(ItemData _itemData) { itemData = _itemData; }
