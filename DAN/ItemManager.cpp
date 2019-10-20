//-----------------------------------------------------------------------------
// �A�C�e���Ǘ��N���X [Item.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/4
//-----------------------------------------------------------------------------
#include "ItemManager.h"
#include "StaticMeshObject.h"
#include "ImguiManager.h"
using namespace itemNS;


//=============================================================================
// ������
//=============================================================================
void ItemManager::initialize()
{
	itemList.reserve(INITIAL_RESERVE);
}


//=============================================================================
// �I������
//=============================================================================
void ItemManager::uninitialize()
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		SAFE_DELETE(itemList[i]);
	}

	std::vector<Item*> temp;
	itemList.swap(temp);
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
}


//=============================================================================
// �`�揈��
//=============================================================================
void ItemManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		itemList[i]->render(view, projection, cameraPosition);
	}
}


//=============================================================================
// �A�C�e���I�u�W�F�N�g�̍쐬
//=============================================================================
void ItemManager::createItem(ItemData* itemData)
{
	switch (itemData->itemType)
	{
	case BATTERY:
		itemList.emplace_back(new Battery(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS), itemData));
		break;
	case EXAMPLE:
		itemList.emplace_back(new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData));
		break;
	}
}


//=============================================================================
// �A�C�e���I�u�W�F�N�g�̔j��
//=============================================================================
void ItemManager::destroyItem(int _id)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		if (itemList[i]->getItemData()->id == _id)
		{
			delete itemList[i];
			itemList.erase(itemList.begin() + i);
		}
	}
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

		//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//�ʒu
		//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//��]
		//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//�X�P�[��
		//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//���a
		//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//���ߒl
		//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//���x
		//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//�����x
		//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//�d��

		//ImGui::Checkbox("onGravity", &onGravity);										//�d�͗L�����t���O
		//ImGui::Checkbox("onActive", &onActive);										//�A�N�e�B�u���t���O
		//ImGui::Checkbox("onRender", &onRender);										//�`��L�����t���O
		//ImGui::Checkbox("onLighting", &onLighting);									//���������t���O
		//ImGui::Checkbox("onTransparent", &onTransparent);								//���߃t���O
		//ImGui::Checkbox("operationAlpha", &operationAlpha);							//���ߒl�̑���L���t���O

		//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//���ߒl�̑���L���t���O
	}

#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<Item*>& ItemManager::getList() { return itemList; }