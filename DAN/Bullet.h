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
#include "ImguiManager.h"

namespace bulletNS{
	const float		SPEED			= 100.0f;	//�e��
	const float		INTERVAL_TIME	= 0.25f;	//�C���^�[�o������
	const float		RELOAD_TIME		= 0.5f;		//�����[�h����
	const int		MAGAZINE_NUM	= 8;		//�e��
	const float		EXIST_TIME		= 30.0f;	//���ݎ���
	const int		DIGITAL_POWER	= 20;		//�f�W�^���p���[
	const float		LAUNCH_FACT_TIME = 0.333f;	//���ˎ����c������
	const float		LOST_DISTANCE	= 400.0f;

	//�e���G�t�F�N�g
	class Muzzle :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		Muzzle(D3DXMATRIX* syncMatrix) {
			this->syncMatrix = syncMatrix;
			effectNo = effekseerNS::MUZZLE;
			scale = D3DXVECTOR3(0.1f,0.1f,0.1f);
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
			manager->SetScale(handle, scale.x, scale.y, scale.z);
		};
	};

	//�e�{�̃G�t�F�N�g
	class BulletBody :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		BulletBody(D3DXMATRIX* syncMatrix) {
			this->syncMatrix = syncMatrix;
			effectNo = effekseerNS::DAC_BULLET;
			scale = D3DXVECTOR3(1.0f,1.0f,1.0f);
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

	class BulletBody2 :public effekseerNS::Instance
	{
	public:
		D3DXMATRIX* syncMatrix;
		BulletBody2(D3DXMATRIX* syncMatrix) {
			this->syncMatrix = syncMatrix;
			effectNo = effekseerNS::POW_BULLET;
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
	effekseerNS::Instance* effect;			//�e�G�t�F�N�g

public:
	int playerNo;
public:
//[��{����]
	Bullet(Ray shootingRay,int playerNo, bool isPowUp);
	~Bullet();
	void update(float frameTime);
	void render();
	bool collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix);
	void destroy();
	//getter
	int	getDigitalPower();
	bool isCollideInitial();
	D3DXVECTOR3	getBulletSpeed();
	//setter
	void setDigitalPower(float value);


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
	float                   powerRate;
	bool					isPowerUp;		// �A�C�e�������p���[�A�b�v���Ă���
	PLAY_PARAMETERS shotSE;
	PLAY_PARAMETERS reroadSE;
	std::string		currentScene;			//���݂̃V�[��

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
	bool launch(Ray shootingRay,int playerNo);
	//�����[�h
	void reload(int playerNo);
	//�e�폜
	void destroy(Bullet* bullet,int nodeNumber);

//[getter]
	int getRemaining();
	float getReloadTime();
	Bullet* getBullet(int i);
	int getNum();
	bool getIsLaunched();
	float getPowerRate();
//[setter]
	void setPowerRate(float value);
	void setCurrentScene(std::string _scene);

#ifdef _DEBUG
	//void bulletGUI();
#endif
};
