//-----------------------------------------------------------------------------
// �E���t�N���X [Wolf.h]
// Author : HAL�������ԕ� 2�N���Q�[���w�� GP12A332 32 �����a�P
// �쐬�J�n�� : 2019/10/21
//-----------------------------------------------------------------------------
#pragma once
#include"Enemy.h"

namespace wolfNS
{
	enum PARTS_TYPE
	{
		BODY,		// ����
		ARM,		// �r
		PARTS_MAX	// �p�[�c�̐�
	};

	const D3DXVECTOR3 PARTS_OFFSET_POS[PARTS_MAX] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// ����
		D3DXVECTOR3(0.0f, 1.00f, -0.264f),	// �r
	};

}


//=============================================================================
// �N���X��`
//=============================================================================
class Wolf: public Enemy
{
private:
	Object* parts[wolfNS::PARTS_MAX];			// �p�[�c�I�u�W�F�N�g
// ���p�[�c�I�u�W�F�N�g��Object�N���X�̍X�V�������s��Ȃ�.
// �����[���h�ϊ����̏����̓A�j���[�V�����}�l�[�W������ւ���.
	// Sound
	PLAY_PARAMETERS playParmeters[2];

public:
	Wolf(enemyNS::ConstructionPackage constructionPackage);
	~Wolf();

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
