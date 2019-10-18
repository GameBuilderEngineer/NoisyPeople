//-----------------------------------------------------------------------------
// ���^AI�C�x���g���s�N���X [OperationGenerator.cpp]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/12
//-----------------------------------------------------------------------------
#include "AIDirector.h"
#include "OperationGenerator.h"
using namespace aiNS;


//=============================================================================
// ������
//=============================================================================
void OperationGenerator::initialize(PLAYERAD* _playerAD, ENEMYAD* _enemyAD, TREEAD* _treeAD, ITEMAD* _itemAD)
{
	playerAD = _playerAD;
	enemyAD = _enemyAD;
	treeAD = _treeAD;
	itemAD = _itemAD;
}


//=============================================================================
// �I������
//=============================================================================
void OperationGenerator::uninitialize()
{
}


//=============================================================================
// �X�V����
//=============================================================================
void OperationGenerator::update()
{
	treeAD->numBeingAttackedTree = 5;
}
