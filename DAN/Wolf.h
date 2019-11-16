//-----------------------------------------------------------------------------
// �E���t�N���X [Wolf.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

//=============================================================================
//�N���X��`
//=============================================================================
class Wolf: public Enemy
{
private:

public:
	Wolf(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData);
	~Wolf();

	void update(float frameTime);				// �X�V
	void chase(float frameTime) override;		// �ǐՃX�e�[�g
	void patrol(float frameTime) override;		// �x���X�e�[�g
	void rest(float frameTime) override;		// �x�e�X�e�[�g
	void attackTree(float frameTime) override;	// �c���[�U���X�e�[�g
	void die(float frameTime) override;			// ���S�X�e�[�g

	// Getter

	// Setter
};
