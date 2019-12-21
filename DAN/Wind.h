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
		D3DXVECTOR3(-114.0f, 60.0f, 187.0f),	// �k�̑�n�ɂ�����Ƃ�
		D3DXVECTOR3(369.0f, 81.0f, -102.0f),	// ���̏���
	};

	// �Փ˔��苅�̑傫���i= �I�u�W�F�N�g�̑傫���j
	const float SCALE[NUM_WIND] =
	{
		3.0f,
		3.0f,
		3.0f,
		3.0f,
		3.0f
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
		8.0f, 
		6.0f,
		4.0f,
		6.0f,
		8.0f,
	};

	// ���̑��x
	const float SPEED[NUM_WIND]
	{
		2.0f * 60,
		1.0f * 60,
		1.0f * 60,
		1.0f * 60,
		1.5f * 60,
	};

	const float EFFECT_RENDERING_DISTANCE = 200.0f;	// �G�t�F�N�g�`�拗��
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
		effekseerNS::Instance* effect;
		BoundingSphere sphere;
	};

	// ���ɓ������Ă��邩�̃t���O
	bool isPlayerWindReceive[gameMasterNS::PLAYER_NUM];

	Wind wind[windNS::NUM_WIND];		// ���I�u�W�F�N�g
	Ray ray;							// ���p���C
	Player* player;						// �v���C���[


public:
	void initialize(Player* _player);
	void uninitialize();
	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	void windCollision(Player* _player);
};