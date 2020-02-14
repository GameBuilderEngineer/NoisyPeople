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
		SAMPLE_SCISSORS,
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

		GREEN_TREE_002,
		GRASS,
		STONE_003,
		A_TRUNK,
		A_LEAF,
		B_TRUNK,
		B_LEAF,
		C_TRUNK,
		C_LEAF,
		OCEAN,
		ITEM_BRANCH,


		YAMADA_ROBOT,
		YAMADA_ROBOT2,
		SKY_DOME,
		TUTORIAL_FILED,
		DATE_ISLAND_FINAL,
		DATE_ISLAND_FINAL_FACE,
		DATE_ISLAND_FINAL_FACE_BLACK,
		DIGITAL_SPHERE,

		POWER_UP_ITEM,
		DISTANT_VIEW,

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

	//Normal��Tangent�̏���t�L����֐�
	HRESULT withNormalTangent(
		LPDIRECT3DDEVICE9 device,	//�`��f�o�C�X
		LPD3DXMESH* sourceMesh		//���̃��b�V��
		);

};

