//===================================================================================================================================
//�yBearGauge.h�z
// [�쐬��]HAL����GP12A332 32�@�����a�P
// [�쐬��]2020/1/19
//===================================================================================================================================
#pragma once
#include "InstancingBillboard.h"

namespace BearGaugeNS
{
	enum TYPE
	{
		HP,
		LEFT_ARM,
		RIGHT_ARM,
		TYPE_MAX
	};

	const float SCALE_X = 5.0f;

	class BearGaugeInstance :public InstancingBillboardNS::Instance
	{
	private:
		int hitPointMax;
		D3DXVECTOR3 offSet;
	public:
		//�R���X�g���N�^
		BearGaugeInstance(D3DXVECTOR3 _position, int _hitPointMax);
		//�X�V
		void update(float frameTime, D3DXMATRIX* _worldMatrix, int hitPoint);
	};
}

class BearGauge :public InstancingBillboard
{
private:
	BearGaugeNS::BearGaugeInstance* instancePointer[BearGaugeNS::TYPE_MAX];
	static BearGauge* pointer;

public:
	BearGauge(int hitPoint1, int hitPoint2, int hitPoint3);
	~BearGauge();
	void update(float frameTime);
	BearGaugeNS::BearGaugeInstance* getInstance(int type) { return instancePointer[type]; }
	static BearGauge* get() { return pointer; }
};
