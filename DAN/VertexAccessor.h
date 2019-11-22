//-----------------------------------------------------------------------------
// ���_�A�N�Z�X�N���X [VertexAccessor.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshLoader.h"

namespace vtxAccess
{
	// �A�N�Z�X�v�f
	enum VERTEX_ELEMENT
	{
		POSITION,
		NORMAL,
		COLOR,
		UV,
		// �o���v�}�b�v�Ƃ�����ꍇ��
		// �����𑝂₷�I
		ELEMENT_MAX
	};
}

// ���_�A�N�Z�X�N���X
struct VertexAccessor
{
	WORD offset[vtxAccess::ELEMENT_MAX];		// �I�t�Z�b�g�T�C�Y�z��

	// ���_����擾���������ւ̃|�C���^��Ԃ�
	void* getPointer(int target, BYTE* begin);

	// ���_������͂��Ē��_�A�N�Z�X�N���X��Ԃ�
	void initialize(StaticMesh* staticMesh);
};

