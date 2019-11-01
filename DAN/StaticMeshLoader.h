//===================================================================================================================================
//�yStaticMeshLoader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/24
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
		FIELD,									//01
		CUBE,									//02
		STAR_REGULAR_POLYHEDRON,				//03
		STAR_REGULAR_POLYHEDRON_X10,			//04
		STAR_REGULAR_POLYHEDRON_X100,			//05

		SAMPLE_REDBULL,							//03
		SAMPLE_SCISSORS,						//08
		ENEMY_01,

		DEAD_TREE,								//01
		GREEN_TREE_001,							//02
		GREEN_TREE_002,							//03
		GRASS,									//04
		STONE_001,								//05
		STONE_002,								//06
		STONE_003,								//07

		YAMADA_LAND,							//01
		NAKAGOMI_LAND,							//02
		YAMADA_ROBOT,							//03
		SAMPLE_NAVMESH,							//04
		YAMADA_ROBOT2,							//05
		YAMADA_PLANE,							//06
		DICE,									//07
		YAMADA_TEST_ZONE,						//08
		SKY_DOME,								//09
		TUTORIAL_FILED,							//10

		MESH_NUM,								//NUM
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

