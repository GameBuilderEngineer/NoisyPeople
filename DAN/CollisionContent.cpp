//-----------------------------------------------------------------------------
// �Փ˓��e�L�q�N���X [CollisionContent.cpp]
// �쐬�J�n�� : 2019/11/9
//-----------------------------------------------------------------------------
#include <cassert>
#include "CollisionContent.h"


//=============================================================================
// �v���C���[�ƃG�l�~�[�Ɠ����蔻��
//=============================================================================
void CollisionContent::playerCollideEnemy(Object* obj1, Object* obj2)
{
	Player* player = castPointerFromTwoObject<Player>(obj1, obj2, objectNS::PLAYER);
	Enemy* enemy = castPointerFromTwoObject<Enemy>(obj1, obj2, objectNS::ENEMY);
	assert(player && enemy);

	D3DXVECTOR3 vecToPlayer = *player->getCentralPosition() - *enemy->getCentralPosition();
	float len = D3DXVec3Length(&vecToPlayer);
	if (len <= (player->radius + enemy->radius) * 0.5f/*�Ƃ肠����*/)
	{
		// �߂荞�ݖ߂�
		if (player->getOnGround())
		{
			player->slip(vecToPlayer, *player->getGroundNormal());
		}
		D3DXVec3Normalize(&vecToPlayer, &vecToPlayer);
		*player->getPosition() += vecToPlayer * ((player->radius + enemy->radius) * 0.5f/*�Ƃ肠����*/ - len);

		// �X�s�[�h�x�N�g���̃X���b�v
		player->setSpeed(player->slip(player->speed, vecToPlayer));

		enemy->setIsHitPlayer(true);
	}
	else
	{
		enemy->setIsHitPlayer(false);
	}
}


//=============================================================================
// �v���C���[�ƃG�l�~�[�̍U���̓����蔻��
//=============================================================================
void CollisionContent::enemyAttacksPlayer(Object* obj1, Object* obj2)
{
	Player* player = castPointerFromTwoObject<Player>(obj1, obj2, objectNS::PLAYER);
	Enemy* enemy = castPointerFromTwoObject<Enemy>(obj1, obj2, objectNS::ENEMY);
	assert(player && enemy);


	//dammy
}
