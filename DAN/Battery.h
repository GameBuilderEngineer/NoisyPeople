//-----------------------------------------------------------------------------
// �o�b�e���[�N���X [Battery.h]
// Author : �V��
// �쐬�J�n�� : 2019/10/15  
//-----------------------------------------------------------------------------
#pragma once
#include"Item.h"

//=============================================================================
// ���O���
//=============================================================================
namespace batteryNS
{
	const int RECOVERY_POWER = 85;			//�d�͉񕜗�
}

//=============================================================================
//�N���X��`
//=============================================================================
class Battery: public Item
{
private:
	static StaticMesh* staticMesh;

public:
	Battery(StaticMesh* _staticMesh, itemNS::ItemData _itemData);
	static void initialize(StaticMesh* staticMesh);
	virtual void update(float frameTime);
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	// Getter

	// Setter
};
