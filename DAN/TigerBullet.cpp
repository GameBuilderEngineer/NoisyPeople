//-----------------------------------------------------------------------------
// �^�C�K�[�o���b�g [TigerBullet.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/10
//-----------------------------------------------------------------------------
#include "TigerBullet.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
TigerBullet::TigerBullet(Ray shootingRay)
{
	launchPosition = shootingRay.start;
	bulletSpeed = shootingRay.direction * SPEED;
	//initialCollide = launchPosition + shootingRay.direction * shootingRay.distance;

	// �e���̏�����
	ballisticRay.initialize(launchPosition, shootingRay.direction);
	ballisticRay.color = D3DXCOLOR(0, 255, 120, 255);

	{// �I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ̎w��
		using namespace ObjectType;
		treeCell.type = BULLET;
		treeCell.target = PLAYER;
	}

	postureControl(axisZ.direction, shootingRay.direction, 1.0f);	// ���f����i�s�����֎p�����䂷��
	existenceTimer = EXIST_TIME;									// ���ݎ���
	endPoint = launchPosition + bulletSpeed * EXIST_TIME;			// ���ɂ��Փ˂��Ȃ������ꍇ�̏I���ʒu
	isHit = false;													// �Փ˃t���O���I�t
	Object::initialize(&launchPosition);
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
TigerBullet::~TigerBullet()
{

}


//=============================================================================
// �X�V
//=============================================================================
void TigerBullet::update(float frameTime)
{
	if (existenceTimer <= 0)
	{
		return;
	}
	existenceTimer -= frameTime;

	// �ʒu�X�V
	D3DXVec3Lerp(&position, &ballisticRay.start, &endPoint, 1.0f - existenceTimer / EXIST_TIME);
	Object::update();

	// �O�t���[���̏Փ˔���ŏՓ˂��Ă����true
	if (isHit)
	{
		hit();
	}
}


//=============================================================================
// �`��
//=============================================================================
void TigerBullet::render()
{
	//float length = Base::between2VectorLength(ballisticRay.start, position);
	//ballisticRay.render(length);
	//debugRender();
}


//=============================================================================
// �Փ˔���
//=============================================================================
bool TigerBullet::collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix)
{
	// �e�����C�̏��������h�~
	Ray ray;
	ray.initialize(ballisticRay.start, ballisticRay.direction);

	// ���C����
	bool hit = ray.rayIntersect(targetMesh, targetMatrix);

	if (hit)
	{
		// ��������
		hit = (Base::between2VectorLength(ballisticRay.start, position) > ray.distance);
	}

	return hit;
}


//=============================================================================
// �폜
//=============================================================================
void TigerBullet::hit()
{
	existenceTimer = 0.0f;

	//�T�E���h�̍Đ�
	PLAY_PARAMETERS hitSE;
	hitSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_HitBulletTree, false ,NULL,false,NULL };
	SoundInterface::SE->playSound(&hitSE);
	//�G�t�F�N�g�̍Đ�
	effekseerNS::Instance* instance = new effekseerNS::Instance();
	instance->position = position;
	effekseerNS::play(0, instance);
}


//=============================================================================
// ����
//=============================================================================
//void TigerBullet::isHitBeforeDisappear()
//{
//
//}


//=============================================================================
// Getter
//=============================================================================


//=============================================================================
// Setter
////=============================================================================


///////////////////////////////////////////////////////////////////////////////
//////////////////////////// �o���b�g�}�l�[�W���[ /////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//=============================================================================
// �R���X�g���N�^
//=============================================================================
TigerBulletManager::TigerBulletManager(StaticMeshRenderer* _renderer)
{
	renderer = _renderer;
	remaining = MAGAZINE_NUM;
	intervalTimer = 0.0f;
	isShot = false;
}


//=============================================================================
// �f�X�g���N�^
//=============================================================================
TigerBulletManager::~TigerBulletManager()
{
	for (int i = 0; i < bulletList.nodeNum; i++)
	{
		destroy(*bulletList.getValue(i), i);
	}

	// �o���b�g���X�g�̏I������
	bulletList.terminate();
}


//=============================================================================
// �X�V
//=============================================================================
void TigerBulletManager::update(float frameTime)
{
	// ���ˊԊu���Ԃ̍X�V
	if (intervalTimer > 0.0f)
	{
		intervalTimer -= frameTime;
	}

	// �o���b�g�̍X�V
	for (int i = 0; i < bulletList.nodeNum; i++)
	{
		TigerBullet* bullet = *bulletList.getValue(i);
		bullet->update(frameTime);
	}

	// �����_���[�̍X�V
	renderer->updateAccessList();

	// �o���b�g�̍폜
	for (int i = 0; i < bulletList.nodeNum; i++)
	{
		TigerBullet* bullet = *bulletList.getValue(i);

		if (bullet->existenceTimer < 0.0f || bullet->getIsHit())
		{
			destroy(bullet, i);
		}
	}

	// ���X�g�̍X�V
	bulletList.listUpdate();
}


//=============================================================================
// �`��
//=============================================================================
void TigerBulletManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{

}


//=============================================================================
// ����
//=============================================================================
bool TigerBulletManager::shoot(Ray shootingRay)
{
	// ���ˊԊu��������
	if (intervalTimer > 0.0f)
	{
		return false;
	}

	// �c�e���Ɣ��ˊԊu�̐ݒ�
	if (remaining <= 0)
	{
		remaining = MAGAZINE_NUM;
		intervalTimer = RELOAD_TIME;
		return false;
	}
	else
	{
		remaining--;
		intervalTimer = INTERVAL_TIME;
	}

	// �o���b�g�̍쐬�ƕ`��o�^
	TigerBullet* bullet = new TigerBullet(shootingRay);
	bulletList.insertFront(bullet);
	bulletList.listUpdate();
	renderer->registerObject(bullet);
	renderer->updateAccessList();
}


//=============================================================================
// �e�̔j��
//=============================================================================
void TigerBulletManager::destroy(TigerBullet* bullet, int nodeNumber)
{
	// �`���؂�
	renderer->unRegisterObjectByID(bulletList.getNode(nodeNumber)->value->id);

	// �_�u���|�C���^��n���ă|�C���^�m�[�h��j��
	bulletList.remove(bulletList.getNode(nodeNumber));

	// �o���b�g��j��
	SAFE_DELETE(bullet);
}