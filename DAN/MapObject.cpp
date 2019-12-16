//-----------------------------------------------------------------------------
// �}�b�v�I�u�W�F�N�g�N���X [MapObject.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/12
//-----------------------------------------------------------------------------
#include "MapObject.h"
#include "ImguiManager.h"
#include "ObjectTypeList.h"
using namespace mapObjectNS;


// Static�����o�ϐ�
int MapObject::numOfMapObject = 0;


//=============================================================================
// �R���X�g���N�^
//=============================================================================
MapObject::MapObject(StaticMesh* _staticMesh, MapObjectData _mapObjData)
{
	numOfMapObject++;
	staticMesh = _staticMesh;

	// �}�b�v�I�u�W�F�N�g�f�[�^���Z�b�g
	mapObjectData = _mapObjData;
	position = mapObjectData.defaultPosition;
	axisZ.direction = mapObjectData.defaultDirection;

	{// �Փ˔���̐ݒ�
		using namespace ObjectType;
		treeCell.type = MAPOBJECT;
		treeCell.target = PLAYER | ENEMY;
	}

	// �I�u�W�F�N�g�̏�����
	onGravity = true;
	setSize(SETTING_SIZE[mapObjectData.type]);
	Object::initialize(&position);

	// ���b�V�����a���擾
	BoundingSphere radiusCheck;
	radiusCheck.initialize(&position, _staticMesh->mesh);
	setRadius(radiusCheck.getRadius());

	// �����ύX
	postureControl(axisZ.direction, mapObjectData.defaultDirection, 1);
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
StaticMesh* MapObject::getStaticMesh() { return staticMesh; }

//=============================================================================
// Setter
//=============================================================================
void MapObject::setDataToMapObject(MapObjectData _itemData) { mapObjectData = mapObjectData; }
