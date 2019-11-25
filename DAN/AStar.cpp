//-----------------------------------------------------------------------------
// A*�A���S���Y���ɂ��p�X�����N���X [Astar.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#include "Astar.h"
using namespace meshDataNS;
using namespace aStarNS;


//=============================================================================
// ����������
//=============================================================================
void AStar::initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor)
{
	meshData = _meshData;
	vtxAccessor = _vtxAccessor;
}


//=============================================================================
// �I������
//=============================================================================
void AStar::uninitialize(void)
{

}


//=============================================================================
// �o�H�T��
//[����1] DWORD start�@�J�n�|���S���ʃC���f�b�N�X
//[����2] DWORD dest �@�ړI�n�|���S���ʃC���f�b�N�X
//[����3] DWORD start�@�J�n���W
//[����4] DWORD start�@�ړI�n���W
//=============================================================================
LinkedList<meshDataNS::Index2>* AStar::pathSearch(
	DWORD startIndex, DWORD destIndex, D3DXVECTOR3 startPos, D3DXVECTOR3 destPos)
{
	releaseAStarNode();		// A*�m�[�h���X�g�����
	head = NULL;			// �擪�m�[�h������
	work = NULL;			// ��ƃm�[�h������
	isImpasse = false;		// �܏��H�t���O������

	// �J�n�m�[�h�����X�g�ɒǉ�
	AStarNode startNode;
	startNode.faceIndex = startIndex;
	startNode.status = INVALID;
	startNode.cost = startNode.heuristic = startNode.score = 0.0f;
	startNode.edge.index[0] = startNode.edge.index[1] = -1;
	startNode.pos = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	startNode.parent = startNode.next = NULL;
	addAStarNode(work, startNode);

	// ���C�����[�v
	while (head != NULL)
	{
		// ���X�g�̒��ōł���X�R�A�̃m�[�h����m�[�h�ɐݒ�
		work = getLeastCostNode();

		// �o�H���܏��H�������͖ړI�n�m�[�h�܂ŒT������
		if (work == NULL || work->faceIndex == destIndex)
		{
			if(work == NULL) isImpasse = true;
			break;
		}

		// ��m�[�h�̎�����I�[�v��
		for (int i = 0; i < 3; i++)
		{
			DWORD adjacentIndex = meshData->getFaceArray()[work->faceIndex].adjacency[i];
			if (adjacentIndex == -1){ continue; }								// �אږʂ��Ȃ�
			if (searchNodeByFaceIndex(adjacentIndex) != NULL) { continue; }		// ���X�g�Ɋ��o

			// �V�����m�[�h�����X�g�ɒǉ�
			AStarNode data;
			data.faceIndex = adjacentIndex;
			data.status = OPEN;
			float len;
			culcDistanceToEdge(&len, &data.pos, work->pos, work->faceIndex, adjacentIndex);
			data.cost = work->cost + len;
			data.heuristic = D3DXVec3Length(&(destPos - data.pos));
			data.score = data.cost + data.score;
			data.edge = meshData->getSharingVertexIndexFromTwoFace(work->faceIndex, data.faceIndex);
			addAStarNode(work, data);
		}

		// ��m�[�h���N���[�Y
		work->status = CLOSED;
	}

	// �܏��H�̏ꍇ�ł�����R�X�g�̒Ⴂ�m�[�h��ړI�n�Ƃ��Đݒ肷��
	if (isImpasse)
	{
		//work = getLeastHeuristicNode();
		return NULL;
	}

	// �G�b�W���X�g���쐬���ԋp����
	return createEdgeList(edgeList = new LinkedList<meshDataNS::Index2>, work);
}


//=============================================================================
// ���X�g�Ƀm�[�h��ǉ�
//=============================================================================
HRESULT AStar::addAStarNode(AStarNode *parentNode, AStarNode setData)
{
	AStarNode *p;
	if ((p = (AStarNode*)malloc(sizeof(AStarNode))) == NULL)
	{
		MessageBox(NULL, TEXT("�������̊m�ۂɎ��s���܂����B\n�A�v���P�[�V�������I�����܂��B"), TEXT("SystemError"), MB_OK);
		PostQuitMessage(0);
	}

	*p = setData;			// �f�[�^��o�^
	p->parent = parentNode;	// �e�m�[�h��o�^
	p->next = head;
	head = p;
	return S_OK;
}


//=============================================================================
// ���X�g�̉��
//=============================================================================
void AStar::releaseAStarNode()
{
	// �m�ۂ��Ă����S���������������
	AStarNode *p;
	while (head != NULL)
	{
		p = head->next;
		free(head);
		head = p;
	}
}


//=============================================================================
// ���X�g����ł���X�R�A�ȃm�[�h���擾
//=============================================================================
AStarNode* AStar::getLeastCostNode()
{
	AStarNode *p = head;			// ���X�g�̐擪���w��
	AStarNode *leastScoreNode = p;	// �Œ�X�R�A�̃m�[�h
	float leastScore = FLT_MAX;		// �Œ�X�R�A
	bool isEnterLoop = false;		// while���[�v�ɓ����Ă��Ȃ�

	// ���X�g�����܂Ńm�[�h��T��
	while (p->next != NULL)
	{
		isEnterLoop = true;			// while���[�v�ɓ�����

		if (p->score < leastScore
			&& p->status == OPEN)
		{// �X�R�A����菬��������x����m�[�h�ɂ��Ă��Ȃ������ŒT��

			leastScore = p->score;
			leastScoreNode = p;
		}

		p = p->next;				// ���̗v�f�ɑ{���Ώۂ�ύX
	}

	if (isEnterLoop == true && leastScore == FLT_MAX)
	{// ���X�g���ɃI�\�v���m�[�h���Ȃ��ꍇ�i�܏��H��ԁj
		return NULL;
	}

	return leastScoreNode;
}


//=============================================================================
// ���X�g����ł�����R�X�g�̒Ⴂ�m�[�h���擾
//=============================================================================
AStarNode* AStar::getLeastHeuristicNode()
{
	AStarNode *p = head;					// ���X�g�̐擪���w��
	AStarNode *leastHeuristicNode = p;	// �Œᐄ��R�X�g�̃m�[�h
	float leastHeuristic = FLT_MAX;		// �Œᐄ��R�X�g

	// ���X�g�����܂Ńm�[�h��T��
	while (p->next != NULL)
	{
		if (p->heuristic <= leastHeuristic)
		{// ����R�X�g�����̍Œ�R�X�g�ȉ��ł���Ȃ�
			leastHeuristic = p->heuristic;

			if (p->cost <= leastHeuristicNode->cost)
			{// �ړ��R�X�g�����̍Œ�R�X�g�m�[�h�ȉ��ł����// ��
				leastHeuristicNode = p;
			}// ����œ�������a���Ȃ��Ȃ�
		}

		p = p->next;					// ���̗v�f�ɑ{���Ώۂ�ύX
	}

	return leastHeuristicNode;
}


//=============================================================================
// ���X�g����m�[�h��T��
//=============================================================================
AStarNode* AStar::searchNodeByFaceIndex(DWORD searchIndex)
{
	AStarNode* p = head;		// ���X�g�̐擪���w��

	// ���X�g�̐擪��searchIndex���������ꍇ
	if (p != NULL && p->faceIndex == searchIndex)
	{
		return p;
	}

	// ���X�g�̐擪��NULL�Ȃ�I��
	if (p == NULL)
	{
		return NULL;
	}

	// searchIndex��������
	while (p->next != NULL && p->next->faceIndex != searchIndex)
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
// ���X�g����o�H���擾
//=============================================================================
LinkedList<meshDataNS::Index2>* AStar::createEdgeList(
	LinkedList<meshDataNS::Index2>* edgeList, aStarNS::AStarNode *work)
{
	if (work->parent != NULL)
	{
		edgeList->insertFront(work->edge);
		createEdgeList(edgeList, work->parent);
	}

	return edgeList;
}


//=============================================================================
// ���W����2�̃|���S���̋��E���܂ł̋��������߂�
//=============================================================================
float AStar::culcDistanceToEdge(float* outLen, D3DXVECTOR3* outPos, D3DXVECTOR3 inPos, DWORD faceInd1, DWORD faceInd2)
{
	// ���L���Ă��钸�_�C���f�b�N�X���擾
	Index2 sharing = meshData->getSharingVertexIndexFromTwoFace(faceInd1, faceInd2);
	if (sharing.index[0] == -1 || sharing.index[1] == -1) return -1;// �ʂ��ڂ��Ă��Ȃ�

	// �C���f�b�N�X�̒��_���W���擾
	D3DXVECTOR3* pos1 = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(sharing.index[0]));
	D3DXVECTOR3* pos2 = (D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(sharing.index[1]));

	// ���͍��W�Ɩʋ��E���̌�_�����߂�
	D3DXVECTOR3 nearestPoint = nearestPointOnLine(*pos1, *pos2, inPos);
	if(outPos != NULL) *outPos = nearestPoint;

	// ��_�܂ł̋��������߂�
	float ans;
	if (nearestPoint == *pos1)		ans = D3DXVec3Length(&(*pos1 - inPos));
	else if(nearestPoint == *pos2)	ans = D3DXVec3Length(&(*pos2 - inPos));
	else							ans = D3DXVec3Length(&(nearestPoint - inPos));
	if(outLen != NULL) *outLen = ans;
	return ans;
}
