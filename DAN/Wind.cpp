//-----------------------------------------------------------------------------
// ���N���X [Wind.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/11
//-----------------------------------------------------------------------------
#include "Wind.h"
using namespace windNS;


//=============================================================================
// ������
//=============================================================================
void WindManager::initialize(std::string _sceneName, LPD3DXMESH _attractorMesh,
	D3DXMATRIX* _attractorMatrix)
{
	// �`��I�u�W�F�N�g�̍쐬
	windRenderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10));

	// ���I�u�W�F�N�g��������
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].initialize((D3DXVECTOR3*)&POSITION[i]);
		wind[i].windDirection = DIRECTION[i];
		wind[i].windSpeed = SPEED[i];
		windRenderer->registerObject(&wind[i]);
		wind[i].scale *= 0.2f;
	}
}


//=============================================================================
// �I������
//=============================================================================
void WindManager::uninitialize()
{
	SAFE_DELETE(windRenderer);
}


//=============================================================================
// �X�V����
//=============================================================================
void WindManager::update(float frameTime)
{
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].update();
	}

	windRenderer->update();
}


//=============================================================================
// �`�揈��
//=============================================================================
void WindManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	windRenderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}


//=============================================================================
// ���Ƃ̏Փˏ���
//=============================================================================
void  WindManager::windCollision(Player* player)
{
	float distance = 0.0f;

	for (int i = 0; i < NUM_WIND; i++)
	{
		distance = between2VectorLength(wind[i].position, *player->getCentralPosition());
		if (distance > DISTANCE[i]) { continue; }

		ray.update(player->position, -wind[i].windDirection);
		if (ray.rayIntersect(staticMeshNS::reference(staticMeshNS::STAR_REGULAR_POLYHEDRON_X10)->mesh,
			wind[i].matrixWorld))
		{
			player->addSpeed(wind[i].windDirection * wind[i].windSpeed);
		}
	}
}
