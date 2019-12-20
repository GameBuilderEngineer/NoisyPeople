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


// �o���b�g�G�t�F�N�g�N���X
class TigerBulletEffect :public effekseerNS::Instance
{
public:
public:
	D3DXMATRIX* syncMatrix;
	TigerBulletEffect(D3DXMATRIX* syncMatrix) {
		this->syncMatrix = syncMatrix;
		effectNo = effekseerNS::TIGER_BULLET;
		scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}
	virtual void update() {

		D3DXMATRIX M = *syncMatrix;

		::Effekseer::Manager*	manager = getEffekseerManager(managerNo)->manager;
		::Effekseer::Matrix43 matrix;
		matrix.Value[0][0] = M._11;
		matrix.Value[0][1] = M._12;
		matrix.Value[0][2] = M._13;
		matrix.Value[1][0] = M._21;
		matrix.Value[1][1] = M._22;
		matrix.Value[1][2] = M._23;
		matrix.Value[2][0] = M._31;
		matrix.Value[2][1] = M._32;
		matrix.Value[2][2] = M._33;
		matrix.Value[3][0] = M._41;
		matrix.Value[3][1] = M._42;
		matrix.Value[3][2] = M._43;
		manager->SetMatrix(handle, matrix);
	};
};


//=============================================================================
// �o���b�g
//=============================================================================
class TigerBullet: public Object
{
private:
	D3DXVECTOR3			launchPosition;			//���ˈʒu
	Ray					ballisticRay;			//�e�����C
	D3DXVECTOR3			bulletSpeed;			//���x
	D3DXVECTOR3			endPoint;				//�I���_
	D3DXVECTOR3			initialCollide;			//�����Փ˒n�_
	D3DXVECTOR3			collidePosition;		//�Փˈʒu
	TigerBulletEffect*	tigerBulletEffect;		//�o���b�g�G�t�F�N�g
	bool isHit;

public:
	const float		SPEED = 25.0f;			//�e��
	const float		EXIST_TIME = 3.0f;		//���ݎ���

	TigerBullet(Ray shootingRay);
	~TigerBullet();
	void update(float frameTime);
	void render();
	bool collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);

	void hit();

	// Getter
	bool getIsHit() { return isHit; }
	// Setter
	void setIsHit(bool set) { isHit = set; }

};


//=============================================================================
// �o���b�g�}�l�[�W���[
//=============================================================================
class TigerBulletManager: public Base
{
private:
	LinkedList<TigerBullet*>	bulletList;		// �o���b�g���X�g
	int							remaining;		// �c�e��
	float						intervalTimer;	// ���̔��˂܂ł̃C���^�[�o������
	bool						isShot;			// ���˂�����

public:
	const int MAGAZINE_NUM = 3;
	const float INTERVAL_TIME = 0.5f;
	const float RELOAD_TIME = 2.0f;

	TigerBulletManager();
	~TigerBulletManager();
	// �X�V
	void update(float frameTime);
	// �`��
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);
	// ����
	bool shoot(Ray shootingRay);
	// �e�̔j��
	void destroy(TigerBullet* bullet, int nodeNumber);
	// �o���b�g���X�g�̎擾
	LinkedList<TigerBullet*>* getBulletList();
};