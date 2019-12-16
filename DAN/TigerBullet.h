//-----------------------------------------------------------------------------
// �^�C�K�[�o���b�g [TigerBullet.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/10
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "LinkedList.h"
#include "Sound.h"
#include "EffekseerManager.h"

//=============================================================================
// �o���b�g
//=============================================================================
class TigerBullet: public Object
{
private:
	D3DXVECTOR3		launchPosition;			//���ˈʒu
	Ray				ballisticRay;			//�e�����C
	D3DXVECTOR3		bulletSpeed;			//���x
	D3DXVECTOR3		endPoint;				//�I���_
	D3DXVECTOR3		initialCollide;			//�����Փ˒n�_
	D3DXVECTOR3		collidePosition;		//�Փˈʒu

	bool isHit;

public:
	const float		SPEED = 100.0f;	//�e��
	const float		EXIST_TIME = 3.0f;	//���ݎ���

	TigerBullet(Ray shootingRay);
	~TigerBullet();
	void update(float frameTime);
	void render();
	bool collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);

	void hit();

	// Getter
	bool getIsHit() { return isHit; }
	// Setter
};


//=============================================================================
// �o���b�g�}�l�[�W���[
//=============================================================================
class TigerBulletManager: public Base
{
private:
	LinkedList<TigerBullet*>	bulletList;		// �o���b�g���X�g
	StaticMeshRenderer*			renderer;		// �����_���[
	int							remaining;		// �c�e��
	float						intervalTimer;	// ���̔��˂܂ł̃C���^�[�o������
	bool						isShot;			// ���˂�����

public:
	const int MAGAZINE_NUM = 3;
	const float INTERVAL_TIME = 0.0f;
	const float RELOAD_TIME = 2.0f;

	TigerBulletManager(StaticMeshRenderer* _renderer);
	~TigerBulletManager();
	// �X�V
	void update(float frameTime);
	// �`��
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	// ����
	bool shoot(Ray shootingRay);
	// �e�̔j��
	void destroy(TigerBullet* bullet, int nodeNumber);
};