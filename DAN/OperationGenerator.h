//-----------------------------------------------------------------------------
// ���^AI�C�x���g���s�N���X [OperationGenerator.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#pragma once

//=============================================================================
// ���O���
//=============================================================================
namespace aiNS
{
	// �O���錾
	struct	PlayerAnalyticalData;
	typedef PlayerAnalyticalData PLAYERAD;
	struct	EnemyAnalyticalData;
	typedef EnemyAnalyticalData	ENEMYAD;
	struct	TreeAnalyticalData;
	typedef TreeAnalyticalData TREEAD;
	struct	ItemAnalyticalData;
	typedef ItemAnalyticalData ITEMAD;
}


//=============================================================================
//�N���X��`
//=============================================================================
class OperationGenerator
{
private:
	aiNS::PLAYERAD* playerAD;
	aiNS::ENEMYAD* enemyAD;
	aiNS::TREEAD* treeAD;
	aiNS::ITEMAD* itemAD;

public:
	void initialize(aiNS::PLAYERAD* _playerAD, aiNS::ENEMYAD* _enemyAD,
		aiNS::TREEAD* _treeAD, aiNS::ITEMAD* _itemAD);
	void uninitialize();
	void update();
};
