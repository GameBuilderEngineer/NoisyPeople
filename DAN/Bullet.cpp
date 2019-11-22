//===================================================================================================================================
//�yBullet.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/05
// [�X�V��]2019/11/11
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Bullet.h"
#include "UtilityFunction.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace bulletNS;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// [�o���b�g�}�l�[�W��]
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region Bullet
//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
Bullet::Bullet(Ray shootingRay)
{
	//�p�����[�^�̏�����
	digitalPower = DIGITAL_POWER;

	//�����̑��
	this->launchPosition	= shootingRay.start;												//���ˈʒu
	this->speed				= shootingRay.direction*SPEED;										//���x
	this->initialCollide	= launchPosition + shootingRay.direction * shootingRay.distance;	//�����Փˈʒu

	//�e���̏�����
	ballisticRay.initialize(launchPosition, shootingRay.direction);
	ballisticRay.color = D3DXCOLOR(0, 255, 120, 255);



	{//�I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ̎w��
		using namespace ObjectType;
		treeCell.type = BULLET;
		treeCell.target = ENEMY | TREE;
	}

	Object::initialize(&launchPosition);							//�o���b�g���f���̏�����
	postureControl(axisZ.direction, shootingRay.direction,1.0f);	//���f����i�s�����֎p�����䂷��
	Object::update();
	existenceTimer = EXIST_TIME;
	endPoint = launchPosition + speed * EXIST_TIME;					//���ɂ��Փ˂��Ȃ������ꍇ�̏I���ʒu
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
Bullet::~Bullet()
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void Bullet::update(float frameTime)
{
	if (existenceTimer <= 0)return;
	existenceTimer -= frameTime;
	//�ʒu�X�V
	D3DXVec3Lerp(&position, &ballisticRay.start, &endPoint,1.0f - existenceTimer/EXIST_TIME);
	Object::update();
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void Bullet::render()
{
	float length = Base::between2VectorLength(ballisticRay.start, position);
	ballisticRay.render(length);
	debugRender();
}

//===================================================================================================================================
//�y�Փ˔���z
//===================================================================================================================================
bool Bullet::collide(LPD3DXMESH targetMesh, D3DXMATRIX targetMatrix)
{
	bool hit = ballisticRay.rayIntersect(targetMesh, targetMatrix);
	
	if (hit)
	{
		hit = (Base::between2VectorLength(ballisticRay.start, position)
		> ballisticRay.distance);
	}

	return hit;
}
//===================================================================================================================================
//�ygetter�z
//===================================================================================================================================
int Bullet::getDigitalPower(){return digitalPower;}
bool Bullet::isCollideInitial() { 
	float initial = Base::between2VectorLength(ballisticRay.start, initialCollide);
	float now = Base::between2VectorLength(ballisticRay.start, position);
	return now >= initial; 
}
D3DXVECTOR3 Bullet::getBulletSpeed() { return this->speed; }


//===================================================================================================================================
//�y�폜�z
//===================================================================================================================================
void Bullet::destroy()
{
	existenceTimer = 0.0f;
	//�T�E���h�̍Đ�
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	PLAY_PARAMETERS hitSE;
	hitSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_HitBulletTree, false ,NULL,false,NULL, true, filterParameters };
	SoundInterface::SE->playSound(&hitSE);
	//�G�t�F�N�g�̍Đ�
	effekseerNS::Instance* instance = new effekseerNS::Instance();
	instance->position = position;
	effekseerNS::play(instance);

}
#pragma endregion

//////////////////////////////////////////////////////////////////////////////////////////////////////
// [�o���b�g�}�l�[�W��]
//////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma region BulletManager
//===================================================================================================================================
//�y�R���X�g���N�^�F�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
BulletManager::BulletManager()
{
	bulletList		= new LinkedList<Bullet*>;
	renderer		= new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::SAMPLE_SCISSORS));
	remaining		= MAGAZINE_NUM;
	intervalTimer	= 0.0f;
	reloadTimer		= 0.0f;
	reloading		= false;
	isLaunched		= false;
	launchFactTime	= 0.0f;

	//�T�E���h�̐ݒ�
	FILTER_PARAMETERS filterParameters = { XAUDIO2_FILTER_TYPE::LowPassFilter, 0.25f, 1.5f };
	shotSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Shot, false ,NULL,false,NULL, true, filterParameters };
	reroadSE = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Reload, false ,NULL,false,NULL, true, filterParameters };
}

//===================================================================================================================================
//�y�f�X�g���N�^�F�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
BulletManager::~BulletManager()
{
	//�����_���[�̉���/�I��
	renderer->allUnRegister();
	SAFE_DELETE(renderer);
	//�o���b�g���X�g�̍폜
	bulletList->terminate();
	SAFE_DELETE(bulletList);
}

//===================================================================================================================================
//�y�X�V�F�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
void BulletManager::update(float frameTime)
{
	//�C���^�[�o������
	if(intervalTimer > 0)intervalTimer -= frameTime;


	//�e�o���b�g�̍X�V
	for (int i = 0; i < bulletList->nodeNum; i++)
	{
		//�o���b�g�̃|�C���^�擾
		Bullet* bullet = (*bulletList->getValue(i));

		//�X�V
		bullet->update(frameTime);

		//�������Ԑ؂�
		if (bullet->existenceTimer <= 0)
		{
			destroy(bullet, i);
		}
		//�����Փˈʒu
		else if(bullet->isCollideInitial())
		{
			bullet->destroy();
			destroy(bullet, i);
		}
	}
	//���X�g�̍X�V[�폜�ɂ��Ή�]
	bulletList->listUpdate();


	//�����_���[�̍X�V
	renderer->update();


	//�����[�h����
	if (reloadTimer > 0)
	{
		reloadTimer -= frameTime;
	}
	else {
		if (reloading)
		{
			reloading = false;			//�����[�h�I��
			remaining = MAGAZINE_NUM;	//���U
		}
	}

	//LAUNCH_FACT_TIME�b�Ŕ��ˎ���������
	if (isLaunched)
	{
		launchFactTime += frameTime;
		if (launchFactTime > LAUNCH_FACT_TIME)
		{
			isLaunched = false;
		}
	}
}

//===================================================================================================================================
//�y�`��F�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
void BulletManager::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition)
{
	//�e�����C�̕`��
	for (int i = 0; i < bulletList->nodeNum; i++)
	{
		(*bulletList->getValue(i))->render();
	}
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPosition);
}

//===================================================================================================================================
//�y���ˁF�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
bool BulletManager::launch(Ray shootingRay)
{
	//�C���^�[�o�����F���˂��Ȃ�
	if (intervalTimer > 0)return false;	
	
	//�����[�h���F���˂��Ȃ�
	if (reloading)return false;
	
	//�c�i����0�F���������[�h
	if (remaining <= 0)
	{
		reload();
		return false;
	}

	//�o���b�g���X�g�֐V���ɐ���
	Bullet* newBullet = new Bullet(shootingRay);

	//���X�g�֒ǉ�
	bulletList->insertFront(newBullet);
	
	//���X�g�̍X�V[�ǉ��ɂ��Ή�]
	bulletList->listUpdate();

	//�����_���[�ւ̓o�^
	renderer->registerObject(newBullet);

	//�c�i���̌��Z
	remaining--;

	//�C���^�[�o�����Ԃ̃Z�b�g
	intervalTimer = INTERVAL_TIME;

	//�T�E���h�̍Đ�
	SoundInterface::SE->playSound(&shotSE);

	//�G�t�F�N�g�̍Đ�
	//effekseerNS::Instance* instance = new effekseerNS::Instance();
	//instance->position = newBullet->position;
	//effekseerNS::play(instance);

	//�Q�[�����ɔ��ˎ������c��
	isLaunched = true;
	launchFactTime = 0.0f;

	return true;
}

//===================================================================================================================================
//�y�����[�h�F�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
void BulletManager::reload()
{
	//�c�i�����ő�F�����[�h���Ȃ�
	if (remaining >= MAGAZINE_NUM)return;
	//�����[�h���F�����[�h���Ȃ�
	if (reloading)return;
	reloading = true;							//�����[�h�J�n
	reloadTimer = RELOAD_TIME;					//�����[�h�^�C���̐ݒ�
	SoundInterface::SE->playSound(&reroadSE);	//�T�E���h�̍Đ�

}

//===================================================================================================================================
//�y�e�폜�F�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
void BulletManager::destroy(Bullet* bullet,int nodeNumber)
{
	//�폜
	renderer->unRegisterObjectByID(bullet->id);//�폜�O�Ƀ����_���[����
	SAFE_DELETE(bullet);
	bulletList->remove(bulletList->getNode(nodeNumber));
}

//===================================================================================================================================
//�ygetter�F�o���b�g�}�l�[�W���[�z
//===================================================================================================================================
int BulletManager::getRemaining(){return remaining;}
float BulletManager::getReloadTime() { return reloadTimer; }
Bullet* BulletManager::getBullet(int i) { return *bulletList->getValue(i); }
int BulletManager::getNum() { return bulletList->nodeNum; }
bool BulletManager::getIsLaunched() { return isLaunched; }
#pragma endregion
