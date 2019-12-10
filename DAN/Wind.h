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
	const int NUM_WIND = 5;				// ���I�u�W�F�N�g�̐�

	// ���W
	const D3DXVECTOR3 POSITION[NUM_WIND] =
	{
		D3DXVECTOR3(234.0f, 13.0f, -178.0f),	// ���̑�n�̌��̒�
		D3DXVECTOR3(179.0f, 13.0f, -26.0f),		// ���̃W�����v����Ƃ�
		D3DXVECTOR3(58.0f, 34.0f, 158.0f),		// �k�̃W�����v����Ƃ�
		D3DXVECTOR3(-105.0f, 60.0f, 192.0f),	// �k�̑�n�ɂ�����Ƃ�
		D3DXVECTOR3(393.0f, 81.0f, -116.0f),	// ���̏���
	};

	// ���̕���
	const D3DXVECTOR3 DIRECTION[NUM_WIND] =
	{
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
	};

	// ���̔򋗗�
	const float DISTANCE[NUM_WIND] =
	{
		20.0f,
		14.0f,
		7.0f,
		15.0f,
		15.0f,
	};

	// ���̑��x
	const float SPEED[NUM_WIND]
	{
		1.5f,
		1.0f,
		1.0f,
		1.0f,
		1.0f,
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