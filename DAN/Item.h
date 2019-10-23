//-----------------------------------------------------------------------------
// �A�C�e�����N���X [Item.h]
// Author : 
// �쐬�J�n�� : 2019/  /
//-----------------------------------------------------------------------------
#pragma once
#include "StaticMeshRenderer.h"
#include "BoundingSphere.h"


//=============================================================================
// ���O���
//=============================================================================
namespace itemNS
{
	enum ITEM_TYPE
	{
		BATTERY,
		ITEM_TYPE_MAX
	};


	struct ItemData
	{
		int id;							// ���ʔԍ�
		ITEM_TYPE itemType;				// �A�C�e���^�C�v
		D3DXVECTOR3 initialPosition;	// �������W
		void zeroClear() { ZeroMemory(this, sizeof(ItemData)); }
	};
}


//=============================================================================
//�N���X��`
//=============================================================================
class Item: public Object
{
private:
	itemNS::ItemData* itemData;

	// Static
	static int numOfItem;					// �A�C�e���I�u�W�F�N�g�̑���

public:
	Item(StaticMesh* _staticMesh, itemNS::ItemData* _itemData);
	~Item();
	virtual void update(float frameTime);
	BoundingSphere sphereCollider;

	// Getter
	static int getNumOfItem();				// �A�C�e���̐����擾
	itemNS::ItemData* getItemData();		// �A�C�e���f�[�^���擾

	// Setter
	void setDataToItem(itemNS::ItemData* _itemData);
};
