//===================================================================================================================================
//�yItemTools.cpp�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#include "ItemTools.h"
#include "ImguiManager.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
ITEM_TOOLS::ITEM_TOOLS()
{
	//�A�C�e���t�@�C���\���̂̏�����
	itemFile = { 0 };
	itemFile.ifmt = nullptr;

	//�p�X
	setToolsDirectory();

	//�t�@�C��
	FILE	*fp = NULL;
	fp = fopen(ITEM_FILE_PATH, "rb");
	if (fp == NULL)	//�t�@�C����������܂���
	{
		//�A�C�e���t�@�C���̐V�K�쐬
		CreatNewItemFile();
	}
	else
	{
		//ITEM�̓ǂݍ���
		fread(&itemFile.item, sizeof(ITEM_ITEM), 1, fp);

		//�������m�ۂ�IFMT�̓ǂݍ���
		if (itemFile.item.itemMax != NULL)
		{
			//�������m��
			itemFile.ifmt = new ITEM_IFMT[itemFile.item.itemMax];

			for (int i = 0; i < itemFile.item.itemMax; i++)
			{
				//IFMT�̓ǂݍ���
				fread(&itemFile.ifmt[i], sizeof(ITEM_IFMT), 1, fp);
			}
		}


		fclose(fp);

		//�A�C�e�����
		ItemListboxCurrent = 0;
		ItemListboxType = itemNS::ITEM_TYPE::BATTERY;

		//�`��p
		//renderer = new StaticMeshObject(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
		//initialize();
		//for (int i = 0; i < enemyFile.enmy.enemyMax; i++)
		//{
		//	generate(D3DXVECTOR3(enemyFile.efmt[i].posX, enemyFile.efmt[i].posY, enemyFile.efmt[i].posZ));
		//	renderer->updateAccessList();
		//}
	}

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
ITEM_TOOLS::~ITEM_TOOLS()
{
	//�t�@�C���̏����o��
	OutputItemFile();

	if (itemFile.item.itemMax != NULL)
	{
		SAFE_DELETE_ARRAY(itemFile.ifmt);
	}

	//�`��p
	//SAFE_DELETE(renderer);
}

//===================================================================================================================================
//�y�A�C�e���t�@�C���̐V�K�쐬�z
//===================================================================================================================================
void ITEM_TOOLS::CreatNewItemFile(void)
{
	//�t�@�C��
	FILE	 *fp = NULL;
	fp = fopen(ITEM_FILE_PATH, "wb");

	//�A�C�e���t�@�C���\����
	ITEM_FILE tmpEnemyFile;

	//�`�����N
	memcpy(tmpEnemyFile.item.chunkId, ITEM_CHUNK, sizeof(tmpEnemyFile.item.chunkId));

	//�A�C�e���̏�����
	tmpEnemyFile.item.itemMax = 0;

	//ENMY�̃T�C�Y
	tmpEnemyFile.item.size = sizeof(ITEM_ITEM);

	//�����o��
	fwrite(&tmpEnemyFile.item, sizeof(ITEM_ITEM), 1, fp);

	fclose(fp);
}

//===================================================================================================================================
//�y�A�C�e���̎�ނ�ݒ�z
//===================================================================================================================================
void ITEM_TOOLS::SetItemType(short itemId, short itemType)
{
	itemFile.ifmt[itemId].itemType = itemType;
}

//===================================================================================================================================
//�y�A�C�e���̈ʒu��ݒ�z
//===================================================================================================================================
void ITEM_TOOLS::SetItemPos(short itemId, const D3DXVECTOR3 pos)
{
	itemFile.ifmt[itemId].posX = pos.x;
	itemFile.ifmt[itemId].posY = pos.y;
	itemFile.ifmt[itemId].posZ = pos.z;
}

//===================================================================================================================================
//�y�A�C�e���̉�]����ݒ�z
//===================================================================================================================================
void ITEM_TOOLS::SetItemDir(short itemId, const D3DXVECTOR3 dir)
{
	itemFile.ifmt[itemId].dirX = dir.x;
	itemFile.ifmt[itemId].dirY = dir.y;
	itemFile.ifmt[itemId].dirZ = dir.z;
}

//===================================================================================================================================
//�y�A�C�e���̐ݒu�z
//===================================================================================================================================
void ITEM_TOOLS::SetItem(short itemId, short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�`�����N
	memcpy(itemFile.ifmt[itemId].chunkId, ITEM_CHUNK, sizeof(itemFile.ifmt[itemId].chunkId));

	//�A�C�e�����
	SetItemType(itemId, itemType);
	SetItemPos(itemId, pos);
	SetItemDir(itemId, dir);

	//�T�C�Y
	itemFile.ifmt[itemId].size = (short)sizeof(ITEM_IFMT);
}

//===================================================================================================================================
//�y�A�C�e���t�@�C���̏����o�������z
//===================================================================================================================================
void ITEM_TOOLS::OutputItemFile(void)
{
	//�p�X
	setToolsDirectory();

	FILE	*fp = NULL;					//�t�@�C��
	fp = fopen(ITEM_FILE_PATH, "wb");

	//ITEM�̏����o��
	fwrite(&itemFile.item, sizeof(ITEM_ITEM), 1, fp);

	//IFMT�̏����o��
	for (int i = 0; i < itemFile.item.itemMax; i++)
	{
		//IFMT�̏����o��
		fwrite(&itemFile.ifmt[i], sizeof(ITEM_IFMT), 1, fp);
	}

	fclose(fp);
}

//===================================================================================================================================
//�y�A�C�e���̃t�H�[�}�b�g�\���̂�ǉ��z
//===================================================================================================================================
void ITEM_TOOLS::AddItemFormat(short itemType, const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
	//�A�C�e���̐�+1
	itemFile.item.itemMax++;

	//�A�C�e���̃t�H�[�}�b�g�\���̂𐮗�
	UpdateIfmt(itemFile.item.itemMax - 1);

	//�A�C�e���̃t�H�[�}�b�g�\���̂̍Ō�ɒǉ�
	SetItem(itemFile.item.itemMax - 1, itemType, pos, dir);

	//�`��p
	//generate(pos);
	//renderer->updateAccessList();
	//needUpdate = true;
}

//===================================================================================================================================
//�y�A�C�e���̍ő吔���擾�z
//===================================================================================================================================
int ITEM_TOOLS::GetItemMax(void)
{
	return itemFile.item.itemMax;
}

//===================================================================================================================================
//�y�A�C�e���̃t�H�[�}�b�g�\���̂𐮗��z
//===================================================================================================================================
void ITEM_TOOLS::UpdateIfmt(int oldItemMax)
{
	//�o�b�N�A�b�v�p
	ITEM_IFMT *backUpIfmt = new ITEM_IFMT[itemFile.item.itemMax];

	//����(itemType��ITEM_TYPE::ITEM_TYPE_MAX�Ȃ���)
	int newItemMax = 0;	//���g������ifmt
	for (int i = 0; i < oldItemMax; i++)
	{
		if (itemFile.ifmt[i].itemType == itemNS::ITEM_TYPE::ITEM_TYPE_MAX)
		{
			//�X�L�b�v
			continue;
		}
		else
		{
			//�o�b�N�A�b�v�����
			backUpIfmt[newItemMax] = itemFile.ifmt[i];
			newItemMax++;
		}
	}

	//������蒼���K�v�������
	if (oldItemMax > 0)
	{
		//���������
		SAFE_DELETE_ARRAY(itemFile.ifmt);
	}

	//�������m�ۂ�����
	if (itemFile.item.itemMax > 0)
	{
		itemFile.ifmt = new ITEM_IFMT[itemFile.item.itemMax];

		//���[�h�o�b�N�A�b�v�f�[�^
		for (int i = 0; i < newItemMax; i++)
		{
			itemFile.ifmt[i] = backUpIfmt[i];
			itemFile.ifmt[i].itemId = i;	//Id�̍X�V
		}

		//ITEM�̏�������
		itemFile.item.size = (short)(sizeof(ITEM_ITEM) + (sizeof(ITEM_IFMT) * itemFile.item.itemMax));
	}
	//���������
	SAFE_DELETE_ARRAY(backUpIfmt);
}

//===================================================================================================================================
//�y�A�C�e���\���̂��擾�z
//===================================================================================================================================
itemNS::ITEMSET ITEM_TOOLS::GetItemSet(short itemId)
{
	itemNS::ITEMSET tmpItemSet = { 0 };
	tmpItemSet.id = itemFile.ifmt[itemId].itemId;
	tmpItemSet.type = itemFile.ifmt[itemId].itemType;
	tmpItemSet.defaultPosition = D3DXVECTOR3(itemFile.ifmt[itemId].posX, itemFile.ifmt[itemId].posY, itemFile.ifmt[itemId].posZ);
	tmpItemSet.defaultDirection = D3DXVECTOR3(itemFile.ifmt[itemId].dirX, itemFile.ifmt[itemId].dirY, itemFile.ifmt[itemId].dirZ);

	return tmpItemSet;
}

//===================================================================================================================================
//�yImGUI�ւ̏o�́z
//===================================================================================================================================
void ITEM_TOOLS::outputItemToolsGUI(const D3DXVECTOR3 pos, const D3DXVECTOR3 dir)
{
#ifdef _DEBUG
	//�t���O
	bool creatFlag = false;
	bool deleteFlag = false;

	ImGui::Begin("Item Tools");

	//�@�\
	ImGui::Checkbox("New Item", &creatFlag);
	ImGui::Checkbox("Delete", &deleteFlag);

	//�A�C�e���̎��
	const char* listboxItemType[] = { "BATTERY" };
	ImGui::ListBox("Item Type", &ItemListboxType, listboxItemType, itemNS::ITEM_TYPE::ITEM_TYPE_MAX);

	//�A�C�e���̏��
	if (itemFile.item.itemMax != NULL)
	{
		//�������m��
		char **ListboxIndexName = new char*[itemFile.item.itemMax];
		for (int i = 0; i < itemFile.item.itemMax; i++)
		{
			ListboxIndexName[i] = new char[CHAR_MAX];
			sprintf(ListboxIndexName[i], "%d", i);
		}

		ImGui::ListBox("Item ", &ItemListboxCurrent, ListboxIndexName, itemFile.item.itemMax);
		ImGui::Text("Item pos:%f %f %f", itemFile.ifmt[ItemListboxCurrent].posX,
			itemFile.ifmt[ItemListboxCurrent].posY,
			itemFile.ifmt[ItemListboxCurrent].posZ);
		ImGui::Text("Item dir:%f %f %f", itemFile.ifmt[ItemListboxCurrent].dirX,
			itemFile.ifmt[ItemListboxCurrent].dirY,
			itemFile.ifmt[ItemListboxCurrent].dirZ);
		ImGui::Text("Item Type:%s",
			listboxItemType[itemFile.ifmt[ItemListboxCurrent].itemType]);

		//���������
		for (int i = 0; i < itemFile.item.itemMax; i++)
		{
			delete[] ListboxIndexName[i];
		}
		delete[] ListboxIndexName;
	}

	ImGui::End();

	//�I��


	//�폜
	if (deleteFlag)
	{
		//�폜
		DeleteItemFormat(ItemListboxCurrent);

		//�߂�
		ItemListboxCurrent--;
		if (ItemListboxCurrent < 0)
		{
			ItemListboxCurrent = 0;
		}
	}

	//�V�K�쐬
	if (creatFlag)
	{
		//�쐬
		AddItemFormat(ItemListboxType, pos, dir);

		//�i��
		ItemListboxCurrent = itemFile.item.itemMax - 1;
	}
#endif
}


//===================================================================================================================================
//�y�A�C�e���̃t�H�[�}�b�g�\���̂������z
//===================================================================================================================================
void ITEM_TOOLS::DeleteItemFormat(short itemId)
{
	//�����p(MAX�Ȃ����)
	if (itemFile.item.itemMax != NULL)
	{
		//�������
		itemFile.item.itemMax--;
		if (itemFile.item.itemMax < 0)
		{
			itemFile.item.itemMax = 0;
		}

		//TYPE_MAX�Ȃ����
		itemFile.ifmt[itemId].itemType = itemNS::ITEM_TYPE::ITEM_TYPE_MAX;

		//�A�C�e���̃t�H�[�}�b�g�\���̂𐮗�
		UpdateIfmt(itemFile.item.itemMax + 1);

		//�`��p
		//renderer->deleteObject(enemyFile.enmy.enemyMax - enemyId);
		//renderer->updateAccessList();
		//needUpdate = true;
	}
}
