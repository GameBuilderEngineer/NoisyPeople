//-----------------------------------------------------------------------------
// �A�C�e���Ǘ��N���X [Item.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "ItemManager.h"
#include "StaticMeshRenderer.h"
#include "ImguiManager.h"
using namespace itemNS;

//=============================================================================
// ������
//=============================================================================
void ItemManager::initialize()
{
	nextID = 0;		// ���񔭍sID��0�ɏ�����

	// �`��I�u�W�F�N�g���쐬
	batteryRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
	exampleItemRender = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2));
}


//=============================================================================
// �I������
//=============================================================================
void ItemManager::uninitialize()
{
	// �S�A�C�e���I�u�W�F�N�g��j��
	destroyAllItem();

	// �x�N�^�[�̊m�ۃ��������������i�������A���P�[�^�����ɖ߂��j
	std::vector<Item*> temp;
	itemList.swap(temp);

	// �`��I�u�W�F�N�g�̔j��
	SAFE_DELETE(batteryRenderer);
	SAFE_DELETE(exampleItemRender);
}


//=============================================================================
// �X�V����
//=============================================================================
void ItemManager::update(float frameTime)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		itemList[i]->update(frameTime);
	}

	batteryRenderer->update();
	exampleItemRender->update();
}


//=============================================================================
// �`�揈��
//=============================================================================
void ItemManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	batteryRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
	exampleItemRender->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// �A�C�e���I�u�W�F�N�g�̍쐬
//=============================================================================
void ItemManager::createItem(ItemData itemData)
{
	Item* item = NULL;
	//Item* item2 = NULL;

	switch (itemData.type)
	{
	case BATTERY:
		item = new Battery(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS), itemData);
		itemList.emplace_back(item);
		batteryRenderer->registerObject(item);
		break;
	case EXAMPLE:
		item = new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData);
		itemList.emplace_back(item);
		exampleItemRender->registerObject(item);
		//itemList.emplace_back(new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData));

		break;
	}
}


//=============================================================================
// �A�C�e���I�u�W�F�N�g�̔j��
//=============================================================================
void ItemManager::destroyItem(int _itemID)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		if (itemList[i]->getItemData()->itemID == _itemID)
		{
			// �`��̉���
			switch (itemList[i]->getItemData()->type)
			{
			case BATTERY:
				batteryRenderer->unRegisterObjectByID(itemList[i]->id);
				break;
			}
			SAFE_DELETE(itemList[i]);				// �C���X�^���X�j��
			itemList.erase(itemList.begin() + i);	// �x�N�^�[�v�f������
			break;
		}
	}
}


//=============================================================================
// �S�A�C�e���I�u�W�F�N�g�̔j��
//=============================================================================
void ItemManager::destroyAllItem()
{
	// �`���S����
	batteryRenderer->allUnRegister();
	exampleItemRender->allUnRegister();

	for (size_t i = 0; i < itemList.size(); i++)
	{
		SAFE_DELETE(itemList[i]);
	}
	itemList.clear();
}


//=============================================================================
// �A�C�e��ID�𔭍s����
//=============================================================================
int ItemManager::issueNewItemID()
{
	int ans = nextID;
	nextID++;
	return ans;
}


//=============================================================================
// ImGui�ɏo��
//=============================================================================
void ItemManager::outputGUI()
{
#ifdef _DEBUG
	if (ImGui::CollapsingHeader("ItemInformation"))
	{
		ImGuiIO& io = ImGui::GetIO();
		//float limitTop = 1000;
		//float limitBottom = -1000;

		ImGui::Text("numOfItem:%d", Item::getNumOfItem());
	}
#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<Item*>& ItemManager::getList() { return itemList; }