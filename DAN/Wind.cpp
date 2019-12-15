//-----------------------------------------------------------------------------
// ���N���X [Wind.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/11
//-----------------------------------------------------------------------------
#include "Wind.h"
#include "Input.h"
using namespace windNS;


//=============================================================================
// ������
//=============================================================================
void WindManager::initialize(std::string _sceneName, LPD3DXMESH _attractorMesh,
	D3DXMATRIX* _attractorMatrix)
{
	// ���I�u�W�F�N�g��������
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].windDirection = DIRECTION[i];
		wind[i].windSpeed = SPEED[i];
		// �I�u�W�F�N�g�̃}�g���N�X���Փ˔���Ɏg��
		// wind���̂̃X�P�[���Ƌ��̃X�P�[�������낦��ƃf�o�b�O�`��ƏՓ˔��肪��v����
		wind[i].scale *= SCALE[i];
		wind[i].sphere.initialize(&wind[i].position, 1.0f);
		wind[i].sphere.setScale(SCALE[i]);
		wind[i].initialize((D3DXVECTOR3*)&POSITION[i]);

		// �G�t�F�N�g���N�����Ă���
		wind[i].effect = new effekseerNS::Instance(0, effekseerNS::WIND);
		wind[i].effect->position = wind[i].position;
		wind[i].effect->scale = D3DXVECTOR3(3.5f, 7.0f, 3.5f);
		effekseerNS::play(0, wind[i].effect);
	}
}


//=============================================================================
// �I������
//=============================================================================
void WindManager::uninitialize()
{
}


//=============================================================================
// �X�V����
//=============================================================================
void WindManager::update(float frameTime)
{

}


//=============================================================================
// �`�揈��
//=============================================================================
void WindManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
#ifdef _DEBUG
	for (int i = 0; i < NUM_WIND; i++)
	{
		wind[i].sphere.render();
	}
#endif
}


//=============================================================================
// ���Ƃ̏Փˏ���
//=============================================================================
void  WindManager::windCollision(Player* player)
{
	float distance = 0.0f;

	for (int i = 0; i < NUM_WIND; i++)
	{
#if 0 // ���̏������s���Ƒ��̃G�t�F�N�g�Ńo�O��.���̂�������Ȃ�.
		// �G�t�F�N�g��`�悷�邩���f����
		float temp1 = between2VectorLength(wind[i].position, player[0].center);
		float temp2 = between2VectorLength(wind[i].position, player[1].center);
		if (temp1 > EFFECT_RENDERING_DISTANCE && temp2 > EFFECT_RENDERING_DISTANCE)
		{
			wind[i].effect->setShown(false);
			continue;
		}
		else
		{
			wind[i].effect->setShown(true);
		}
#endif

		// �Փ˔���
		for (int k = 0; k < playerNS::NUM_PLAYER; k++)
		{
			distance = between2VectorLength(wind[i].position, player[k].center);
			if (distance > DISTANCE[i]) { continue; }

			ray.update(player[k].center, -wind[i].windDirection);
			if (ray.rayIntersect(wind[i].sphere.getMesh(), wind[i].matrixWorld))
			{
				player[k].jump();
				player[k].addSpeed(wind[i].windDirection * wind[i].windSpeed);
			}
		}
	}

}
