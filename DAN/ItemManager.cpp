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
#if 0
	// �A�C�e���f�[�^���X�g�̒ǉ�
	ItemData itemData;
	itemDataList.reserve(/*�c�[���Ő��������t�@�C���Ɋi�[���ꂽ�A�C�e���f�[�^�̐�*/ +NUM_SURPLUS_DATA);
	for (int i = 0; i < itemDataList.size(); i++)
	{
		itemData.zeroClear();
		if (i >= /*�c�[���Ő��������t�@�C���Ɋi�[���ꂽ�A�C�e���f�[�^�̐�*/ 0/*��*/) continue;

		// �t�@�C������f�[�^��ǂݎ��

		// �f�[�^���X�g�ɒǉ�����
		itemDataList.push_back(itemData);
	}

	// �A�C�e���I�u�W�F�N�g�̍쐬
	itemList.reserve(itemDataList.size());
	for (size_t i = 0; itemDataList.size() - NUM_SURPLUS_DATA; i++)
	{
		createItem(&itemDataList[i]);
	}
#endif

	// �`��I�u�W�F�N�g���쐬
	batteryRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
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
}


//=============================================================================
// �`�揈��
//=============================================================================
void ItemManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	batteryRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// �A�C�e���I�u�W�F�N�g�̍쐬
//=============================================================================
void ItemManager::createItem(ItemData* itemData)
{
	Item* item = NULL;

	switch (itemData->itemType)
	{
	case BATTERY:
		item = new Battery(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS), itemData);
		itemList.emplace_back(item);
		batteryRenderer->generateObject(item);
		break;
	case EXAMPLE:
		itemList.emplace_back(new exampleItem(staticMeshNS::reference(staticMeshNS::YAMADA_ROBOT2), itemData));
		break;
	}
}


//=============================================================================
// �A�C�e���I�u�W�F�N�g�̔j��
// ���g�p�s��
//=============================================================================
void ItemManager::destroyItem(int _id)
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		if (itemList[i]->getItemData()->id == _id)
		{
			SAFE_DELETE(itemList[i]);
			itemList.erase(itemList.begin() + i);
			break;
		}
	}
	// batteryRenderer->deleteObject();
	// ����StaticMeshObject�ɓo�^���ꂽ�I�u�W�F�N�g���ʂɔj���ł��Ȃ�
}


//=============================================================================
// �S�A�C�e���I�u�W�F�N�g�̔j��
// ��������
//=============================================================================
void ItemManager::destroyAllItem()
{
	for (size_t i = 0; i < itemList.size(); i++)
	{
		SAFE_DELETE(itemList[i]);
	}
	
	itemList.clear();
	SAFE_DELETE(batteryRenderer);
	batteryRenderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
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