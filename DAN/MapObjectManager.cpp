//-----------------------------------------------------------------------------
// �}�b�v�I�u�W�F�N�g�Ǘ��N���X [MapObject.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/12
//-----------------------------------------------------------------------------
#include "MapObjectManager.h"
#include "ImguiManager.h"
using namespace mapObjectNS;


//=============================================================================
// ������
//=============================================================================
void MapObjectManager::initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix)
{
	nextID = 0;			// ���񔭍sID��0�ɏ�����

	// �ڒn�t�B�[���h���Z�b�g
	attractorMesh = _attractorMesh;
	attractorMatrix = _attractorMatrix;

	// �`��I�u�W�F�N�g�̍쐬
	stone01Renderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STONE_003));


#if 0	// �}�b�v�I�u�W�F�N�g�c�[���̃f�[�^��ǂݍ���

#endif
}


//=============================================================================
// �I������
//=============================================================================
void MapObjectManager::uninitialize()
{
	// �S�c���[�I�u�W�F�N�g��j��
	destroyAllMapObject();

	// �x�N�^�[�̊m�ۃ��������������i�������A���P�[�^�����ɖ߂��j
	std::vector<MapObject*> temp;
	mapObjectList.swap(temp);

	// �`��I�u�W�F�N�g�̔j��
	SAFE_DELETE(stone01Renderer);
}


//=============================================================================
// �X�V����
//=============================================================================
void MapObjectManager::update(float frameTime)
{
	for (size_t i = 0; i < mapObjectList.size(); i++)
	{
		mapObjectList[i]->update(frameTime);
	}

	stone01Renderer->update();
}


//=============================================================================
// �`�揈��
//=============================================================================
void MapObjectManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	stone01Renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// �}�b�v�I�u�W�F�N�g�̍쐬
//=============================================================================
void MapObjectManager::createMapObject(MapObjectData mapObjectData)
{
	MapObject* mapObject = new MapObject(staticMeshNS::reference(staticMeshNS::STONE_003), mapObjectData);

	// �}�b�v�I�u�W�F�N�g�̕`����Z�b�g
	switch (mapObjectData.type)
	{
	case STONE_01:
		mapObject->scale *= 0.1f;// �T�C�Y�̒���
		stone01Renderer->registerObject(mapObject);
		break;
	}

	mapObject->setAttractor(attractorMesh, attractorMatrix);
	mapObjectList.push_back(mapObject);
}


//=============================================================================
// �S�}�b�v�I�u�W�F�N�g�̔j��
//=============================================================================
void MapObjectManager::destroyAllMapObject()
{
	// �`��S����
	stone01Renderer->allUnRegister();


	for (int i = 0; i < mapObjectList.size(); i++)
	{
		SAFE_DELETE(mapObjectList[i]);
	}
	mapObjectList.clear();
}


//=============================================================================
// �c���[ID�𔭍s����
//=============================================================================
int MapObjectManager::issueNewMapObjectID()
{
	int ans = nextID;
	nextID++;
	return ans;
}


//=============================================================================
// ImGui�ɏo��
//=============================================================================
void MapObjectManager::outputGUI()
{
#ifdef _DEBUG

#endif
}


//=============================================================================
// Getter
//=============================================================================
std::vector<MapObject*>& MapObjectManager::getMapObjectList(){ return mapObjectList; }
