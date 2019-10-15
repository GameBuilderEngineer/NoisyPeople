//===================================================================================================================================
//�yStaticMeshLoader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"

//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V���\���́z
//===================================================================================================================================
struct StaticMesh
{
	LPDIRECT3DVERTEXBUFFER9				vertexBuffer;				//���_���o�b�t�@
	LPDIRECT3DINDEXBUFFER9				indexBuffer;				//�C���f�b�N�X�o�b�t�@
	LPD3DXMESH							mesh;						//���b�V�����
	D3DMATERIAL9*						materials;					//�}�e���A�����i�[��
	LPD3DXBUFFER						bufferMaterial;				//���b�V���̃}�e���A���o�b�t�@
	LPDIRECT3DTEXTURE9*					textures;					//�e�N�X�`�����i�[��
	DWORD								numMaterial;				//�}�e���A�����̐�
	D3DVERTEXELEMENT9					vertexElement[65];			//���_����
	LPDIRECT3DVERTEXDECLARATION9		declaration;				//���_�錾
	DWORD								numBytesPerVertex;			//���_�T�C�Y
	D3DXATTRIBUTERANGE*					attributeTable;				//�����e�[�u��
	DWORD								attributeTableSize;			//�����e�[�u���T�C�Y
};

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace staticMeshNS {
	enum MESH_NUMBER {
		FIELD,
		CUBE,
		SAMPLE_HIERARCHY_MESH,
		PLANET,
		SAMPLE_ROBOT001,
		SAMPLE_ROBOT002,
		SAMPLE_SKINNED_MESH,
		STAR_REGULAR_POLYHEDRON,
		STAR_REGULAR_POLYHEDRON_X10,
		STAR_REGULAR_POLYHEDRON_X100,
		SAMPLE_STATIC_MESH,
		SAMPLE_TOON_MESH,
		SAMPLE_BUNNY,
		SAMPLE_PAPER,
		SAMPLE_REDBULL,
		SAMPLE_SHOES,
		SAMPLE_TEAPOT,
		SAMPLE_LIGHT,
		SAMPLE_BALL,
		SAMPLE_SCISSORS,
		SAMPLE_PLAYSTATION,
		SAMPLE_HAT,
		DEAD_TREE,
		YAMADA_LAND,
		NAKAGOMI_LAND,
		YAMADA_ROBOT,
		SAMPLE_NAVMESH,
		YAMADA_ROBOT2,
		YAMADA_PLANE,
		DICE,
		YAMADA_TEST_ZONE,
		
		MESH_NUM,
	};

	//�O���Q�Ɨp�֐�
	StaticMesh* reference(int staticMeshNo);
}


//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V���Ǎ��N���X�z
//===================================================================================================================================
class StaticMeshLoader :public Base
{
private:
	const char* fileName[staticMeshNS::MESH_NUM];
public:
	StaticMesh	staticMesh[staticMeshNS::MESH_NUM];
	StaticMeshLoader();
	~StaticMeshLoader();

	HRESULT load(LPDIRECT3DDEVICE9 device);
	void release(void);
};

