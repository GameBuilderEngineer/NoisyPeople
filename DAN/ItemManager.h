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
	const int INITIAL_RESERVE = 50;				// �A�C�e���|�C���^�̏����m�ې�
}


//=============================================================================
//�N���X��`
//=============================================================================
class ItemManager
{
private:
	std::vector<itemNS::ItemData> itemDataList;	// �A�C�e���f�[�^���X�g
	std::vector<Item*> itemList;				// �A�C�e���|�C���^���X�g

public:
	void initialize();
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createItem(itemNS::ItemData* itemData);
	void destroyItem(int _id);
	void outputGUI();

	// Getter
	std::vector<Item*>& getList();
};