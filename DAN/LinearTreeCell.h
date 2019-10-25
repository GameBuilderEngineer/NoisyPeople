//===================================================================================================================================
//�yLinearTreeCell.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/23
// [�X�V��]2019/10/23
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include <vector>
#include <list>

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace std;

//===================================================================================================================================
//�y��ԃN���X�z
//===================================================================================================================================
template <class T>
class Cell;

//===================================================================================================================================
//�y���ؓo�^�I�u�W�F�N�g�z
//===================================================================================================================================
template<class T>
class ObjectTree
{
public:
	Cell<T>*		cell;	//�o�^���
	T*				object;	//����ΏۃI�u�W�F�N�g
	ObjectTree*		prev;	//�O��ObjectTree�N���X�|�C���^
	ObjectTree*		next;	//����ObjectTree�N���X�|�C���^
public:
	//�R���X�g���N�^
	ObjectTree()
	{
		cell	= NULL;
		object	= NULL;
		prev	= NULL;
		next	= NULL;
	}
	
	//�f�X�g���N�^
	virtual ~ObjectTree()
	{

	}
public:
	//���烊�X�g����O���
	bool remove()
	{
		//���łɈ�E���Ă��鎞�͏����I��
		if (!cell)return false;

		//������o�^���Ă����ԂɎ��g��ʒm
		if (cell->onRemove(this))return false;

		//��E����
		//�O��̃I�u�W�F�N�g���т���
		if (prev != NULL)
		{
			prev->next = next;
			prev = NULL;
		}
		if (next != NULL)
		{
			next->prev = prev;
			next = NULL;
		}
		cell = NULL;
		return true;
	}

	//��Ԃ�o�^
	void registerCell(Cell<T>* cell)
	{

	}

	//���̃I�u�W�F�N�g�ւ̃|�C���^���擾
	ObjectTree<T>* getNextObj()
	{
		return next;
	}

};

//===================================================================================================================================
//�y���`�S���ؖ��O��ԁz
//===================================================================================================================================
namespace linear4TreeNS
{
	const int MAX_LEVEL = 9;	//�ő啪�����x��
}

//===================================================================================================================================
//�y���`�S���؊Ǘ��N���X�z
//===================================================================================================================================
template <class T>
class Linear4TreeManager
{
protected:
	unsigned int		dim;
	Cell<T>**			cellArray;							//���`��ԃ|�C���^�z��
	unsigned int		pow[linear4TreeNS::MAX_LEVEL + 1];	//�ׂ��搔�l�z��
	float				width;								//�̈��X����
	float				height;								//�̈��Y����
	float				left;								//�̈�̍���(X���ŏ��l)
	float				top;								//�̈�̏㑤(Y���ŏ��l)
	float				unitWidth;							//�ŏ����x����Ԃ̕��P��
	float				unitHeight;							//�ŏ����x����Ԃ̍����P��
	DWORD				cellNum;							//��Ԃ̐�
	unsigned int		level;								//�ŉ��ʃ��x��
public:
	//�R���X�g���N�^
	Linear4TreeManager()
	{
		level		= 0;
		width		= 0.0f;
		height		= 0.0f;
		left		= 0.0f;
		top			= 0.0f;
		unitWidth	= 0.0f;
		unitHeight	= 0.0f;
		cellNum		= 0.0f;
		cellArray	= NULL;
		dim			= 0;
	}

	//�f�X�g���N�^
	virtual ~Linear4TreeManager()
	{
		for (DWORD i = 0; i < cellNum; i++)
		{
			SAFE_DELETE(cellArray[i]);
		}
		SAFE_DELETE_ARRAY(cellArray);
	}

	//���`�S���ؔz����\�z����
	bool initialize(unsigned int level, float left, float top, float right, float bottom)
	{
		//�ݒ�ō����x���ȏ�̋�Ԃ͍��Ȃ�
		if (level >= linear4TreeNS::MAX_LEVEL)return false;

		//�e���x���ł̋�Ԑ����Z�o
		pow[0] = 1;
		for (int i = 1; i < linear4TreeNS::MAX_LEVEL+1; i++)
		{
			pow[i] = pow[i - 1] * 4;
		}

		//level���x��(0��_)�̔z��쐬
		cellNum		= (pow[level + 1] - 1) / 3;
		cellArray	= new Cell<T>*[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>)*cellNum);

		//�̈��o�^
		this->left			= left;
		this->top			= top;
		this->width			= right - left;
		this->height		= bottom - top;
		this->unitWidth		= width / (1 << level);
		this->unitHeight	= height / (1 << level);
		this->level			= level;

		return true;

	}

	//�I�u�W�F�N�g��o�^����
	bool registObject(float left, float top, float right, float bottom, ObjectTree<T>* objectTree)
	{
		//�I�u�W�F�N�g�̋��E�͈͂���o�^���[�g���ԍ����Z�o
		DWORD element = getMortonNumber(left, top, right, bottom);
		if (element < cellNum)
		{
			//��Ԃ������ꍇ�͐V�K�쐬
			if (!cellArray[element])
			{
				createNewCell(element);
			}
			return cellArray[element]->push(objectTree);
		}
		return false;
	}

	//�Փ˔��胊�X�g���쐬����
	DWORD getAllCollisionList(vector<T*> &collisionVector)
	{
		//���X�g�i�z��j�͕K������������
		collisionVector.clear();

		//���[�g��Ԃ̑��݂��`�F�b�N
		if (cellArray[0] == NULL)	return 0;	//��Ԃ����݂��Ă��Ȃ�

		//���[�g��Ԃ�����
		list<T*> collisionStac;
		getCollisionList(0, collisionVector, collisionStac);

		return (DWORD)collisionVector.size();

	}

protected:
	//��ԓ��ŏՓ˃��X�g���쐬����
	bool getCollisionList(DWORD element, vector<T*> &collisionVector, list<T*> &collisionStac)
	{
		typename list<T*>::iterator it;
		//�@��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
		ObjectTree<T>* objectTree1 = cellArray[element]->getFirstObject();
		while (objectTree1 != NULL)
		{
			ObjectTree<T>* objectTree2 = objectTree1->next;
			while (objectTree2 != NULL)
			{
				//�Փ˃��X�g�쐬
				collisionVector.push_back(objectTree1->object);
				collisionVector.push_back(objectTree2->object);
				objectTree2 = objectTree2->next;
			}
			//�A�Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
			for (it = collisionStac.begin(); it != collisionStac.end(); it++)
			{
				collisionVector.push_back(objectTree1->object);
				collisionVector.push_back(*it);
			}
			objectTree1 = objectTree1->next;
		}

		bool childFlag = false;
		//�B�q��ԂɈړ�
		DWORD objectNum = 0;
		DWORD i, nextElement;
		for (i = 0; i < 4; i++)
		{
			nextElement = element * 4 + 1 * i;
			if (nextElement < cellNum 
				&& cellArray[element * 4 + 1 + i])
			{
				if (!childFlag)
				{
					//�C�o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
					objectTree1 = cellArray[element]->getFirstObject();
					while (objectTree1)
					{
						collisionStac.push_back(objectTree1->object);
						objectNum++;
						objectTree1 = objectTree1->next;
					}
				}
				childFlag = true;
				getCollisionList(element * 4 + 1 + i, collisionVector, collisionStac);	//�q��Ԃ�
			}
		}

		//�D�X�^�b�N����I�u�W�F�N�g���O��
		if (childFlag)
		{
			for (i = 0; i < objectNum; i++)
			{
				collisionStac.pop_back();
			}
		}

		return true;

	}

	//��Ԃ𐶐�
	bool createNewCell(DWORD element)
	{
		//�����̗v�f�ԍ�
		while (!cellArray[element])
		{
			//�w��̗v�f�ԍ��ɋ�Ԃ�V�K�쐬
			cellArray[element] = new Cell<T>;

			//�e��ԂɃW�����v
			element = (element - 1) >> 2;
			if (element >= cellNum)break;

		}
		return true;
	}

	//���W�����Ԕԍ����Z�o
	DWORD getMortonNumber(float left, float top, float right, float bottom)
	{
		//�ŏ����x���ɂ�����e���ʒu�Z�o
		DWORD leftTop = getPointElement(left, top);
		DWORD rightBottom = getPointElement(right, bottom);

		//��Ԕԍ��������Z����
		//�ŏ�ʋ�؂肩�珊�����x�����Z�o
		DWORD difference = rightBottom - leftTop;
		unsigned int hiLevel = 0;
		unsigned int i;
		for (i = 0; i < level; i++)
		{
			DWORD check = (difference >> (i * 2)) & 0x3;
			if (check != 0)
			{
				hiLevel = i + 1;
			}
			DWORD spaceNum = rightBottom >> (hiLevel * 2);
			DWORD addNum = (pow[level - hiLevel] - 1) / 3;
			spaceNum += addNum;

			if (spaceNum > cellNum)
				return 0xffffffff;

			return spaceNum;
		}
	}

	//�r�b�g�����֐�
	DWORD bitSeparate32(DWORD n)
	{
		n = (n | (n << 8)) & 0x00ff00ff;
		n = (n | (n << 4)) & 0x0f0f0f0f;
		n = (n | (n << 2)) & 0x33333333;
		return (n | (n << 1)) & 0x55555555;
	}

	//2D���[�g����Ԕԍ��Z�o�֐�
	WORD get2DMortonNumber(WORD x, WORD y)
	{
		return (WORD)(bitSeparate32(x) | (bitSeparate32(y) << 1));
	}

	//���W�����`4���ؗv�f�ԍ��ϊ��֐�
	DWORD getPointElement(float x, float y)
	{
		return get2DMortonNumber((WORD)((x - left) / unitWidth), (WORD)((y - top) / unitHeight));
	}
};

//===================================================================================================================================
//�y��ԃN���X�z
//template<class T>
//class Cell
//{
//protected:
//	ObjectTree<T>* latest;	//	�ŐVObjectTree�ւ̃|�C���^
//
//public:
//	Cell();				//�R���X�g���N�^
//	virtual ~Cell();	//�f�X�g���N�^
//
//	void resetLink(ObjectTree<T>* &objectTree);	//�����N��S�ă��Z�b�g����
//
//	bool push(ObjectTree<T>* &objectTree);	//ObjectTree���v�b�V��
//
//	ObjectTree<T>* getFirstObject();	//�ŏ��̃I�u�W�F�N�g�c���[���擾
//
//	bool onRemove(ObjectTree<T>* removeObject);	//�폜�����I�u�W�F�N�g�`�F�b�N
//}
//===================================================================================================================================
template<class T>
class Cell
{
protected:
	ObjectTree<T>* latest;	//	�ŐVObjectTree�ւ̃|�C���^

public:
	//�R���X�g���N�^
	Cell(){}

	//�f�X�g���N�^
	virtual ~Cell()
	{
		if (latest != NULL)
			resetLink(latest);
	}

	//�����N��S�ă��Z�b�g����
	void resetLink(ObjectTree<T>* &objectTree)
	{
		if (objectTree->next != NULL)
		{
			resetLink(objectTree->next);
		}
		objectTree = NULL;	//NULL�Ń��Z�b�g
	}

	//ObjectTree���v�b�V��
	bool push(ObjectTree<T>* &objectTree)
	{
		if (objectTree == NULL)			return false;	//�����I�u�W�F�N�g�͓o�^���Ȃ�
		if (objectTree->cell == this)	return false;	//�Q�d�o�^�`�F�b�N
		if (latest == NULL)
		{
			latest = objectTree;						//��ԂɐV�K�o�^
		}
		else
		{
			//�ŐVObjectTree�I�u�W�F�N�g���X�V
			objectTree->next	= latest;
			latest->prev		= objectTree;
			latest				= objectTree;
		}
		objectTree->registerCell(this);					//��Ԃ�o�^
		return true;
	}

	ObjectTree<T>* getFirstObject()
	{
		return latest;
	}

	//�폜�����I�u�W�F�N�g�`�F�b�N
	bool onRemove(ObjectTree<T>* removeObject)
	{
		if (latest == removeObject)
		{
			//���̃I�u�W�F�N�g�ɑ}���ւ�
			if (latest != NULL)
				latest = latest->getNextObj();
		}
		return true;
	}
};