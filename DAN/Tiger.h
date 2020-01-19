//-----------------------------------------------------------------------------
// �^�C�K�[�N���X [Tiger.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include "Enemy.h"
#include "TigerBullet.h"

//=============================================================================
// ���O���
//=============================================================================
namespace tigerNS
{
	// �p�[�c�̎��
	enum PARTS_TYPE
	{
		BODY,		// ����
		GUN,		// �e��
		LEG_L,		// ����
		LEG_R,		// �E��
		PARTS_MAX	// �p�[�c�̐�
	};	

	// �p�[�c�֌W�̃I�t�Z�b�g���W
	const D3DXVECTOR3 PARTS_OFFSET_POS[PARTS_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ����
		D3DXVECTOR3(0.0f, 2.71f, -0.94f),	// �e
		D3DXVECTOR3(0.85f, 1.84f, -0.62f),	// ����
		D3DXVECTOR3(-0.85f, 1.84f, -0.62f),	// �E��
	};
	//const D3DXVECTOR3 MUZZLE_POSITION = D3DXVECTOR3(0.0f, 3.01f, -2.47f);
	const D3DXVECTOR3 MUZZLE_POSITION = D3DXVECTOR3(0.0f, 0.28f, 3.38f);
}


//=============================================================================
//�N���X��`
//=============================================================================
class Tiger: public Enemy
{
private:
	enemyNS::EnemyParts* parts[tigerNS::PARTS_MAX];
	// ���p�[�c�I�u�W�F�N�g��Object�N���X�̍X�V�������s��Ȃ�.
	// �����[���h�ϊ����̏����̓A�j���[�V�����}�l�[�W������ւ���.

	TigerBulletManager* bulletManager;			// �o���b�g�}�l�[�W��

public:
	Tiger(enemyNS::ConstructionPackage constructionPackage);
	~Tiger();

	void update(float frameTime);				// �X�V
	void chase(float frameTime) override;		// �ǐՃX�e�[�g
	void patrol(float frameTime) override;		// �x���X�e�[�g
	void rest(float frameTime) override;		// �x�e�X�e�[�g
	void attackTree(float frameTime) override;	// �c���[�U���X�e�[�g
	void die(float frameTime) override;			// ���S�X�e�[�g

	void shot();

	// Getter
	enemyNS::EnemyParts* getParts(int type);
	TigerBulletManager* getBulletMangaer();

	// Setter
};
