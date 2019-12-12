//-----------------------------------------------------------------------------
// �G�l�~�[�ǐՃ}�[�N[EnemyChaseMark.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/12/10
//-----------------------------------------------------------------------------
#pragma once
#include "InstancingBillboard.h"

//=============================================================================
// �C���X�^���X�N���X
//=============================================================================
namespace enemyChaseMarkNS
{
	class StandardChaseMark :public InstancingBillboardNS::Instance
	{
	public:
		static int cntInstance;	// �ꉞ�J�E���g�@�g��Ȃ�����

		//�R���X�g���N�^
		StandardChaseMark(D3DXVECTOR3 position);
		//�X�V
		virtual void update(float frameTime) override;
	};
}


//=============================================================================
// �����_���[�N���X
//=============================================================================
class EnemyChaseMark :public InstancingBillboard
{
private:

public:
	EnemyChaseMark();
	~EnemyChaseMark();
	void update(float frameTime);

	// �C���X�^���X�����ƍ폜�̃��b�p�[�֐�
	void generateMark(enemyChaseMarkNS::StandardChaseMark* mark);
	void deleteMark(enemyChaseMarkNS::StandardChaseMark* mark);
};
