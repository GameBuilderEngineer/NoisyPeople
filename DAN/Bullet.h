//===================================================================================================================================
//�yBullet.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/05
// [�X�V��]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Object.h"
#include "StaticMeshRenderer.h"
#include "LinkedList.h"
#include "Ray.h"
#include "Sound.h"
#include "EffekseerManager.h"

namespace bulletNS{
	const float		SPEED			= 100.0f;	//�e��
	const float		INTERVAL_TIME	= 0.25f;	//�C���^�[�o������
	const float		RELOAD_TIME		= 0.5f;		//�����[�h����
	const int		MAGAZINE_NUM	= 8;		//�e��
	const float		EXIST_TIME		= 30.0f;	//���ݎ���
	const int		DIGITAL_POWER	= 20;		//�f�W�^���p���[
	const float		LAUNCH_FACT_TIME = 0.333f;	//���ˎ����c������


	//�e���G�t�F�N�g
	class Muzzle :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		D3DXMATRIX* syncMatrixRotation;
		Muzzle(D3DXVECTOR3* syncPosition, D3DXMATRIX* syncMatrixRotation) {
			this->syncPosition = syncPosition;
			this->syncMatrixRotation = syncMatrixRotation;
			effectNo = effekseerNS::MUZZLE;
			scale = D3DXVECTOR3(0.1f,0.1f,0.1f);
		}
		virtual void update() {
			position = *syncPosition;
			
			D3DXMATRIX R = *syncMatrixRotation;

			rotation.x = atan2f(R._32, R._33);
			rotation.y = asinf(-R._31);
			rotation.z = atan2f(R._21, R._11);

			Instance::update();
		};
	};

}

//===================================================================================================================================
//�y�o���b�g�N���X�z
//===================================================================================================================================
class Bullet :	public Object
{
private:
	D3DXVECTOR3		launchPosition;			//���ˈʒu
	Ray				ballisticRay;			//�e�����C
	D3DXVECTOR3		speed;					//���x
	D3DXVECTOR3		endPoint;				//�I���_
	D3DXVECTOR3		initialCollide;			//�����Փ˒n�_
	D3DXVECTOR3		collidePosition;		//�Փˈʒu
	int				digitalPower;			//�f�W�^���p���[
public:
//[��{����]
	Bullet(Ray shootingRay);
	~Bullet();
	void update(float frameTime);
	void render();
	bool collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);
	void destroy();
	//getter
	int	getDigitalPower();
	bool isCollideInitial();
	D3DXVECTOR3	getBulletSpeed();
};

//===================================================================================================================================
//�y�o���b�g�}�l�[�W���N���X�z
//===================================================================================================================================
class BulletManager :public Base
{
private:
	LinkedList<Bullet*>*	bulletList;		//�o���b�g���X�g
	StaticMeshRenderer*		renderer;		//�����_���[
	int						remaining;		//�c�e��
	float					intervalTimer;	//���̔��˂܂ł̃C���^�[�o������
	float					reloadTimer;	//�����[�h����
	bool					reloading;		//�����[�h��
	bool					isLaunched;		//���˂�����
	float					launchFactTime;	//���ˎ����c�����ԁi���˂����������Q�[�����Ɏc������.�X�V�p�x�̒Ⴂ�G�l�~�[�Z���T�ɂ̂��邽�߁j

	PLAY_PARAMETERS shotSE;
	PLAY_PARAMETERS reroadSE;

public:

//[��{����]
	//�R���X�g���N�^
	BulletManager();
	//�f�X�g���N�^
	~BulletManager();
	//�X�V
	void update(float frameTime);
	//�`��
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

//[�A�N�V����]
	//����
	bool launch(Ray shootingRay);
	//�����[�h
	void reload();
	//�e�폜
	void destroy(Bullet* bullet,int nodeNumber);

//[getter]
	int getRemaining();
	float getReloadTime();
	Bullet* getBullet(int i);
	int getNum();
	bool getIsLaunched();
};
