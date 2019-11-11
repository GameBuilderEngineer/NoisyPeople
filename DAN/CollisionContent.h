//-----------------------------------------------------------------------------
// �Փ˓��e�L�q�N���X [CollisionContent.h]
// �쐬�J�n�� : 2019/11/9
//-----------------------------------------------------------------------------
#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include "TreeManager.h"
#include "ItemManager.h"

//=============================================================================
// CollisionContent�N���X�͏Փ˔���̏Փˏ������̂�S������
//=============================================================================
class CollisionContent
{
private:
	Object* obj[2];

	// �����Ƃ��ēn����2�̃I�u�W�F�N�g�^�|�C���^�̂�����3�����Ŏw�肳�ꂽ�^��
	// �L���X�g�ł���|�C���^�����ۂɂ��̕��ɃL���X�g���ĕԋp�l�Ƃ��郁�\�b�h
	template <typename T>
	T* castPointerFromTwoObject(Object* obj1, Object* obj2, DWORD objectType)
	{
		obj[0] = obj1, obj[1] = obj2;
		for (int i = 0; i < 2; i++)
		{
			if (obj[i]->objectType == objectType)
			{
				return (T*)(obj[i]);
			}
		}
		return NULL;
	}

public:
	// �v���C���[�ƃG�l�~�[�Ɠ����蔻��
	void playerCollideEnemy(Object* obj1, Object* obj2);
	// �v���C���[�ƃG�l�~�[�̍U���̓����蔻��
	void enemyAttacksPlayer(Object* obj1, Object* obj2);
};

