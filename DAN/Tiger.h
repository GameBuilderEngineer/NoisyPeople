//-----------------------------------------------------------------------------
// �^�C�K�[�N���X [Tiger.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include "Enemy.h"
#include "TigerAnimation.h"
#include "TigerBullet.h"

//=============================================================================
// ���O���
//=============================================================================
namespace tigerNS
{
	enum PARTS_TYPE
	{
		BODY,		// ����
		LEG_L,		// ����
		LEG_R,		// �E��
		GUN,		// �e��
		PARTS_MAX	// �p�[�c�̐�
	};	
}


//=============================================================================
//�N���X��`
//=============================================================================
class Tiger: public Enemy
{
private:
	Object* parts[tigerNS::PARTS_MAX];			// �p�[�c�I�u�W�F�N�g
	TigerAnimationManager* animationManager;	// �A�j���[�V�����}�l�[�W��
	TigerBulletManager* bulletManager;			// �o���b�g�}�l�[�W��
	D3DXVECTOR3 muzzlePosition;					// ���e���|�W�V����

public:
	Tiger(enemyNS::ConstructionPackage constructionPackage);
	~Tiger();

	void update(float frameTime);				// �X�V
	void chase(float frameTime) override;		// �ǐՃX�e�[�g
	void patrol(float frameTime) override;		// �x���X�e�[�g
	void rest(float frameTime) override;		// �x�e�X�e�[�g
	void attackTree(float frameTime) override;	// �c���[�U���X�e�[�g
	void die(float frameTime) override;			// ���S�X�e�[�g

	void shot(Player* target);

	// Getter
	Object* getParts(int type);

	// Setter
};
