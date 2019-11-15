//-----------------------------------------------------------------------------
// �x�A�[�N���X [Bear.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

//=============================================================================
//�N���X��`
//=============================================================================
class Bear: public Enemy
{
private:

public:
	Bear(StaticMesh* _staticMesh, enemyNS::EnemyData* _enemyData);
	~Bear();

	void update(float frameTime);			// �X�V
	void chase(float frameTime) override;	// �ǐՃX�e�[�g
	void patrol(float frameTime) override;	// �x���X�e�[�g
	void rest(float frameTime) override;	// �x�e�X�e�[�g
	void die(float frameTime) override;		// ���S�X�e�[�g

	// Getter

	// Setter
};
