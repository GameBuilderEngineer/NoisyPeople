//===================================================================================================================================
//�yStaticMeshLoader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/11/07
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
		//FIELD,									//01
		//CUBE,									//02
		//STAR_REGULAR_POLYHEDRON,				//03
		//STAR_REGULAR_POLYHEDRON_X10,			//04
		//STAR_REGULAR_POLYHEDRON_X100,			//05

		//SAMPLE_REDBULL,							//03
		SAMPLE_SCISSORS,						//08
		//ENEMY_01,
		//ENEMY_02,
		WOLF,
		TIGER,
		BEAR,
		WOLF_BODY,
		WOLF_ARM,
		TIGER_BODY,
		TIGER_GUN,
		TIGER_LEG_L,
		TIGER_LEG_R,
		BEAR_BODY, 
		BEAR_ARM_L,
		BEAR_ARM_R,
		BEAR_WAIST,
		BEAR_LEG_L,
		BEAR_LEG_R,

		//DEAD_TREE,								//01
		//GREEN_TREE_001,							//02
		GREEN_TREE_002,							//03
		GRASS,									//04
		//STONE_001,								//05
		//STONE_002,								//06
		STONE_003,								//07
		A_TRUNK,								//08
		A_LEAF,									//09
		B_TRUNK,								//10
		B_LEAF,									//11
		C_TRUNK,								//12
		C_LEAF,									//13
		OCEAN,									//14

		ITEM_BRANCH,


		//YAMADA_LAND,							//01
		//NAKAGOMI_LAND,							//02
		YAMADA_ROBOT,							//03
		//SAMPLE_NAVMESH,							//04
		YAMADA_ROBOT2,							//05
		//YAMADA_PLANE,							//06
		//DICE,									//07
		//YAMADA_TEST_ZONE,						//08
		SKY_DOME,								//09
		TUTORIAL_FILED,							//10
		//DATE_ISLAND,							//11
		//DATE_ISLAND_V2,							//12
		//DATE_ISLAND_V2_NAVI_MESH,				//13
		//DATE_ISLAND_V2_BLACK,					//14
		DATE_ISLAND_FINAL,						//15
		//DATE_ISLAND_FINAL_BLACK,				//16
		//DATE_ISLAND_FINAL_NAVIMESH,				//17
		DATE_ISLAND_FINAL_FACE,					//18
		DATE_ISLAND_FINAL_FACE_BLACK,			//19
		DIGITAL_SPHERE,							//01

		//NAV_TEST1,
		//NAV_TEST2,
		//NAV_TEST3,
		//NAV_TEST4,
		//NAV_TEST5,
		//NAV_TEST6,

		POWER_UP_ITEM,
		DISTANT_VIEW,

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

	//Normal��Tangent�̏���t�L����֐�
	HRESULT withNormalTangent(
		LPDIRECT3DDEVICE9 device,	//�`��f�o�C�X
		LPD3DXMESH* sourceMesh		//���̃��b�V��
		);

};

