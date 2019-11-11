//===================================================================================================================================
//�yCollisionManager.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/11
// [�X�V��]2019/11/11
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "CollisionManager.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace CollisionManagerNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
CollisionManager::CollisionManager()
{

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
CollisionManager::~CollisionManager()
{

}

//===================================================================================================================================
//�y�Փˏ�������z
//===================================================================================================================================
bool CollisionManager::collision(Object* obj1, Object* obj2)
{
	if (obj1->type == ObjectType::NONE || obj2->type == ObjectType::NONE)return false;

	int type1 = obj1->type;
	int type2 = obj2->type;

	switch (COMBI[type1][type2])
	{
	//PLAYER
	case PLAYER_PLAYER:	return playerAndPlayer((Player*)obj1, (Player*)obj2); break;
	case PLAYER_BULLET: return playerAndBullet((Player*)obj1, (Bullet*)obj2); break;
	case PLAYER_ENEMY: return playerAndEnemy((Player*)obj1, (Enemy*)obj2); break;

	//BULLET
	case BULLET_PLAYER: return playerAndBullet((Player*)obj2, (Bullet*)obj1); break;
	case BULLET_BULLET: break;

	//ENEMY
	case ENEMY_PLAYER:	return playerAndEnemy((Player*)obj2, (Enemy*)obj1); break;

	default:break;
	}

	return false;
}


//===================================================================================================================================
//�y�v���C���[ <-> �v���C���[�z
//===================================================================================================================================
bool CollisionManager::playerAndPlayer(Player* player1, Player* player2)
{
	bool hit = false;
	hit = player1->getBodyCollide()->collide(
		player2->getBodyCollide()->getCenter(), player2->radius,
		player1->matrixWorld, player2->matrixWorld);

	if (hit)
	{
		D3DXVECTOR3 repulsion;//�����x�N�g��
		float length = Base::between2VectorDirection(&repulsion, player1->position, player2->position);
		length = ((player1->radius+player2->radius)-length)/2.0f;
		player1->position -= repulsion * length;
		player2->position += repulsion * length;
		player1->Object::update();
		player2->Object::update();
	}

	return hit;
}


//===================================================================================================================================
//�y�v���C���[ <-> �o���b�g�z
//===================================================================================================================================
bool CollisionManager::playerAndBullet(Player* player, Bullet* bullet)
{
	bool hit = false;



	return hit;
}

//===================================================================================================================================
//�y�v���C���[ <-> �G�l�~�[�z
//===================================================================================================================================
bool CollisionManager::playerAndEnemy(Player* player, Enemy* enemy)
{
	bool hit = false;
	hit = player->getBodyCollide()->collide(
		enemy->getSphereCollider()->getCenter(), enemy->radius,
		player->matrixWorld, enemy->matrixWorld);

	if (hit)
	{
		D3DXVECTOR3 repulsion;//�����x�N�g��
		float length = Base::between2VectorDirection(&repulsion, player->position, enemy->position);
		length = ((player->radius + enemy->radius) - length) / 2.0f;
		player->position -= repulsion * length;
		enemy->position += repulsion * length;
		player->Object::update();
		enemy->Object::update();
	}

	return hit;
}
