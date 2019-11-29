//-----------------------------------------------------------------------------
// ���N���X [Wind.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/11
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "StaticMeshRenderer.h"
#include "Player.h"

namespace windNS
{
	const int NUM_WIND = 3;				// ���I�u�W�F�N�g�̐�

	// ���W
	const D3DXVECTOR3 POSITION[NUM_WIND] =
	{
		D3DXVECTOR3(235.0f, 85.0f, -117.0f),
		D3DXVECTOR3(158.0f, 27.640f, 27.783f),
		D3DXVECTOR3(-64.0f, 11.145f, -190.0f),
	};

	// ���̕���
	const D3DXVECTOR3 DIRECTION[NUM_WIND] =
	{
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	};

	// ���̔򋗗�
	const float DISTANCE[NUM_WIND] =
	{
		10.0f,
		10.0f,
		10.0f,
	};

	// ���̑��x
	const float SPEED[NUM_WIND]
	{
		1.0f,
		1.0f,
		1.0f
	};
}


//=============================================================================
//�N���X��`
//=============================================================================
class WindManager: public Base
{
private:

	// ���N���X
	class Wind: public Object
	{
	public:
		D3DXVECTOR3 windDirection;
		float windSpeed;
	};

	Wind wind[windNS::NUM_WIND];		// ���I�u�W�F�N�g
	Ray ray;							// ���p���C
	StaticMeshRenderer* windRenderer;	// �`��I�u�W�F�N�g

public:
	void initialize(std::string _sceneName, LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void windCollision(Player* player);
};