//-----------------------------------------------------------------------------
// A*�A���S���Y���ɂ��p�X�����N���X [Astar.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#include "Astar.h"
using namespace aStarNS;


//*****************************************************************************
// �}�N���E�񋓌^��`
//*****************************************************************************
#define INTERVAL_ROUTE_SEARCH	(5)		// �o�H�T���̍X�V�t���[���Ԋu�i�K����j
#define  SURROUNDING_NODE_MAX	(8)		// ���͂̃m�[�h�̐� 

// �}�b�v�m�[�h�̕`��J���[
#define ROUTE_R					(1.0f)	// ��
#define ROUTE_G					(0.7f)	// ��
#define ROUTE_B					(0.0f)	// ��


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
// �I�[�v�����X�g�Ƀm�[�h��ǉ�
HRESULT AddOpenList(OPENLIST **head, OPENLIST *parentNode, OPENLIST setData);

// �I�[�v�����X�g�̉��
void ReleaseOpenList(OPENLIST **head);

// �I�[�v�����X�g����m�[�h��T��
OPENLIST *SearchNodeFromOpenList(OPENLIST **head, int searchKey);

// �I�[�v�����X�g����ł���X�R�A�ȃm�[�h���擾
OPENLIST *GetLeastCostNode(OPENLIST *head);

// �I�[�v�����X�g����ł�����R�X�g�̒Ⴂ�m�[�h���擾
OPENLIST *GetLeastHeuristicNode(OPENLIST *head);

// �I�[�v�����X�g����o�H���擾
void GetPathFromOpenList(OPENLIST *work, int numRouteArray);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
MAPVECTOR2		*route;					// �o�H���ւ̃|�C���^	
int				cntRoute;				// �o�H�̈ړ������J�E���g
static int		mapX, mapY;				// �}�b�v���WXY�ő�l


//=============================================================================
// ����������
//=============================================================================
void AStar::initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor)
{
	meshData = _meshData;
	vtxAccessor = _vtxAccessor;

	// �|���S���ʃm�[�h�z����쐬����
	faceNode = new FaceNode[meshData->getNumFaces()];
	for (int i = 0; i < meshData->getNumFaces(); i++)
	{
		for (int k = 0; k < 3; k++)
		{
			faceNode->vtx[k].index = meshData->getVertexIndexPointerFromFaceIndex(
				meshData->getIndexArray(), i)[k];
			faceNode->vtx[k].ptr = meshData->getVertexPointerFromVertexIndex(
				meshData->getVtxArray(), faceNode->vtx[k].index);
		}
		for (int k = 0; k < 3; k++)
		{
			faceNode->adjacency[k] = *meshData->getAdjacentFaceIndexPointerFromFaceIndex(
				meshData->getAdjacencyArray(), i + k);
		}
		faceNode[i].resetStatus();
	}

#if 1
	setDataDirectory();
	FILE* fp;
	fp = fopen("FaceNodeArray.txt", "w");
	fprintf(fp, "�|���S���ʃm�[�h�z��̒��g\n�o�͌��FAStar.cpp\n");

	for (int i = 0; i < meshData->getNumFaces(); i++)
	{
		fprintf(fp, "�y�|���S��Index%d�z\n" ,i);
		for (int k = 0; k < 3; k++)
		{
			D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
			//D3DXVECTOR3* position = (D3DXVECTOR3*)vtxAccessor->getPointer(
			//	vtxAccess::POSITION, &meshData->getVtxArray()[i * 3 * meshData->getStride() + k * meshData->getStride()]);
			fprintf(fp, "���_%d Ind�F%d�@Pos(%f, %f, %f)\n", k, faceNode[i].vtx[k].index, position.x, position.y, position.z);
		}
	}

	fclose(fp);
#endif

#if 0
	int enemyMax = GetNumberEnemy();

	// �o�H�E�ړI�n���̏�����
	for (int i = 0; i < enemyMax; i++)
	{
		AI_MEMORY *memory = GetAiMemory(i);
		for (int routeType = 0; routeType < ROUTE_TYPE_MAX; routeType++)
		{
			memory->route[routeType].ptr = NULL;
			memory->route[routeType].numRouteStep = 0;
			memory->route[routeType].referenceRec = 0;
			memory->destination[routeType] = NULL;
		}
	}

	// �}�b�v�T�C�Y��ݒ�
	GAMEDATA *gameData = GetGameData();	
	mapX = gameData->mapX;
	mapY = gameData->mapY;
#endif
}


//=============================================================================
// �I������
//=============================================================================
void AStar::uninitialize(void)
{
	SAFE_DELETE_ARRAY(faceNode);

#if 0
	int enemyMax = GetNumberEnemy();

	for (int i = 0; i < enemyMax; i++)
	{
		AI_MEMORY *memory = GetAiMemory(i);

		// �o�H��񃁃����̉��
		for (int routeType = 0; routeType < ROUTE_TYPE_MAX; routeType++)
		{
			free(memory->route[routeType].ptr);
			memory->route[routeType].ptr = NULL;
		}
	}
#endif
}


//=============================================================================
// AI�o�H�T������
//=============================================================================
void AStar::pathSearch(/*int characterNo*/DWORD start, DWORD dest)
{
#if 0
	// �o�H�T���̊Ԋu����
	static DWORD cntSearch = 0;
	cntSearch++;
	if (cntSearch % INTERVAL_ROUTE_SEARCH)
	{// INTERVA_ROUTE_SEARCH����łȂ��ƁA
	 // NPC_VS_NPC�̂Ƃ��Ј���̌o�H�T�����X�V����Ȃ��Ȃ�
		return;
	}


	///////////////////////////////////////////////////////////////////////////
	//////////////////////////// �T���o�H�̌��� ///////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CHARACTER *enemy = GetCharacter(characterNo);	// �G�l�~�[���擾
	AI_MEMORY *memory = GetAiMemory(enemy->aiNo);	// �L���̈���擾
	MAP map = GetMap();								// �}�b�v���擾

	// �����̃}�b�v���W���擾
	MAPVECTOR2 myMapPos = TransrateWorldPosToMapPos(enemy->transform->pos);

	/* �o�H�T���Z���N�^�[ */
	// �eNPCAI���ƂɁA���[�v���A�ړI�n�m�[�h�𑊎���W�A�I�[�u���W�A�A�C�e�����W�ƕς���
	// �eNPCAI�͂��̂R�̌o�H�����펞�ێ����A�X�V�𑱂���

	// ���t���[���łǂ̌o�H��T�����邩������
	memory->searchSelecter++;
	memory->searchSelecter = memory->searchSelecter % 3;
	ROUTE_TYPE routeType = (ROUTE_TYPE)memory->searchSelecter;

	// �ړI�n�̃m�[�h��ݒ肷��i��ɑ���AI���W���[���Ń|�C���^�ɒ��g�����Ă����j
	MAPNODE* destinationNode = memory->destination[routeType];

	// �ړI�n���ݒ肳��Ă��Ȃ���ΏI��
	if (destinationNode == NULL)
	{
		return;
	}

	if (routeType == ROUTE_TO_ORB)
	{
		routeType = ROUTE_TO_ORB;
	}

#endif

	///////////////////////////////////////////////////////////////////////////
	///////////////////////// A�X�^�[�A���S���Y�� /////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	// �������炦
	OPENLIST *head = NULL;							// �I�[�v�����X�g�̐擪
	BOOL isBlindAlley = FALSE;						// �܏��H�ł͂Ȃ�

	// ��m�[�h�ɊJ�n�m�[�h��ݒ肷��
	OPENLIST *work = NULL;
	OPENLIST data;
	data.cost = 0;									// �J�n�n�_������0
	data.heuristic = 0;								// �J�n�n�_������0
	data.score = 0;									// �J�n�n�_������0
	data.faceIndex = start;
#if 0
	data.mapPos = myMapPos;							// �����̃}�b�v���W
#endif
	data.parent = NULL;								// �J�n�n�_������NULL
	data.next = NULL;								// �J�n�n�_������NULL

	// �I�[�v�����X�g�ɊJ�n�m�[�h��ǉ�����
	AddOpenList(&head, work, data);

	//--------------
	// ���C�����[�v
	//--------------
	while (head != NULL)
	{
		// �I�[�v�����X�g�̒��ōł���R�X�g�ł���m�[�h����m�[�h�ɐݒ�
		work = GetLeastCostNode(head);

		if (work == NULL)
		{// �܏��H�Ɋׂ��Ă���Ȃ烋�[�v����o��
			isBlindAlley = TRUE;
			break;
		}

		// ��m�[�h���ړI�n�̃m�[�h�ł���ꍇ
		if (work->faceIndex == dest)
		{
			break;	// �o�H�̊���
		}

		// ��m�[�h���ړI�n�̃m�[�h�łȂ��ꍇ
		else
		{
			// ��m�[�h�̎�����I�[�v�����Ă���
			for (int i = 0; i < 3/*SURROUNDING_NODE_MAX*/; i++)
			{
				// �I�[�v������m�[�h�̃}�b�v���W���v�Z
				/*MAPVECTOR2*/DWORD ans = CalcSurroundingNodePos(work->faceIndex, i);

				if (/*pos.x*/ans == -1)
				{// ���W�����݂��Ȃ�
					continue;
				}
#if 0
				if ((map[pos.y][pos.x].status == NODE_OPEN
						|| map[pos.y][pos.x].status == NODE_CLOSED
						|| map[pos.y][pos.x].terrain == OBSTACLE)
					/*&& MapPosEqual(pos, destinationNode[routeType].mapPos) == FALSE*/)
				{// �I�[�v���Ώۂł��ړI�n�ł��Ȃ�
					continue;
				}
#endif
				if (faceNode[ans].status == (NODE_OPEN || NODE_CLOSED))
				{
					continue;
				}

				// �����Ńm�[�h���I�[�v������
#if 0
				map[pos.y][pos.x].status = NODE_OPEN;					// �}�b�v��
#endif
				faceNode[ans].status = NODE_OPEN;

				data.status = NODE_OPEN;								// �I�[�v�����X�g��


				// �I�[�v�����X�g�̑��̃f�[�^�쐬
				// �ړ��R�X�g
				data.cost = work->cost + 1;	
#if 0
				int differenceX, differenceY;
				differenceX = abs(destinationNode->mapPos.x - pos.x);
				differenceY = abs(destinationNode->mapPos.y - pos.y);
				// ����R�X�g:�c���̍ő�l�ɏc���̍��v�������邱�ƂŁA���t���[���������ړI�n�����Ɍo�H���Ƃ�悤�ɂȂ�
				data.heuristic = max(differenceX, differenceY) + differenceX + differenceY;
				data.score = data.cost + data.heuristic;				// �X�R�A
				data.mapPos = pos;										// ���W
#endif

				// ����̎O�p�`�̏d�S�����߂�
				// �e�ӂɐ��������낵���̋�����ۊǁ@�� �R�X�g
				// ���̊e�ӂƐ����̌�_����A�ړI�n�̎O�p�`�d�S�܂ł̋����𑪂�ۊǁ��q���[���X�e�B�b�N
				data.heuristic = 0;
				data.faceIndex = ans;

				// �ȏ���I�[�v�����X�g�ɒǉ�
				AddOpenList(&head, work, data);
			}

#if 0			// ��m�[�h���N���[�Y
			map[work->mapPos.y][work->mapPos.x].status = NODE_CLOSED;	// �}�b�v��
			work->status = NODE_CLOSED;									// �I�[�v�����X�g��
#endif
			faceNode[work->faceIndex].status = NODE_CLOSED;
			work->status = NODE_CLOSED;
		}
	}

#if 0
	// �}�b�v�m�[�h�̃X�e�[�^�X��������
	for (int y = 0; y < mapY; y++)
	{
		for (int x = 0; x < mapX; x++)
		{
			map[y][x].status = NODE_NONE;			// �I�[�v����N���[�Y�h�����ׂ�NONE�ɕ��A
		}
	}
#endif
	for (int i = 0; i < 1000; i++)
	{
		faceNode[i].status = NODE_NONE;
	}


	// �܏��H�̏ꍇ�ł�����R�X�g�̒Ⴂ�m�[�h��ړI�n�Ƃ��Đݒ肷��
	if (isBlindAlley)
	{
		work = GetLeastHeuristicNode(head);
	}

#if 0
	// �^�C���J���[�̃��Z�b�g
	if (memory->searchSelecter == memory->moveRouteType)
	{
		ResetColorTile();
	}
#endif

	//// �o�H���i���I�z��j�̃��������m��
	//if ((route = (MAPVECTOR2*)malloc(sizeof(MAPVECTOR2) * work->cost)) == NULL)
	//{
	//	MessageBox(NULL, TEXT("�������̊m�ۂɎ��s���܂����B\n�A�v���P�[�V�������I�����܂��B"), TEXT("SystemError"), MB_OK);
	//	PostQuitMessage(0);
	//}


	//// �I�[�v�����X�g����o�H���Ɍo�H��ǉ�
	//GetPathFromOpenList(work, work->cost);


	//// AI�L���̈�ɃX�g�A
	//free(memory->route[routeType].ptr);					// ���̃����������
	//memory->route[routeType].ptr = route;				// �|�C���^�Đݒ�
	//memory->route[routeType].numRouteStep = work->cost;	// �o�H���̗v�f�����L�^
	//memory->route[routeType].referenceRec = 0;			// �Q�ƋL�^��������
	//route = NULL;

	//// �I�[�v�����X�g�����
	//ReleaseOpenList(&head);
}


//=============================================================================
// �I�[�v�����X�g�Ƀm�[�h��ǉ�
//=============================================================================
HRESULT AddOpenList(OPENLIST **head, OPENLIST *parentNode, OPENLIST setData)
{
	OPENLIST *p;

	// �L���̈�̊m��
	if ((p = (OPENLIST*)malloc(sizeof(OPENLIST))) == NULL)
	{
		MessageBox(NULL, TEXT("�������̊m�ۂɎ��s���܂����B\n�A�v���P�[�V�������I�����܂��B"), TEXT("SystemError"), MB_OK);
		PostQuitMessage(0);
	}

	*p = setData;			// �f�[�^��o�^
	p->parent = parentNode;	// �e�m�[�h��o�^

	// �|�C���^�̂Ȃ�����
	p->next = *head;		// ���܂ł̐擪�|�C���^��e�|�C���^�ɐݒ�
	*head = p;				// �V���ȗ̈��擪�|�C���^�ɐݒ�

	return S_OK;
}


//=============================================================================
// �I�[�v�����X�g�̉��
//=============================================================================
void ReleaseOpenList(OPENLIST **head)
{
	OPENLIST *p;

	// �m�ۂ��Ă����S���������������
	// ���|�C���^��NULL�܂ŏ���
	while (*head != NULL)
	{
		p = (*head)->next;
		free(*head);
		*head = p;
	}
}


//=============================================================================
// �I�[�v�����X�g����m�[�h��T��
//=============================================================================
OPENLIST *SearchNodeFromOpenList(OPENLIST **head, int searchKey)
{
	OPENLIST *p = *head;		// ���X�g�̐擪���w��

	// ���X�g�̐擪��serchKey���������ꍇ
	if (p != NULL && p->key == searchKey)
	{
		return p;
	}

	// ���X�g�̐擪��NULL�Ȃ�I��
	if (p == NULL)
	{
		return NULL;
	}

	// searchKey��������
	while (p->next != NULL && p->next->key != searchKey)
	{
		p = p->next;
	}

	if (p->next == NULL)
	{
		return NULL;
	}

	return p->next;
}


//=============================================================================
// �I�[�v�����X�g����ł���X�R�A�ȃm�[�h���擾
//=============================================================================
OPENLIST *GetLeastCostNode(OPENLIST *head)
{
	OPENLIST *p = head;				// ���X�g�̐擪���w��
	OPENLIST *leastScoreNode = p;	// �Œ�R�X�g�̃m�[�h
	int leastScore = INT_MAX;		// �Œ�R�X�g
	BOOL isEnterLoop = FALSE;		// while���[�v�ɓ����Ă��Ȃ�

	// ���X�g�����܂Ńm�[�h��T��
	while (p->next != NULL)
	{
		isEnterLoop = TRUE;			// while���[�v�ɓ�����

		if (p->score < leastScore
			&& p->status == NODE_OPEN)
		{// �R�X�g����菬��������x����m�[�h�ɂ��Ă��Ȃ������ŒT��

			leastScore = p->score;
			leastScoreNode = p;
		}

		p = p->next;				// ���̗v�f�ɑ{���Ώۂ�ύX
	}

	if (isEnterLoop == TRUE && leastScore == INT_MAX)
	{// ���X�g���ɃI�\�v���m�[�h���Ȃ��ꍇ�i�܏��H��ԁj
		return NULL;
	}

	return leastScoreNode;
}


//=============================================================================
// �I�[�v�����X�g����ł�����R�X�g�̒Ⴂ�m�[�h���擾
//=============================================================================
OPENLIST *GetLeastHeuristicNode(OPENLIST *head)
{
	OPENLIST *p = head;					// ���X�g�̐擪���w��
	OPENLIST *leastHeuristicNode = p;	// �Œ�R�X�g�̃m�[�h
	int leastHeuristic = INT_MAX;		// �Œ�R�X�g

	// ���X�g�����܂Ńm�[�h��T��
	while (p->next != NULL)
	{
		if (p->heuristic <= leastHeuristic)
		{// ����R�X�g�����̍Œ�R�X�g�ȉ��ł���Ȃ�
			leastHeuristic = p->heuristic;

			if (p->cost <= leastHeuristicNode->cost)
			{// �ړ��R�X�g�����̍Œ�R�X�g�m�[�h�ȉ��ł����
				leastHeuristicNode = p;
			}// ����œ�������a���Ȃ��Ȃ�
		}

		p = p->next;					// ���̗v�f�ɑ{���Ώۂ�ύX
	}

	return leastHeuristicNode;
}



//=============================================================================
// �I�[�v�����X�g����o�H���擾
//=============================================================================
void GetPathFromOpenList(OPENLIST *work, int numRouteArray)
{
	if (numRouteArray > 0)
	{// �o�H���z��̌�납�珇�ɍ��W�������Ă���
#if 0
		route[numRouteArray - 1] = work->mapPos;
#endif
		//route[numRouteArray - 1] = work->faceIndex;
		numRouteArray--;
#if 0
		// �ړ���܂ł̃^�C���ɐF��`��
		AI_MEMORY *memory = GetAiMemory(0);
		if (memory->searchSelecter == memory->moveRouteType)
		{
			SetColorTile(work->mapPos.x, work->mapPos.y, D3DXCOLOR(ROUTE_R, ROUTE_G, ROUTE_B, 1.0f));
		}
#endif

	}

	// �e�|�C���^��NULL�܂ōċA����
	if (work->parent != NULL)
	{
		GetPathFromOpenList(work->parent, numRouteArray);
	}
}


//=============================================================================
// ���Ӄm�[�h�̃}�b�v���W���v�Z
//=============================================================================
DWORD CalcSurroundingNodePos(/*MAPVECTOR2 workPos, int position*/ DWORD faceIndex, int i)
{
#if 0
	MAPVECTOR2 ans;	
	GAMEDATA *gameData = GetGameData();

	switch (position)
	{
	case 0:	// ����
		ans.x = workPos.x - 1;
		ans.y = workPos.y - 1;
		break;

	case 1:	// ��
		ans.x = workPos.x;
		ans.y = workPos.y - 1;
		break;

	case 2:	// �E��
		ans.x = workPos.x + 1;
		ans.y = workPos.y - 1;
		break;

	case 3:	// ��
		ans.x = workPos.x - 1;
		ans.y = workPos.y;
		break;

	case 4:	// �E
		ans.x = workPos.x + 1;
		ans.y = workPos.y;
		break;

	case 5:	// ����
		ans.x = workPos.x - 1;
		ans.y = workPos.y + 1;
		break;

	case 6:	// ��
		ans.x = workPos.x;
		ans.y = workPos.y + 1;
		break;

	case 7:	// �E��
		ans.x = workPos.x + 1;
		ans.y = workPos.y + 1;
		break;
	}

	if ((ans.x < 0 || ans.x >= gameData->mapX)
		|| (ans.y < 0 || ans.y >= gameData->mapY))
	{// ���݂��Ȃ����W�̏ꍇ
		ans.x = -1;
		ans.y = -1;
	}
	
	return ans;
#endif
 
	return 0;//  faceNode[faceIndex].adjacency[i];
}