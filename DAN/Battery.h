//-----------------------------------------------------------------------------
// �o�b�e���[�N���X [Battery.h]
// Author : 
// �쐬�J�n�� : 2019/  /  
//-----------------------------------------------------------------------------
#pragma once
#include"Item.h"


//=============================================================================
//�N���X��`
//=============================================================================
class Battery: public Item
{
private:
	static StaticMesh* staticMesh;

public:
	Battery(StaticMesh* _staticMesh, itemNS::ItemData* _itemData);
	static void initialize(StaticMesh* staticMesh);
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	//bool sphereCollide(D3DXVECTOR3 targetCenter, float targetRadius, D3DXMATRIX targetMatrix) override;

	// Getter

	// Setter
};
