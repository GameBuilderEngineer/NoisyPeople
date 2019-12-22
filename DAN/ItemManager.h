//-----------------------------------------------------------------------------
// �A�C�e���Ǘ��N���X [Item.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
#include "Item.h"
#include "Battery.h"
#include "exampleItem.h"


//=============================================================================
// ���O���
//=============================================================================
namespace itemNS
{	
	const int NUM_SURPLUS_DATA = 10;			// �����A�C�e���f�[�^���ɉ����Ċm�ۂ���]�蕪�̃x�N�^�[�v�f��

}


//=============================================================================
//�N���X��`
//=============================================================================
class ItemManager
{
private:
	static ItemManager* instance;				// �O���擾�p�|�C���^
	std::vector<Item*> itemList;				// �A�C�e���|�C���^���X�g
	StaticMeshRenderer* batteryRenderer;		// �`��I�u�W�F�N�g
	StaticMeshRenderer* exampleItemRender;		// �e�X�g�p�A�C�e��
	int nextID;									// ����c���[���sID
	LPD3DXMESH	attractorMesh;					// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;				// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X

public:
	// �O���擾�p
	ItemManager() { instance = this; }	// ���V���O���g���ł͂Ȃ�
	~ItemManager();
	static ItemManager* get() { return instance; }

	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createItem(itemNS::ItemData itemData);
	void destroyItem(int _itemID);
	void destroyAllItem();
	int issueNewItemID();
	void outputGUI();

	// Getter
	std::vector<Item*>& getItemList();
};
