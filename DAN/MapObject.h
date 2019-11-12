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
	enum MAP_OBJECT_TYPE
	{
		STONE_01,
		MAP_OBJECT_MAX
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
	LPD3DXMESH	attractorMesh;					// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;				// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X
	BoundingSphere sphereCollider;				// �R���C�_

	// Static
	static int numOfMapObject;				// �A�C�e���I�u�W�F�N�g�̑���

public:
	MapObject(StaticMesh* _staticMesh, mapObjectNS::MapObjectData _itemData);
	~MapObject();
	virtual void update(float frameTime);
	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);

	// Getter
	static int getNumOfMapObject();					// �}�b�v�I�u�W�F�N�g�̐����擾
	mapObjectNS::MapObjectData* getMapObjectData();	// �}�b�v�I�u�W�F�N�g�f�[�^���擾
	BoundingSphere* getSphereCollider();			// �R���C�_�̎擾

	// Setter
	void setDataToMapObject(mapObjectNS::MapObjectData _itemData);
};
