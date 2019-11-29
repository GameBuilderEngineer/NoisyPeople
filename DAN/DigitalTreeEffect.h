//===================================================================================================================================
//�yDigitalTreeEffect.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/25
// [�X�V��]2019/11/25
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "InstancingBillboard.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace DigitalTreeEffectNS
{
	//===================================================================================================================================
	//�y�G�t�F�N�g�C���X�^���X�N���X�z
	//===================================================================================================================================
	class EffectInstance :public InstancingBillboardNS::Instance
	{
	public:
		//�R���X�g���N�^
		EffectInstance(D3DXVECTOR3 position);
		//�X�V
		virtual void update(float frameTime) override;
	};

	//===================================================================================================================================
	//�y�G�t�F�N�g�C���X�^���X�N���X�z
	//===================================================================================================================================
	//�f�W�^���c���[�W���G�t�F�N�g
	class DigitTree :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DigitTree(D3DXVECTOR3* sync) {
			syncPosition = sync;
			effectNo = effekseerNS::DIGIT_TREE;}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};
	//�f�W�^���c���[�P���G�t�F�N�g
	class DigitTreeRaid :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3 * syncPosition;
		DigitTreeRaid(D3DXVECTOR3* sync) {
			syncPosition = sync;
			effectNo = effekseerNS::DIGIT_TREE_RAID;}
		virtual void update() {
			position = *syncPosition;
			Instance::update();
		};
	};

}


//===================================================================================================================================
//�y�f�W�^���c���[�G�t�F�N�g�N���X�z
//===================================================================================================================================
class DigitalTreeEffect :public Base
{
private:
	InstancingBillboard* billboard;

public:
	DigitalTreeEffect();
	~DigitalTreeEffect();

	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateInstance(D3DXVECTOR3 position);
	void playStandardEffect(D3DXVECTOR3* position);
	void playRaidEffect(D3DXVECTOR3* position);

};

