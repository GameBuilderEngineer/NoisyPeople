//-----------------------------------------------------------------------------
// ���_�A�N�Z�X�N���X [VertexAccessor.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/13
//-----------------------------------------------------------------------------
#include "VertexAccessor.h"
using namespace vtxAccess;

//=============================================================================
// ���_�������
//=============================================================================
void VertexAccessor::initialize(StaticMesh* staticMesh)
{
	ZeroMemory(offset, sizeof(WORD) * ELEMENT_MAX);
	BYTE sumOffset = 0;
	int i = 0;

	// �Q�ƑΏۂ��Ƃɒ��_�A�h���X����̃I�t�Z�b�g�l���v�Z����
	while (staticMesh->vertexElement[i].Type != MAXD3DDECLTYPE)
	//while (i < 65)
	{
		switch (staticMesh->vertexElement[i].Usage)
		{
		case D3DDECLUSAGE_POSITION:
			offset[VERTEX_ELEMENT::POSITION] = sumOffset;
			break;

		case D3DDECLUSAGE_NORMAL:
			offset[VERTEX_ELEMENT::NORMAL] = sumOffset;
			break;

		case D3DDECLUSAGE_COLOR:
			offset[VERTEX_ELEMENT::COLOR] = sumOffset;
			break;

		case D3DDECLUSAGE_TEXCOORD:
			offset[VERTEX_ELEMENT::UV] = sumOffset;
			break;

		case D3DDECLUSAGE_BINORMAL:
			offset[VERTEX_ELEMENT::BINORMAL] = sumOffset;
			break;

		default:
			break;
		}

		switch (staticMesh->vertexElement[i].Type)
		{
		case D3DDECLTYPE_FLOAT1:
			sumOffset += sizeof(float);
			break;

		case D3DDECLTYPE_FLOAT2:
			sumOffset += sizeof(float) * 2;
			break;

		case D3DDECLTYPE_FLOAT3:
			sumOffset += sizeof(float) * 3;
			break;

		case D3DDECLTYPE_FLOAT4:
			sumOffset += sizeof(float) * 4;
			break;

		case D3DDECLTYPE_D3DCOLOR:
			sumOffset += sizeof(D3DCOLOR);
			break;

		case D3DDECLTYPE_UNUSED:// ���g�p
			sumOffset += 0;
			break;

		default:
			sumOffset += 0;
			break;
		}

		i++;
	}
}


//=============================================================================
// ���_����擾���������ւ̃|�C���^��Ԃ�
//=============================================================================
void* VertexAccessor::getPointer(int target, BYTE* begin)
{
	if (target < VERTEX_ELEMENT::ELEMENT_MAX)
	{
		return (void*)(begin + offset[target]);
	}
	else
	{
		return NULL;
	}
}

