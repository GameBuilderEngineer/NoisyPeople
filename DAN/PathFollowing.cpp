//-----------------------------------------------------------------------------
// �p�X�t�H���[�C���O�N���X [PathFollowing.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/30
//-----------------------------------------------------------------------------
#include "PathFollowing.h"
using namespace meshDataNS;


//=============================================================================
// ������
//=============================================================================
void PathFollowing::initialize(MeshData* _meshData, VertexAccessor* _vtxAccessor)
{
	meshData = _meshData;
	vtxAccessor = _vtxAccessor;
}


//=============================================================================
// �o�H�ɉ����ړ��x�N�g�����쐬
//=============================================================================
void PathFollowing::createVector(
	D3DXVECTOR3* _out,
	D3DXVECTOR3 _position,
	DWORD* _faceIndex,
	LinkedList<meshDataNS::Index2>* _edgeList)
{
	// �|�C���^�ȊO��S�ă[���N���A
	zeroClear();

	// �������Z�b�g
	out = _out;
	position = _position;
	faceIndex = _faceIndex;
	edgeList = _edgeList;

	if ((edgeList->nodeNum == 0))
	{
		return;
	}

	step1();		// [1]��1�G�b�W�̒��_���W���擾���ăG�b�W�ƌ��ݒn���W�̌�_�����߂�

	if (edgeList->nodeNum == 1)
	{// �c��̃G�b�W��1�����̏ꍇ
		subGoal = edgeIntersection;
	}
	else
	{// �c��̃G�b�W��2�ȏ�̏ꍇ
		step2();	// [2]��2�G�b�W�̒��_���W���擾���ăG�b�W�ƌ��ݒn���W�̌�_�����߂�
		step3();	// [3]��2�G�b�W�ւ̃x�N�g�������ݒn�|���S���ʂƕ��s�ɂȂ�悤�X���b�v�����Ă���
		step4();	// [4]�T�u�S�[�����W�����߂�
	}

	// �x�N�g�����쐬���ďI��
	*out = (subGoal - _position);
	D3DXVec3Normalize(out, out);
}

// [1]��1�G�b�W�̒��_���W���擾���ăG�b�W�ƌ��ݒn���W�̌�_�����߂�
void PathFollowing::step1()
{
	Index2 edge = *edgeList->getValue(0);
	pos1 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[0]));
	pos2 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[1]));
	edgeIntersection = nearestPointOnLine(pos1, pos2, position);
}

// [2]��2�G�b�W�̒��_���W���擾���ăG�b�W�ƌ��ݒn���W�̌�_�����߂�
void PathFollowing::step2()
{
	Index2 edge = *edgeList->getValue(1);
	pos3 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[0]));
	pos4 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[1]));
	nextEdgeIntersection = nearestPointOnLine(pos3, pos4, position);
}

// [3]��2�G�b�W�ւ̃x�N�g�������ݒn�|���S���ʂƕ��s�ɂȂ�悤�X���b�v�����Ă���
void PathFollowing::step3()
{
	nor = meshData->getFaceArray()[*faceIndex].nor;
	vecToNextEdge = nextEdgeIntersection - position;
	D3DXVec3Normalize(&vecToNextEdge, &vecToNextEdge);
	slip(vecToNextEdge, nor);
	slip(vecToNextEdge, -nor);
}

// [4] �T�u�S�[�����W�����߂�
void PathFollowing::step4()
{
	// ���ʂ̕������ő�1�G�b�W��ɕ��ʂ�����
	// ��2�G�b�W�ւ̃x�N�g���Ƒ�1�G�b�W�̌�_�����߂���
	// (1)���ʂ̖@��������
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &nor, &(pos2 - pos1));
	if (cross = D3DXVECTOR3(0.0f, 0.0, 0.0f))
	{// position, pos1, pos2�����꒼���ゾ�����ꍇ(����ɒႢ�\������)�O�ς������̂���
		subGoal = position;	// ���̃t���[����createVector()�̓x�N�g��(0, 0, 0)��Ԃ�
	}
	D3DXVec3Normalize(&cross, &cross);
	D3DXVECTOR3 temp = edgeIntersection + cross;
	if (D3DXVec3LengthSq(&(temp - position)) > D3DXVec3LengthSq(&(edgeIntersection - position)))
	{// ���ʂ̖@�������ł�position���Ɍ�������
		cross = -cross;
	}

	// (2)���ʂ̕������ɑ�����Č�_�܂ł̋��������߂�
	float d = -(cross.x * edgeIntersection.x
		+ cross.y * edgeIntersection.y
		+ cross.z * edgeIntersection.z);

	float distance = cross.x * position.x
		+ cross.y * position.y
		+ cross.z * position.z
		+ d;

	// (3) ��_�����߂�
	D3DXVECTOR3 intersectionOfEdgeAndNextEdge = position + vecToNextEdge * distance;

	// ����W�Ƒ�1�G�b�W�Ƃ̌�_�̒��ԓ_���T�u�S�[���Ƃ���
	subGoal = (intersectionOfEdgeAndNextEdge + edgeIntersection) / 2.0f;
}


// �S�~

//float firstEdgeRadian;
//float radianContainingSecondEdge;
//
//// ��2�G�b�W�Ƃ̌�_����1�G�b�W�̂ǂ���̒��_�ɋ߂��������߂�
//float tempLen1 = D3DXVec3LengthSq(&(nextEdgeIntersection - pos1));
//float tempLen2 = D3DXVec3LengthSq(&(nextEdgeIntersection - pos2));
//if (tempLen1 < tempLen2)
//{
//	nearPos = 1;
//}
//else
//{
//	nearPos = 2;
//}
//
////�u��1�G�b�W�̊e���_�̌��ݒn���W�v�̂Ȃ��p�x
//formedRadianAngle(&firstEdgeRadian, pos1 - position, pos2 - position);
//
////�u��1�G�b�W�̌��ݒn���W�ɋ߂����_�̌��ݒn���W�v�ƁunextEdgeIntersection�̌��ݒn���W�v�̂Ȃ��p
//if (nearPos == 1)
//{
//	if (false == formedRadianAngle(&radianContainingSecondEdge, pos2 - position, vecToNextEdge))
//		return;
//}
//else// nearPos == 2
//{
//	if (false == formedRadianAngle(&radianContainingSecondEdge, pos1 - position, vecToNextEdge))
//		return;
//}
//
//// �͂ݏo�������m
//if (firstEdgeRadian < radianContainingSecondEdge)
//{
//	isProtrude = true;
//}


//// �͂ݏo��������Α�2�G�b�W�Ƃ̌�_�Ƌ߂���1�G�b�W�̒��_���W����Ƃ���
//if (isProtrude)
//{
//	if (nearPos == 1)		work = pos1;
//	else if (nearPos == 2)	work = pos2;
//}