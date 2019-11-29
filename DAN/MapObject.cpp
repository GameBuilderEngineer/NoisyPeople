//-----------------------------------------------------------------------------
// �}�b�v�I�u�W�F�N�g�N���X [MapObject.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/12
//-----------------------------------------------------------------------------
#include "MapObject.h"
#include "ImguiManager.h"
using namespace mapObjectNS;


int MapObject::numOfMapObject = 0;


//=============================================================================
// �R���X�g���N�^
//=============================================================================
MapObject::MapObject(StaticMesh* _staticMesh, MapObjectData _itemData)
{
	numOfMapObject++;

	onGravity = true;
	mapObjectData = _itemData;
	position = mapObjectData.defaultPosition;

	if (CAN_COLLISION[mapObjectData.type])
	{
		treeCell.type = OBJECT_TYPE[mapObjectData.type];
		treeCell.target = COLLISION_TARGET[mapObjectData.type];
		setSize(SETTING_SIZE[mapObjectData.type]);
	}
	else
	{
		treeCell.type = ObjectType::NONE;
	}



	Object::initialize(&position);



	//setRadius(1.0f);


}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
MapObject::~MapObject()
{
	numOfMapObject--;
}


//=============================================================================
// �X�V����
//=============================================================================
void MapObject::update(float frameTime)
{
	Object::update();
}


//=============================================================================
// �d�͔������b�V���i�ڒn���b�V���j�̐ݒ�
//=============================================================================
void MapObject::setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;
}


//=============================================================================
// Getter
//=============================================================================
int MapObject::getNumOfMapObject(){ return numOfMapObject; }
MapObjectData* MapObject::getMapObjectData() { return &mapObjectData; }


//=============================================================================
// Setter
//=============================================================================
void MapObject::setDataToMapObject(MapObjectData _itemData) { mapObjectData = mapObjectData; }
