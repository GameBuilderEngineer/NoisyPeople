//-----------------------------------------------------------------------------
// �x�A�[�N���X [Bear.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

//=============================================================================
// ���O���
//=============================================================================
namespace bearNS
{
	enum PARTS_TYPE
	{
		BODY,		// ����
		ARM_L,		// ���r
		ARM_R,		// �E�r
		WAIST,		// ��
		LEG_L,		// ����
		LEG_R,		// �E��
		PARTS_MAX	// �p�[�c�̐�
	};

	const D3DXVECTOR3 PARTS_OFFSET_POS[PARTS_MAX] =
	{
		D3DXVECTOR3(0.0f, 21.06f, 0.0f),	// ����
		D3DXVECTOR3(-6.46f, 28.71f, -1.45f),// ���r
		D3DXVECTOR3(6.46f, 28.71f, -1.45f),	// �E�r
		D3DXVECTOR3(0.0f, 21.06f, 0.0f),	// ��
		D3DXVECTOR3(-1.99f, 16.01f, -1.05f),// ����
		D3DXVECTOR3(1.99f, 16.01f, -1.05f),	// �E��
	};
}


//=============================================================================
//�N���X��`
//=============================================================================
class Bear: public Enemy
{
private:
	Object* parts[bearNS::PARTS_MAX];			// �p�[�c�I�u�W�F�N�g
	// ���p�[�c�I�u�W�F�N�g��Object�N���X�̍X�V�������s��Ȃ�.
	// �����[���h�ϊ����̏����̓A�j���[�V�����}�l�[�W������ւ���.

public:
	Bear(enemyNS::ConstructionPackage constructionPackage);
	~Bear();

	void update(float frameTime);				// �X�V
	void chase(float frameTime) override;		// �ǐՃX�e�[�g
	void patrol(float frameTime) override;		// �x���X�e�[�g
	void rest(float frameTime) override;		// �x�e�X�e�[�g
	void attackTree(float frameTime) override;	// �c���[�U���X�e�[�g
	void die(float frameTime) override;			// ���S�X�e�[�g

	// Getter
	Object* getParts(int type);

	// Setter
};
