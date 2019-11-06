//-----------------------------------------------------------------------------
// �������f�o�b�O [MemoryDebug.cpp]
//-----------------------------------------------------------------------------
#include "MemoryDebug.h"
#include <vector>


#define LOOPTIME (100)

//=============================================================================
// �G�l�~�[�}�l�[�W��
//=============================================================================
void MemoryDebug::debugEnemyManager()
{
	int numAfterCreate, numAfterDestroy;
	int numDataAfterCreate, numDataAfterDestroy;
	EnemyManager* enemyManager = new EnemyManager;
	enemyManager->initialize(NULL, NULL, NULL, NULL);

	// enemyID
	for(int i = 0; i < LOOPTIME; i++)
	{
		enemyNS::ENEMYSET temp =
		{
			enemyManager->issueNewEnemyID(),
			enemyNS::WOLF,
			stateMachineNS::CHASE,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
		};

		enemyNS::EnemyData* p = enemyManager->createEnemyData(temp);
		enemyManager->createEnemy(p);
	}
	numAfterCreate = enemyManager->getEnemyList().size();
	numDataAfterCreate = enemyManager->getEnemyDataList()->nodeNum;

#if 0
	// �S�Ĕj��
	enemyManager->destroyAllEnemy();
	//enemyManager->destroyAllEnemyData();
#endif

#if 1
	// ID�Ŕj��
	for (int i = 0; i < enemyManager->getNextID(); i++)
	{
		enemyManager->destroyEnemy(i);
		//enemyManager->destroyEnemyData(i);
	}
#endif

	numAfterDestroy = enemyManager->getEnemyList().size();
	numDataAfterDestroy = enemyManager->getEnemyDataList()->nodeNum;

	return;
}


//=============================================================================
// ���̑��}�l�[�W��
//=============================================================================
void MemoryDebug::debugSpawnManager()
{
	// �c���[�}�l�[�W��
	TreeManager* treeManager = new TreeManager;
	treeManager->initialize(NULL, NULL);

	for (int i = 0; i < LOOPTIME; i++)
	{
		treeNS::TreeData treeData;
		treeData.treeID = treeManager->issueNewTreeID();
		treeData.geenState = treeNS::GREEN;
		treeData.model = treeNS::A_MODEL;
		treeData.initialPosition = D3DXVECTOR3(0, 0, 0);

		treeManager->createTree(treeData);
	}
	
	int sizeTree = treeManager->getTreeList().size();
	treeManager->destroyAllTree();
	

	// �A�C�e���}�l�[�W��
	ItemManager* itemManager = new ItemManager;
	itemManager->initialize(NULL, NULL);

	for (int i = 0; i < LOOPTIME; i++)
	{
		itemNS::ItemData itemData;
		itemData.itemID = itemManager->issueNewItemID();
		itemData.type = itemNS::BATTERY;
		itemManager->createItem(itemData);
	}

	int sizeItem = itemManager->getItemList().size();

#if 1
	for (int i = 0; i < LOOPTIME; i++)
	{
		itemManager->destroyItem(i);
	}
#endif
#if 0
	itemManager->destroyAllItem();
#endif

	int sizeItemAfter = itemManager->getItemList().size();
	return;
}
