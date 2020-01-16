//-----------------------------------------------------------------------------
// �p�X�t�H���[�C���O�N���X [PathFollowing.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/30
//-----------------------------------------------------------------------------
#include "PathFollowing.h"
using namespace meshDataNS;

#define SQUARE_FOLLOW

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
// ��[memo]�c��̃G�b�W��2�ȏ゠��ꍇ��NavigationMesh::steering()���炱�̊֐��ɓ���
void PathFollowing::createVector(
	D3DXVECTOR3* _out,
	D3DXVECTOR3 _position,
	DWORD* _faceIndex,
	LinkedList<meshDataNS::Index2>* _edgeList)
{
#ifdef SQUARE_FOLLOW 

	// �|�C���^�ȊO��S�ă[���N���A
	zeroClear();

	// �������Z�b�g
	out = _out;
	position = _position;
	faceIndex = _faceIndex;
	edgeList = _edgeList;

	step1();		// [1]��1�G�b�W�̒��_���W���擾���ăG�b�W�ƌ��ݒn���W�̌�_�����߂�
	if (edgeList->nodeNum < 4)
	{// �c��̃G�b�W��3�ȉ��̏ꍇ
		subGoal = edgeIntersection;
	}
	else
	{
		step2();	// [2]��2�G�b�W�̒��_���W���擾���ăG�b�W�ƌ��ݒn���W�̌�_�����߂�
		step3();	// [3]��2�G�b�W�ւ̃x�N�g�������ݒn�|���S���ʂƕ��s�ɂȂ�悤�X���b�v�����Ă���
		step4();	// [4]�T�u�S�[�����W�����߂�
	}

	// �x�N�g�����쐬���ďI��
	*out = (subGoal - _position);
	D3DXVec3Normalize(out, out);
#else
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
#endif
}
#ifdef SQUARE_FOLLOW 
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
	Index2 edge = *edgeList->getValue(2);
	pos3 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[0]));
	pos4 = *(D3DXVECTOR3*)vtxAccessor->getPointer(vtxAccess::POSITION,
		meshData->getVertexPointerFromVertexIndex(edge.index[1]));
	nextEdgeIntersection = nearestPointOnLine(pos3, pos4, position);
}

// [3]�x�N�g�������ݒn�|���S���ʂƕ��s�ɂȂ�悤�X���b�v�����Ă���
void PathFollowing::step3()
{
	nor = meshData->getFaceArray()[*faceIndex].nor;
	vecToEdge = edgeIntersection - position;
	vecToNextEdge = nextEdgeIntersection - position;
	D3DXVec3Normalize(&vecToEdge, &vecToEdge);
	D3DXVec3Normalize(&vecToNextEdge, &vecToNextEdge);
	vecToEdge = slip(vecToEdge, nor);
	vecToEdge = slip(vecToEdge, -nor);
	vecToNextEdge = slip(vecToNextEdge, nor);
	vecToNextEdge = slip(vecToNextEdge, -nor);
}

// [4] �T�u�S�[�����W�����߂�
void PathFollowing::step4()
{
	subGoal = (pos1 + pos2) / 2;

	//D3DXVECTOR3 tempIntersection;

	////--------------------------------------------------------------------------
	//// ���G�b�W�ւ̍ŒZ�x�N�g�����|���S���̊O�ɂ͂ݏo���ꍇ�ɁA�ŒZ�x�N�g����
	//// �߂����̑��G�b�W�̒��_���A���G�b�W�Ƒ��G�b�W�̌�_�̑���Ƃ���
	////--------------------------------------------------------------------------
	//if (nextEdgeIntersection == pos3 || nextEdgeIntersection == pos4)
	//{
	//	float len1 = between2VectorLength(pos1, nextEdgeIntersection);
	//	float len2 = between2VectorLength(pos2, nextEdgeIntersection);
	//	if (len1 < len2)
	//	{
	//		tempIntersection = pos1;
	//	}
	//	else
	//	{
	//		tempIntersection = pos2;
	//	}

	//	subGoal = (tempIntersection + edgeIntersection) / 2;
	//	return;	// �����ŏI��
	//}

	////-----------------------------------------------------------------------------
	//// ��pos1,position,pos2�̊p�x�����߂遨angleOutSide
	//// ��edgeIntersection,position,nextEdgeIntersection�̊p�x�����߂遨angleInside
	//// �p�x�̔䗦����Εӂ̒��������߂�
	////-----------------------------------------------------------------------------
	//float angleOutside, angleInside;
	//float percentage;
	//float oppositeLen;
	//formedRadianAngle(&angleOutside, pos1 - position, pos2 - position);
	//formedRadianAngle(&angleInside, vecToEdge, vecToNextEdge);
	//percentage = angleInside / angleOutside;
	//float tempLen = between2VectorLength(pos1, pos2);
	//if (isnan(percentage))
	//{
	//	oppositeLen = 0.0f;
	//}
	//else
	//{
	//	oppositeLen = tempLen * percentage;
	//}

	////------------------------------------------------------------------------------
	//// nextEdgeIntersection��edgeIntersection���猩��pos1���ɂ��邩pos2���ɂ��邩��
	//// ���肷�邽�ߕ��ʂ̕����������
	//// ����͑����A���߂���tempIntersection��edgeIntersection���猩��pos1���ɂ��邩
	//// pos2���ɂ��邩�𔻒肷�邱�ƂɂȂ�
	////------------------------------------------------------------------------------
	//D3DXVECTOR3 cross;
	//D3DXVec3Cross(&cross, &nor, &(edgeIntersection - position));// �����̃m�[�}���͓񖇖ڂŕς�邩��
	//D3DXVec3Normalize(&cross, &cross);
	//// ���ʂ̖@�������ł�pos1���Ɍ�������
	//D3DXVECTOR3 temp1 = edgeIntersection + cross;
	//D3DXVECTOR3 temp2 = edgeIntersection - cross;
	//if (D3DXVec3LengthSq(&(temp1 - pos1)) > D3DXVec3LengthSq(&(temp2 - pos1)))
	//{
	//	cross = -cross;
	//}
	//// ���ʂ����߂�
	//float d = -(cross.x * edgeIntersection.x
	//	+ cross.y * edgeIntersection.y
	//	+ cross.z * edgeIntersection.z);
	//float distance = cross.x * nextEdgeIntersection.x
	//	+ cross.y * nextEdgeIntersection.y
	//	+ cross.z * nextEdgeIntersection.z
	//	+ d;

	////----------------------------------------------------------------------
	//// tempIntersection��pos1��pos2�̂ǂ���ɋ߂������Q�l�ɂ��̍��W���Z�o��
	//// �T�u�S�[�������߂�
	////----------------------------------------------------------------------
	//D3DXVECTOR3 dir;
	//if (distance > 0)
	//{// tempIntersection��pos1��
	//	between2VectorDirection(&dir, pos2, pos1);
	//}
	//else
	//{// tempIntersection��pos2��
	//	between2VectorDirection(&dir, pos1, pos2);
	//}
	//tempIntersection = edgeIntersection + dir * oppositeLen;
	//subGoal = (tempIntersection + edgeIntersection) / 2;
}
#else
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
	if (cross == D3DXVECTOR3(0.0f, 0.0, 0.0f))
	{// position, pos1, pos2�����꒼���ゾ�����ꍇ(����ɒႢ�\������)�O�ς������̂���
		subGoal = position;	// ���̃t���[����createVector()�̓x�N�g��(0, 0, 0)��Ԃ�
		return;
	}
	D3DXVec3Normalize(&cross, &cross);
	// ���ʂ̖@�������ł�position���Ɍ�������
	D3DXVECTOR3 temp1 = edgeIntersection + cross;
	D3DXVECTOR3 temp2 = edgeIntersection - cross;
	if (D3DXVec3LengthSq(&(temp1 - position)) > D3DXVec3LengthSq(&(temp2 - position)))
	{
		cross = -cross;
	}

	// (2)���ʂ̕������ɑ�����đ��G�b�W�܂ł̋��������߂�
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
#endif