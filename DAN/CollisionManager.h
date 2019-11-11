//===================================================================================================================================
//�yCollisionManager.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/11
// [�X�V��]2019/11/11
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


//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace CollisionManagerNS{
	//�g�������X�g
	enum COLLISION_COMBI{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,

		COMBI_NUM
	};

	const int COMBI[ObjectType::TYPE_NUM][ObjectType::TYPE_NUM]=
	{
		//NONE
		NONE_NONE,
		NONE_PLAYER,
		NONE_BULLET,

		//PLAYER
		PLAYER_NONE,
		PLAYER_PLAYER,
		PLAYER_BULLET,

		//BULLET
		BULLET_NONE,
		BULLET_PLAYER,
		BULLET_BULLET,

	};




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

	//PLAYER
	static bool playerAndPlayer(Player* player1, Player* player2);		//PLAYER<->PLAYER
	static bool playerAndBullet(Player* player, Bullet* bullet);		//PLAYER<->BULLET
	
	//BULLET
	static bool bulletAndPlayer(Player* player, Bullet* bullet);		//PLAYER<->BULLET
	//static bool bulletAndBullet(Player* player, Bullet* bullet);		//BULLET<->BULLET


};
