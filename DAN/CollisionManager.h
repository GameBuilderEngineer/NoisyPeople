//===================================================================================================================================
//�yCollisionManager.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/11
// [�X�V��]2019/11/12
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "ObjectTypeList.h"
#include "Object.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Tree.h"
#include "GreeningArea.h"
#include "MapObject.h"
#include "TigerBullet.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace CollisionManagerNS{

}

//===================================================================================================================================
//�y�ՓˊǗ��N���X�z
//===================================================================================================================================
class CollisionManager :public Base
{
public:
	CollisionManager();
	~CollisionManager();

	//�Փ˔���֐�
	static bool collision(Object* obj1, Object* obj2);

	//�Փ˔���
	//���Փ˔���
	static bool collisionSphere(Object* obj1, Object* obj2);
	//���Ɠ_�̏Փ˔���
	//obj1:��<->obj2:�_
	static bool collisionSphereAndPoint(Object* obj1, Object* obj2);
	//�~���Փ˔���i���]�j
	static bool collisionCylinder(Object* obj1, Object* obj2);

	//�A�N�V����
	//�����␳(ratio1 = obj1���␳����銄��(0.0�͓����Ȃ�/1.0�͂��ׂĂ̕␳��������)�j
	static void horizontalCorrection(Object* obj1, Object* obj2,float ratio1);

	//PLAYER
	static bool playerAndPlayer(Player* player1, Player* player2);			//PLAYER<->PLAYER
	static bool playerAndBullet(Player* player, Bullet* bullet);			//PLAYER<->BULLET
	static bool playerAndEnemy(Player* player, Enemy* enemy);				//PLAYER<->ENEMY
	static bool playerAndTree(Player* player, Tree* enemy);					//PLAYER<->TREE
	static bool playerAndMapObject(Player* player, MapObject* mapObject);	//PLAYER<->MAPOBJECT
	static bool playerAndEnemyBullet(Player* player, TigerBullet* bullet);	//PLAYER<->ENEMY_BULLET
	
	//BULLET
	static bool bulletAndBullet(Bullet* bullet1, Bullet* bullet2);		//BULLET<->BULLET
	static bool bulletAndEnemy(Bullet* player, Enemy* enemy);			//BULLET<->ENEMY
	static bool bulletAndTree(Bullet* player, Tree* tree);				//BULLET<->TREE

	//ENEMY
	static bool enemyAndEnemy(Enemy* enemy1, Enemy* enemy2);			//ENEMY<->ENEMY
	static bool enemyAndTree(Enemy* player, Tree* tree);				//ENEMY<->TREE
	
	//TREE
	static bool treeAndTree(Tree* tree1, Tree* tree2);					//ENEMY<->TREE

	//GREENING_AREA
	static bool greeningAreaAndTree(GreeningArea* area, Tree* tree);	//GREENING_AREA<->TREE
};
