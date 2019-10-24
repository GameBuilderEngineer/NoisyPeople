//-----------------------------------------------------------------------------
// �A�C�e���Ǘ��N���X [Item.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "Item.h"
#include "Battery.h"
#include "exampleItem.h"


//=============================================================================
// ���O���
//=============================================================================
namespace itemNS
{	
	const int NUM_SURPLUS_DATA = 10;		// �����A�C�e���f�[�^���ɉ����Ċm�ۂ���]�蕪�̃x�N�^�[�v�f��
}


//=============================================================================
//�N���X��`
//=============================================================================
class ItemManager
{
private:
	std::vector<itemNS::ItemData> itemDataList;	// �A�C�e���f�[�^���X�g
	std::vector<Item*> itemList;				// �A�C�e���|�C���^���X�g
	StaticMeshObject* batteryRenderer;
	StaticMeshObject* exampleItemRender;		// �e�X�g�p�A�C�e��
public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createItem(itemNS::ItemData* itemData);
	void destroyItem(int _id);
	void destroyAllItem();
	void outputGUI();

	// Getter
	std::vector<Item*>& getList();
};