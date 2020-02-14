//-----------------------------------------------------------------------------
// �N���X [exampleItem.h]
// Author : �V��
// �쐬�J�n�� : 2019/10/17  
//-----------------------------------------------------------------------------
#pragma once
#include "Item.h"

//=============================================================================
//�N���X��`
//=============================================================================
class exampleItem :public Item
{
private:
	static StaticMesh* staticMesh;

public:
	exampleItem(StaticMesh* _staticMesh, itemNS::ItemData _itemData);
	static void initialize(StaticMesh* staticMesh);
	virtual void update(float frametime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
 
	};

