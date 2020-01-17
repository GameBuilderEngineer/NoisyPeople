//-----------------------------------------------------------------------------
// �A�C�e�����N���X [Item.h]
// Author : 
// �쐬�J�n�� : 2019/  /
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "EffekseerManager.h"


//=============================================================================
// ���O���
//=============================================================================
namespace itemNS
{
	enum ITEM_TYPE
	{
		BATTERY,
		EXAMPLE,
		POWER_UP,
		ITEM_TYPE_MAX
	};

	typedef struct ItemData
	{
		int itemID;						// ���ʔԍ�(0..*)
		int type;						// �A�C�e���^�C�v
		D3DXVECTOR3 defaultPosition;	// �������W
		D3DXVECTOR3 defaultDirection;	// �������ʕ���
		void zeroClear() { ZeroMemory(this, sizeof(ItemData)); }
	}ITEMSET;

	//=============================================================================
	// �A�C�e���W���G�t�F�N�g
	//=============================================================================
	class ItemEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		ItemEffect(int no,D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
		}
		virtual void update()
		{
			position = *syncPosition;
			Instance::update();
		};
	};

	/*class ItemDestroyEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		ItemDestroyEffect(int no, D3DXVECTOR3* sync)
		{
			syncPosition = sync;
			managerNo = no;
			effectNo = effekseerNS::DROP_ITEM;
		}
		virtual void update()
		{
			position = *syncPosition;
			Instance::update();
		}
	};*/

}


//=============================================================================
//�N���X��`
//=============================================================================
class Item: public Object
{
private:
	itemNS::ItemData itemData;
	LPD3DXMESH	attractorMesh;				// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;			// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X


	itemNS::ItemEffect* itemEffect;
	//itemNS::ItemDestroyEffect* itemDestroyEffect;

	// Static
	static int numOfItem;					// �A�C�e���I�u�W�F�N�g�̑���

public:
	BoundingSphere sphereCollider;			// �R���C�_
	Item(StaticMesh* _staticMesh, itemNS::ItemData _itemData);
	~Item();
	virtual void update(float frameTime);
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);

	// Getter
	static int getNumOfItem();				// �A�C�e���̐����擾
	itemNS::ItemData* getItemData();		// �A�C�e���f�[�^���擾

	// Setter
	void setDataToItem(itemNS::ItemData _itemData);
};
