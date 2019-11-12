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

namespace bulletNS{
	const float		SPEED			= 100.0f;	//�e��
	const float		INTERVAL_TIME	= 0.25f;	//�C���^�[�o������
	const float		RELOAD_TIME		= 0.5f;		//�����[�h����
	const int		MAGAZINE_NUM	= 8;		//�e��
	const float		EXIST_TIME		= 1.0f;		//���ݎ���
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

public:
//[��{����]
	Bullet(Ray shootingRay);
	~Bullet();
	void update(float frameTime);
	void render();
	void collide();

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
	void launch(Ray shootingRay);
	//�����[�h
	void reload();
	
//[getter]
	int getRemaining();
	float getReloadTime();
	Bullet* getBullet(int i);
	int getNum();
};
