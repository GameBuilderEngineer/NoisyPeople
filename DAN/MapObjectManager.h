//-----------------------------------------------------------------------------
// �}�b�v�I�u�W�F�N�g�Ǘ��N���X [MapObject.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/12
//-----------------------------------------------------------------------------
#pragma once
#include <vector>
#include "StaticMeshRenderer.h"
#include "MapObject.h"

//=============================================================================
// ���O���
//=============================================================================


//=============================================================================
//�N���X��`
//=============================================================================
class MapObjectManager
{
private:
	std::vector<MapObject*> mapObjectList;		// �c���[�|�C���^���X�g
	StaticMeshRenderer* stone01Renderer;		// ��01�`��I�u�W�F�N�g
	int nextID;									// ���񔭍s�}�b�v�I�u�W�F�N�gID
	LPD3DXMESH	attractorMesh;					// �d�́i���́j�������b�V��
	D3DXMATRIX*	attractorMatrix;				// �d�́i���́j�����I�u�W�F�N�g�}�g���b�N�X

public:
	void initialize(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void createMapObject(mapObjectNS::MapObjectData treeData);
	void destroyAllMapObject();
	int issueNewMapObjectID();
	void outputGUI();

	// Getter
	std::vector<MapObject*>& getMapObjectList();
};
