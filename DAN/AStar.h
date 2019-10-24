//-----------------------------------------------------------------------------
// A*�A���S���Y���ɂ��p�X�����N���X [Astar.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/20
//-----------------------------------------------------------------------------
#pragma once

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "Base.h"
#include "MeshData.h"
#include "VertexAccessor.h"

//*****************************************************************************
// �}�N���E�񋓌^��`
//*****************************************************************************
#define NUM_SURROUND_NODE	(7)					// ���Ӄm�[�h�̐�

// �n�`�^�C�v
enum TERRAIN_TYPE
{// �����̏��Ԃ�i���o�����O��DodgeOrbEditer�ɑ����Ȃ���΂����Ȃ�
	CARDINALE_TERRAIN,			// �ԁ@�@�@�@�����g�p
	CIEL_TERRAIN,				// �@�@�@�@�����g�p
	OBSTACLE,					// ��Q��
	NORMAL_TERRAIN = 9			// �����Ȃ�
};


// �}�b�v���W�\����
typedef struct {
	int x;						// �}�b�v�m�[�h��X���W
	int y;						// �}�b�v�m�[�h��Y���W
}MAPVECTOR2;

//// �}�b�v�m�[�h�\����
//typedef struct {
//	NODE_STATUS		status;		// �X�e�[�^�X
//	int				terrain;	// �n�`�^�C�v
//	MAPVECTOR2		mapPos;		// �}�b�v���W
//	D3DXVECTOR3		worldPos;	// ���[���h���W
//}MAPNODE, **MAP;


//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// �o�H�\����
typedef struct
{
	// A*�A���S���Y���ɂ��p�X�����̌��ʂ��i�[����
	MAPVECTOR2				*ptr;				// �o�H���ւ̃|�C���^
	int						numRouteStep;		// �o�H��񓮓I�z��̗v�f���i�ړ����W�̐��j
	int						referenceRec;		// �o�H��񓮓I�z��̎Q�Ƃ��Ă���v�f�ԍ���ۊ�
}ROUTE;




//meshAnalyzerNS::Face* face;


//=============================================================================
// ���O���
//=============================================================================
namespace aStarNS
{
	enum NODE_STATUS
	{
		NODE_NONE,			// ��x���J���Ă��Ȃ�
		NODE_OPEN,			// �I�[�v����
		NODE_CLOSED,		// ��m�[�h�Ƃ��Ďg�p�ς݂ŃN���[�Y
	};
	struct Vertex
	{
		WORD index;
		BYTE* ptr;
	};

	struct FaceNode
	{
		Vertex vtx[3];					
		DWORD adjacency[3];
		int status;
		void resetStatus() { status = NODE_NONE; }
	};

	// �I�[�v�����X�g�\���́@A*�A���S���Y���Ŏg�p
	typedef struct _OPENLIST
	{
		int				key;			// ��L�[
		NODE_STATUS		status;			// �X�e�[�^�X
		int				cost;			// �ړ��R�X�g
		int				heuristic;		// ����R�X�g
		int				score;			// �X�R�A
		DWORD			faceIndex;		// �ʃC���f�b�N�X
#if 0
		MAPVECTOR2		mapPos;			// �}�b�v���W
#endif
		_OPENLIST		*parent;		// �e�m�[�h
		_OPENLIST		*next;			// ���̗v�f�ւ̃|�C���^
	}OPENLIST;

}


//=============================================================================
//�N���X��`
//=============================================================================
class AStar
{
private:
	MeshData* meshData;
	VertexAccessor* vtxAccessor;
	aStarNS::FaceNode *faceNode;

public:
	void initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor);
	void uninitialize();
	// �o�H����
	void pathSearch(DWORD start, DWORD dest);
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitRouteSearch(int type);					// AI�o�H�T���̏���������
void UninitRouteSearch(void);					// AI�o�H�T���̏I������
void RouteSearch(/*int characterNo*/DWORD start, DWORD dest);
// ���Ӄm�[�h�̃}�b�v���W���v�Z
/*MAPVECTOR2*/DWORD CalcSurroundingNodePos(/*MAPVECTOR2 workPos, int position*/ DWORD faceIndex, int i);