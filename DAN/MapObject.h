//-----------------------------------------------------------------------------
// �}�b�v�I�u�W�F�N�g�N���X [MapObject.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/12
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"


//=============================================================================
// ���O���
//=============================================================================
namespace mapObjectNS
{
	// �}�b�v�I�u�W�F�N�g�̎��
	// ���f���Ⴂ���S�ă^�C�v�Ƃ��ĕ������Ă���
	enum MAPOBJECT_TYPE
	{
		STONE_01,
		STONE_02, 
		STONE_03,

		MAPOBJECT_MAX
	};

	// �T�C�Y
	const D3DXVECTOR3 SETTING_SIZE[MAPOBJECT_MAX] =
	{
		D3DXVECTOR3(1.7f, 1.0f, 1.7f),	// STONE_01
	};

	typedef struct MapObjectData
	{
		int mapObjectID;				// ���ʔԍ�(0..*)
		int type;						// �A�C�e���^�C�v
		D3DXVECTOR3 defaultPosition;	// �������W
		D3DXVECTOR3 defaultDirection;	// �������ʕ���
		void zeroClear() { ZeroMemory(this, sizeof(MapObjectData)); }
	}MAPOBJECTSET;
}


//=============================================================================
//�N���X��`
//=============================================================================
class MapObject: public Object
{
private:
	mapObjectNS::MapObjectData mapObjectData;	// �}�b�v�I�u�W�F�N�g�f�[�^
	StaticMesh*	staticMesh;						// ���b�V�����
	LPD3DXMESH	attractorMesh;					// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;				// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X

	// Static
	static int numOfMapObject;					// �A�C�e���I�u�W�F�N�g�̑���

public:
	MapObject(StaticMesh* _staticMesh, mapObjectNS::MapObjectData _mapObjData);
	~MapObject();
	// �X�V
	virtual void update(float frameTime);
	// �d�͔������b�V���̐ݒ�
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	// Getter
	static int getNumOfMapObject();					// �}�b�v�I�u�W�F�N�g�̐����擾
	mapObjectNS::MapObjectData* getMapObjectData();	// �}�b�v�I�u�W�F�N�g�f�[�^���擾
	StaticMesh* getStaticMesh();					// ���b�V�������擾
	// Setter
	void setDataToMapObject(mapObjectNS::MapObjectData _itemData);
};
