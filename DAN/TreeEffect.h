//===================================================================================================================================
//�yTreeEffect.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/11/25
// [�X�V��]2019/12/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "InstancingBillboard.h"
#include "EffekseerManager.h"
#include "Tree.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace TreeEffectNS
{
	//===================================================================================================================================
	//�y�c���[��ԃ}�[�J�[�C���X�^���X�N���X�z
	//===================================================================================================================================
	class MarkerInstance :public InstancingBillboardNS::Instance
	{
	public:
		Tree* tree;
	public:
		//�R���X�g���N�^
		MarkerInstance(Tree* owner);
		//�X�V
		virtual void update(float frameTime) override;
		void changeColor();
	};

	//===================================================================================================================================
	//�y�c���[�W���C���X�^���X�N���X�z
	//===================================================================================================================================
	class SignInstance :public InstancingBillboardNS::Instance
	{
	public:
		Tree* tree;
	public:
		//�R���X�g���N�^
		SignInstance(Tree* owner);
		//�X�V
		virtual void update(float frameTime) override;
		void changeColor();
	};

	enum {
		INSTANCE_MARKER,
		INSTANCE_SIGN,
	};

	//===================================================================================================================================
	//�y�G�t�F�N�g�C���X�^���X�N���X�z
	//===================================================================================================================================

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
	//�G�t�F�N�g��������
	const float EFFECT_CYCLE = 0.1f;

}


//===================================================================================================================================
//�y�f�W�^���c���[�G�t�F�N�g�N���X�z
//===================================================================================================================================
class TreeEffect :public Base
{
private:
	InstancingBillboard* marker;	//��ԃ}�[�J�[
	InstancingBillboard* sign;		//
	float effectCycle;							//�G�t�F�N�g����



public:
	TreeEffect();
	~TreeEffect();

	void update(float frameTime);
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateInstance(int type,Tree* position);
	void resetEffectCycle();
	void playStandardEffect(D3DXVECTOR3* position);
	void playRaidEffect(int no,D3DXVECTOR3* position);

	void enableRender();
	void disableRender();

};

