//===================================================================================================================================
//�yAdvertisement.h�z
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/11/12
//===================================================================================================================================
#pragma once
#include "InstancingBillboard.h"

//=============================================================================
// �C���X�^���X�N���X
//=============================================================================
namespace advetisementNS
{
	class alphaVer :public InstancingBillboardNS::Instance
	{
	public:
		//�R���X�g���N�^
		alphaVer(D3DXVECTOR3 position);
		//�X�V
		virtual void update(float frameTime) override;
	};
}


//=============================================================================
// �����_���[�N���X
//=============================================================================
class Advertisement :public InstancingBillboard
{
private:

public:
	Advertisement();
	~Advertisement();
	void update(float frameTime);
};
