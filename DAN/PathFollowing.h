//-----------------------------------------------------------------------------
// �p�X�t�H���[�C���O�N���X [PathFollowing.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/30
//-----------------------------------------------------------------------------
#pragma once
#include "Base.h"
#include "LinkedList.h"
#include "MeshData.h"
#include "VertexAccessor.h"


//=============================================================================
// �N���X��`
//=============================================================================
class PathFollowing: public Base
{
private:
	MeshData* meshData;							// ���b�V�����
	VertexAccessor* vtxAccessor;				// ���_�A�N�Z�X�I�u�W�F�N�g

	// createVector()�̈����Ƃ��Ă��炤
	D3DXVECTOR3* out;							// �ŏI�I�ȏo�̓x�N�g��
	DWORD* faceIndex;							// ���݈ʒu���Ă���|���S���ʃC���f�b�N�X
	D3DXVECTOR3 position;						// ���݈ʒu���Ă�����W
	LinkedList<meshDataNS::Index2>* edgeList;	// �G�b�W���X�g

	// createVector()�̒��Ŏg����
	D3DXVECTOR3 pos1, pos2;
	D3DXVECTOR3 pos3, pos4;
	D3DXVECTOR3 edgeIntersection;				// ��1�G�b�W�i���ɒʉ߂���G�b�W�j�Ƃ̌�_
	D3DXVECTOR3 nextEdgeIntersection;			// ��2�G�b�W�i���̎��ɒʉ߂���G�b�W�j�Ƃ̌�_
	D3DXVECTOR3 vecToNextEdge;					// ��2�G�b�W�ւ̃x�N�g���i���K���ρj
	D3DXVECTOR3 nor;							// ���ݒn�|���S���̖@��
	bool isProtrude;							// �͂ݏo������
	int nearPos;								//
	D3DXVECTOR3 subGoal;						// �T�u�S�[��

	// Method
	void zeroClear()
	{
		MeshData* tempMeshData = meshData;
		VertexAccessor* tempVtxAccessor = vtxAccessor;
		ZeroMemory(this, sizeof(PathFollowing));
		meshData = tempMeshData;
		vtxAccessor = tempVtxAccessor;
	}
	void step1();
	void step2();
	void step3();
	void step4();

public:
	// ������
	void initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor);
	// �o�H�ɉ����ړ��x�N�g�����쐬
	void createVector(D3DXVECTOR3* _out, D3DXVECTOR3 _position,
		DWORD* _faceIndex, LinkedList<meshDataNS::Index2>* _edgeList);
};
