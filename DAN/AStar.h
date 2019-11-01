//-----------------------------------------------------------------------------
// A*�A���S���Y���ɂ��p�X�����N���X [Astar.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include "LinkedList.h"
#include "VertexAccessor.h"
#include "MeshData.h"

//=============================================================================
// ���O���
//=============================================================================
namespace aStarNS
{
	// �m�[�h�X�e�[�^�X
	enum NODE_STATUS
	{
		INVALID,		// �����l�i���X�g�ɖ��ǉ�)
		OPEN,			// �I�[�v����
		CLOSED,			// ��m�[�h�Ƃ��Ďg�p�ς݂ŃN���[�Y
	};

	// A�X�^�[�m�[�h�\����
	struct AStarNode
	{
		DWORD				faceIndex;			// �ʃC���f�b�N�X
		NODE_STATUS			status;				// �X�e�[�^�X
		float				cost;				// �ړ��R�X�g
		float				heuristic;			// ����R�X�g
		float				score;				// �X�R�A
		meshDataNS::Index2	edge;				// �I�[�v������ɂ�����ʉ߂����G�b�W
		D3DXVECTOR3			pos;				// �G�b�W��̒ʉߍ��W�i�R�X�g�v�Z�Ɏg�p�j
		AStarNode			*parent;			// �e�m�[�h
		AStarNode			*next;				// ���̗v�f�ւ̃|�C���^
	};
}


//=============================================================================
//�N���X��`
//=============================================================================
class AStar: public Base
{
private:
	MeshData* meshData;							// ���b�V���f�[�^�I�u�W�F�N�g
	VertexAccessor*	vtxAccessor;				// ���_�A�N�Z�X�I�u�W�F�N�g
	aStarNS::AStarNode*	head;					// �擪�m�[�h
	aStarNS::AStarNode*	work;					// ��ƃm�[�h
	bool isImpasse;								// �܏��H�t���O
	LinkedList<meshDataNS::Index2>*	edgeList;	// �G�b�W���X�g�̃|�C���^

public:
	// ������
	void initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor);
	// �I������
	void uninitialize();
	// �o�H�T��
	LinkedList<meshDataNS::Index2>* pathSearch(DWORD startIndex, DWORD destIndex,
		D3DXVECTOR3 startPos, D3DXVECTOR3 destPos);
	// ���X�g�Ƀm�[�h��ǉ�
	HRESULT addAStarNode(aStarNS::AStarNode *parentNode, aStarNS::AStarNode setData);
	// ���X�g�̉��
	void releaseAStarNode();
	// ���X�g����ł���X�R�A�ȃm�[�h���擾
	aStarNS::AStarNode* getLeastCostNode();
	// ���X�g����ł�����R�X�g�̒Ⴂ�m�[�h���擾
	aStarNS::AStarNode* getLeastHeuristicNode();
	// ���X�g����o�H���擾
	LinkedList<meshDataNS::Index2>* createEdgeList(LinkedList<meshDataNS::Index2>* edgeList, aStarNS::AStarNode *work);
	// ���X�g����m�[�h��T��
	aStarNS::AStarNode* searchNodeByFaceIndex(DWORD searchIndex);
	// ���W����2�̃|���S���̋��E���܂ł̋��������߂�
	float culcDistanceToEdge(float* outLen, D3DXVECTOR3* outPos, D3DXVECTOR3 inPos, DWORD faceInd1, DWORD faceInd2);
};
