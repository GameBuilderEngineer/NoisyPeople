//-----------------------------------------------------------------------------
// ���^AI�C�x���g�����N���X [EventMaker.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "EventMaker.h"
using namespace aiNS;


//=============================================================================
// ������
//=============================================================================
void EventMaker::initialize(aiNS::AnalyticalData* _data,
	OperationGenerator* _opeGenerator, GameMaster* _gameMaster, Player* _player, EnemyManager* _enemyManager,
	TreeManager* _treeManager, ItemManager* _itemManager, TelopManager* _telopManager)
{
	data = _data;
	opeGenerator = _opeGenerator;
	gameMaster = _gameMaster;
	player = _player;
	enemyManager = _enemyManager;
	treeManager = _treeManager;
	itemManager = _itemManager;
	telopManager = _telopManager;
}


//=============================================================================
// �I������
//=============================================================================
void EventMaker::uninitialize()
{
}


//=============================================================================
// �X�V����
//=============================================================================
void EventMaker::update()
{
	//// �G�l�~�[���I�쐬�C�x���g�̍쐬
	//if (enemyAD->shouldSpawnFuzzily > FUZZY_VALUE_SHOULD_SPAWN)
	//{
	//	makeEventSpawningEnemyAroundPlayer();
	//}
}


//=============================================================================
// �G�l�~�[���I�쐬�C�x���g�̍쐬�iSPAWN_ENEMY_AROUND_PLAYER)
//=============================================================================
void EventMaker::makeEventSpawningEnemyAroundPlayer()
{
	enemyNS::ENEMYSET enemySet;
	enemySet.enemyID = enemyManager->issueNewEnemyID();
	enemySet.type = decideSpawnEnemyType();
	enemySet.defaultPosition = createGroundedPositionFromPivot(player[0].position, 25.0f, 5.0f);
	enemySet.defaultDirection = *player[0].getPosition() - enemySet.defaultPosition;
	enemySet.defaultState = stateMachineNS::PATROL;
	opeGenerator->spawnEnemy(enemySet);
}


//=============================================================================
// �G�l�~�[���X�|�[���C�x���g�̍쐬�iRESPAWN_ENEMY)
//=============================================================================
void EventMaker::makeEventRespawnEnemy()
{
	// ���X�|�[���͍��̂Ƃ��낵�Ȃ�
}


//=============================================================================
// �G�l�~�[�f�W�^���c���[�P���C�x���g�̍쐬�iENEMY_ATTACKS_TREE�j
//=============================================================================
void EventMaker::makeEventEnemyAttaksTree()
{
	if (data->numDigital <= data->numBeingAttackedTree) { return; }

	// �P���c���[�����߂�
	int treeID = decideAttackTargetTree();

	// �P���Ώۃc���[�̏���
	Tree* attackTarget = NULL;
	D3DXVECTOR3 treePosition;
	std::vector<Tree*> treeList = treeManager->getTreeList();
	for (size_t i = 0; i < treeList.size(); i++)
	{
		if (treeList[i]->getTreeData()->treeID == treeID)
		{
			attackTarget = treeList[i];						// �U���Ώۃc���[���L�^
			treeList[i]->getTreeData()->isAttaked = true;	// �c���[�̍U�����t���O��On
			treePosition = treeList[i]->position;			// �c���[�̍��W���擾
		}
	}

	// �G�l�~�[�̐������߂�(2�̂�������3��)
	int numEnemy = 2 + rand() % 2;

	// �G�l�~�[�̃p�����[�^�쐬
	for (int i = 0; i < numEnemy; i++)
	{
		enemyNS::ENEMYSET enemySet;
		enemySet.enemyID = enemyManager->issueNewEnemyID();
		enemySet.type = decideSpawnEnemyType();
		enemySet.defaultState = stateMachineNS::ATTACK_TREE;
		enemySet.defaultPosition = createGroundedPositionFromPivot(treePosition, 10.0f, 3.0f);
		enemySet.defaultDirection = treePosition - enemySet.defaultPosition;

		// ���s
		opeGenerator->enemyAttaksTree(enemySet, attackTarget);
	}	
}


//=============================================================================
// ���I�쐬����G�l�~�[�̃p�����[�^�����߂�
//=============================================================================
enemyNS::ENEMY_TYPE EventMaker::decideSpawnEnemyType()
{
	enemyNS::ENEMY_TYPE ans;
	float score = 0.0f;

	// �ŋߓ|�����G�̐��������قǍ����Ȃ�悤�X�R�A���v���b�g
	score = fuzzy.grade((float)data->numKilledRecently, 0.0f, 7.0f);

	// �v���C���[2���̃R���f�B�V�����ɂ���ăX�R�A��␳
	int sum = (data->playerCondition[gameMasterNS::PLAYER_1P] + data->playerCondition[gameMasterNS::PLAYER_2P]);
	switch (sum)
	{
	case 0:	// GOOD + GOOD
		score += 0.0;
		break;

	case 1:	// GOOD + NORMAL
		score += 0.1;
		break;

	case 2: // GOOD + BAD, NORMAL + NORMAL
		score += 0.3f;
		break;

	case 3:	// NORMAL + BAD
		score += 0.4f;
		break;

	case 4:	// BAD + BAD
		score += 0.5f;
		break;
	}

	// �����ŃX�R�A���h�a
	score += (float)((rand() % 5) - 2);

	// �X�R�A�ɂ���ăG�l�~�[�̃^�C�v��ύX����
	if (score > 0.7f)
	{
		ans = enemyNS::WOLF;
	}
	else if (score > 0.3f)
	{
		ans = enemyNS::TIGER;
	}
	else
	{
		ans = enemyNS::BEAR;
	}

	return ans;
}


//=============================================================================
// �P���C�x���g�Ώۃf�W�^���c���[��I�肷��
//=============================================================================
int EventMaker::decideAttackTargetTree()
{
	using gameMasterNS::PLAYER_1P;
	using gameMasterNS::PLAYER_2P;
	using gameMasterNS::PLAYER_NUM;

	// ��⃊�X�g���쐬�i�v�f�� = �f�W�^���c���[�̐� - ��O�c���[�̐� - �U�����c���[�̐��j
	int candidateMax = data->numDigital /*- EXCEPTION_TREE_MAX����������*/ - data->numBeingAttackedTree;
	AttackCandidate* candidateList = new AttackCandidate[candidateMax];
	int cntCandidate = 0;

	// ��⃊�X�g�𖄂߂Ă���
	std::vector<Tree*> treeList = treeManager->getTreeList();
	bool isPass;				// ���[�v�p�X�t���O
	float maxDistance = 0.0f;	// �ő勗��
	for (size_t i = 0; i < treeList.size(); i++)
	{
		isPass = false;
		//---------------------------------------------------------------------
		// �A�i���O�c���[�ƑΏۊO�̃f�W�^���c���[�ɑ΂��ď������p�X����
		if (treeList[i]->getTreeData()->type != treeNS::DIGITAL_TREE) { isPass = true; }
		for (int k = 0; k < EXCEPTION_TREE_MAX; k++)
		{
			if (treeList[i]->getTreeData()->treeID == EXCEPTION_TREE_ID[k])
			{
				isPass = true;
			}
		}
		if (treeList[i]->getTreeData()->isAttaked) { isPass = true; }
		if (isPass) { continue; }
		//---------------------------------------------------------------------
		// ID���L�^
		candidateList[cntCandidate].treeID = treeList[i]->getTreeData()->treeID;
		// �����𑪂�
		float distanceToPlayer[PLAYER_NUM];
		for (int k = 0; k < PLAYER_NUM; k++)
		{
			distanceToPlayer[k] = between2VectorLength(player[k].position, treeList[i]->getTreeData()->initialPosition);
		}
		// �߂��v���C���[�Ƃ̋�����ۊ�
		if (distanceToPlayer[PLAYER_1P] < distanceToPlayer[PLAYER_2P])
		{
			candidateList[cntCandidate].distanceToNearPlayer = distanceToPlayer[PLAYER_1P];
		}
		else
		{
			candidateList[cntCandidate].distanceToNearPlayer = distanceToPlayer[PLAYER_2P];
		}
		// �߂��v���C���[�Ƃ̋����̒��ōō��������X�V
		if (candidateList[cntCandidate].distanceToNearPlayer > maxDistance)
		{
			maxDistance = candidateList[cntCandidate].distanceToNearPlayer;
		}
		cntCandidate++;
	}

	// ���̃X�R�A���Z�o���K��l�ȏ�ł���ΑI�����X�g�ɉ�����
	int* selectIndexList = new int[candidateMax];
	int cntSelectIndex = 0;
	for (int i = 0; i < candidateMax; i++)
	{
		candidateList[i].score = fuzzy.grade(candidateList[i].distanceToNearPlayer, 0.0f, maxDistance);
		if (candidateList[i].score > 0.7)
		{
			selectIndexList[cntSelectIndex++] = i;
		}
	}

	// �I�����X�g���烉���_���Ō�⃊�X�g�̃C���f�b�N�X��I�уc���[��I�肷��
	// ���܂����c���[��ID��ԋp����
	int resultIndex = rand() % cntSelectIndex;
	int resultCandidate = selectIndexList[resultIndex];
	int treeID = candidateList[resultCandidate].treeID;
	SAFE_DELETE_ARRAY(selectIndexList);
	SAFE_DELETE_ARRAY(candidateList);
	return treeID;
}


//=============================================================================
// ����W����ɐڒn���W���쐬����
//=============================================================================
D3DXVECTOR3 EventMaker::createGroundedPositionFromPivot(D3DXVECTOR3 pivot, float distance, float reduction)
{
	D3DXVECTOR3 tempDirection;	// ���p
	D3DXVECTOR3 ans;			// ���߂���W
	NavigationMesh* naviMesh = NavigationMesh::getNaviMesh();
	bool isFinish = false;
	float distanceToGround = 0.0f;
	int cnt = 0;

	while (isFinish == false)
	{//������ς��Đڒn���邩�m�F���郋�[�v
		tempDirection = createRandomDirectionXZ();

		while (distance != 1.0f)
		{// �����𓮂����Đڒn���邩�m�F���郋�[�v
			ans = pivot + tempDirection * distance;

			// �ڒn�m�F
			static float RAY_ADDITIONAL_HEIGHT = 20.0f;
			if (naviMesh->isHitGrounding(&distanceToGround, NULL, ans + D3DXVECTOR3(0, RAY_ADDITIONAL_HEIGHT, 0)))
			{
				// Y���W��n�ʂ̍����ɏC��
				if (distanceToGround < RAY_ADDITIONAL_HEIGHT)
				{
					ans.y += (RAY_ADDITIONAL_HEIGHT - distanceToGround);
				}
				else
				{
					ans.y -= (distanceToGround - RAY_ADDITIONAL_HEIGHT);
				}

				isFinish = true;
				break;
			}
			else
			{
				distance -= reduction;
				if (distance < 1.0f) { distance = 1.0f; }
			}
		}
		cnt++;

		if (cnt > 5/*���̉񐔕�����ς�����I������*/)
		{
			isFinish = true;
			ans = pivot;
		}
	}

	return ans;
}


//=============================================================================
// XZ���ʏ�̃x�N�g��(����1.0)�������_���ɍ쐬����
//=============================================================================
D3DXVECTOR3 EventMaker::createRandomDirectionXZ()
{
	// ����
	D3DXVECTOR3 direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	// ��]������
	D3DXQUATERNION quaternion;
	D3DXQuaternionIdentity(&quaternion);
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	D3DXVECTOR3 axisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	anyAxisRotation(&quaternion, axisY, rand() % 361);
	D3DXMatrixRotationQuaternion(&matrix, &quaternion);
	D3DXVec3TransformCoord(&direction, &direction, &matrix);

	return direction;
}