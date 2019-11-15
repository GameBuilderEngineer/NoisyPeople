//===================================================================================================================================
//�yLinearTreeCell.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/23
// [�X�V��]2019/11/11
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "Direct3D9.h"
#include "ShaderLoader.h"
#include "Ray.h"
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
//�y���ؓo�^�I�u�W�F�N�g�N���X�z
//class ObjectTree
//{
//public:
//	Cell<T>*			cell;			//�o�^���
//	T*					object;			//����ΏۃI�u�W�F�N�g
//	ObjectTree<T>*		prev;			//�O��ObjectTree�N���X�|�C���^
//	ObjectTree<T>*		next;			//����ObjectTree�N���X�|�C���^
//public:
//	ObjectTree()						//�R���X�g���N�^
//	virtual ~ObjectTree()				//�f�X�g���N�^
//public:
//	bool remove()						//���烊�X�g����O���
//	void registerCell(Cell<T>* cell)	//��Ԃ�o�^
//	ObjectTree<T>* getNextObj()			//���̃I�u�W�F�N�g�ւ̃|�C���^���擾
//};
//===================================================================================================================================
#pragma region ObjectTree
template<class T>
class ObjectTree
{
public:
	Cell<T>*			cell;	//�o�^���
	T*					object;	//����ΏۃI�u�W�F�N�g
	ObjectTree<T>*		prev;	//�O��ObjectTree�N���X�|�C���^
	ObjectTree<T>*		next;	//����ObjectTree�N���X�|�C���^
	int					type;	//���g�̃I�u�W�F�N�g�^�C�v
	int					target;	//�Փ˂��ׂ��I�u�W�F�N�g�^�C�v
public:
	//�R���X�g���N�^
	ObjectTree()
	{
		cell	= NULL;
		object	= NULL;
		prev	= NULL;
		next	= NULL;
		type	= 0;
		target	= 0;
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
		if (!cell->onRemove(this))return false;

		//��E����
		//�O��̃I�u�W�F�N�g���т���
		if (prev != 0)
		{
			prev->next = next;
		}
		if (next != 0)
		{
			next->prev = prev;
		}
		prev = 0;
		next = 0;
		cell = NULL;
		return true;
	}

	//��Ԃ�o�^
	void registerCell(Cell<T>* cell)
	{
		this->cell = cell;
	}

	//���̃I�u�W�F�N�g�ւ̃|�C���^���擾
	ObjectTree<T>* getNextObj()
	{
		return next;
	}

};
#pragma endregion

//===================================================================================================================================
//�y�Փ˃��X�g�z
//�������m�ی�A���X�g���̂��̂��������܂ŁA����������������ێ�����B
//class CollisionList
//{
//private:
//	T**		root;			//���X�g���[�g�|�C���^
//	size_t	pos;			//�������݃|�C���^
//	size_t	mallocSize;		//�������m�ۃT�C�Y
//
//public:
//	CollisionList() :root(0), pos(0), mallocSize(0)		//�R���X�g���N�^
//	~CollisionList()									//�f�X�g���N�^
//
//	size_t getSize()									//�T�C�Y�擾
//	T** getRoot()										//���[�g�|�C���^�擾
//	void resetPos()										//�������|�C���^�̃��Z�b�g
//	void write(T* obj1, T* obj2)						//��������
//	void reflesh()										//�������
//};
//===================================================================================================================================
#pragma region CollisionList
#define COLLISIONLIST_REALLOCSIZE	100
template <class T>
class CollisionList
{
public:
	//�R���X�g���N�^
	CollisionList() :root(0), pos(0), mallocSize(0)
	{
		root = (T**)malloc(0);
	}
	//�f�X�g���N�^
	~CollisionList()
	{
		reflesh();
	}
	//�T�C�Y�擾
	size_t getSize()
	{
		return pos;
	}
	//���[�g�|�C���^�擾
	T** getRoot()
	{
		return root;
	}
	//�������|�C���^�̃��Z�b�g
	void resetPos()
	{
		pos = 0;
	}
	//��������
	void write(T* obj1, T* obj2) {
		if (pos >= mallocSize)
		{
			root = (T**)realloc(root, sizeof(T*)*(mallocSize + COLLISIONLIST_REALLOCSIZE));
			mallocSize += COLLISIONLIST_REALLOCSIZE;
		}
		root[pos++] = obj1;
		root[pos++] = obj2;
	}
	//�������
	void reflesh()
	{
		if (root)
		{
			free(root);
		}
	}
private:
	T**		root;			//���X�g���[�g�|�C���^
	size_t	pos;			//�������݃|�C���^
	size_t	mallocSize;		//�������m�ۃT�C�Y
};
#pragma endregion

//===================================================================================================================================
//�y���`�S���ؖ��O��ԁz
//===================================================================================================================================
namespace linear4TreeNS
{
	//�Z�o���F(2<<Level+1)^2/3
	//���x��0��Ԑ��F1
	//���x��1��Ԑ��F5
	//���x��2��Ԑ��F21
	//���x��3��Ԑ��F85
	//���x��4��Ԑ��F341
	//���x��5��Ԑ��F1,365
	//���x��6��Ԑ��F5,461
	//���x��7��Ԑ��F21,845
	//���x��8��Ԑ��F87,381
	//���x��9��Ԑ��F349,525
	const int MAX_LEVEL = 9;	//�ő啪�����x��
}

//===================================================================================================================================
//�y���`�S���؊Ǘ��N���X�z
//class Linear4TreeManager :public Base
//{
//protected:
//	Cell<T>**						cellArray;															//���`��ԃ|�C���^�z��
//	unsigned int					pow[linear4TreeNS::MAX_LEVEL + 1];									//�ׂ��搔�l�z��
//	float							width;																//�̈��X����
//	float							height;																//�̈��Y����
//	float							left;																//�̈�̍���(X���ŏ��l)
//	float							top;																//�̈�̏㑤(Y���ŏ��l)
//	float							unitWidth;															//�ŏ����x����Ԃ̕��P��
//	float							unitHeight;															//�ŏ����x����Ԃ̍����P��
//	DWORD							cellNum;															//��Ԃ̐�
//	unsigned int					level;																//�ŉ��ʃ��x��
//	CollisionList<T>				collisionList;														//�Փ˃��X�g
//
//public:
//	Linear4TreeManager()																				//�R���X�g���N�^
//	virtual ~Linear4TreeManager()																		//�f�X�g���N�^
//
//	bool initialize(unsigned int level, float left, float top, float right, float bottom)				//���`�S���ؔz����\�z����
//	bool registerObject(float left, float top, float right, float bottom, ObjectTree<T>* objectTree)	//�I�u�W�F�N�g��o�^����
//	DWORD getAllCollisionList(CollisionList<T>** collisionList)											//�Փ˔��胊�X�g���쐬����
//
//#if _DEBUG
//	void render()																						//�������C���`��
//#endif
//
//protected:
//	bool getCollisionList(DWORD element, list<T*> &collisionStac)										//��ԓ��ŏՓ˃��X�g���쐬����
//	DWORD getMortonNumber(float left, float top, float right, float bottom)								//���W�����Ԕԍ����Z�o
//	DWORD bitSeparate32(DWORD n)																		//�r�b�g�����֐�
//	WORD get2DMortonNumber(WORD x, WORD y)																//2D���[�g����Ԕԍ��Z�o�֐�
//	DWORD getPointElement(float x, float y)																//���W�����`4���ؗv�f�ԍ��ϊ��֐�
//};
//===================================================================================================================================
#pragma region Linear4TreeManager
template <class T>
class Linear4TreeManager :public Base
{
protected:
	Cell<T>**						cellArray;							//���`��ԃ|�C���^�z��
	unsigned int					pow[linear4TreeNS::MAX_LEVEL + 1];	//�ׂ��搔�l�z��
	float							width;								//�̈��X����
	float							height;								//�̈��Y����
	float							left;								//�̈�̍���(X���ŏ��l)
	float							top;								//�̈�̏㑤(Y���ŏ��l)
	float							unitWidth;							//�ŏ����x����Ԃ̕��P��
	float							unitHeight;							//�ŏ����x����Ԃ̍����P��
	DWORD							cellNum;							//��Ԃ̐�
	unsigned int					level;								//�ŉ��ʃ��x��
	CollisionList<T>				collisionList;						//�Փ˃��X�g

#ifdef _DEBUG
	Ray*							line;								//�`��p���C��
	float*							length;
	int								splitSum;							//���C���������v
#endif // _DEBUG

public:
	//�R���X�g���N�^
	Linear4TreeManager()
	{
		cellArray		= NULL;
		level			= 0;
		width			= 0.0f;
		height			= 0.0f;
		left			= 0.0f;
		top				= 0.0f;
		unitWidth		= 0.0f;
		unitHeight		= 0.0f;
		cellNum			= 0;

#ifdef _DEBUG
		line			= NULL;
		splitSum		= 0;
		length			= 0;
#endif
		//�e���x���ł̋�Ԑ����Z�o
		pow[0] = 1;
		for (int i = 1; i < linear4TreeNS::MAX_LEVEL + 1; i++)
		{
			pow[i] = pow[i - 1] * 4;
		}
	}

	//�f�X�g���N�^
	virtual ~Linear4TreeManager()
	{
		for (DWORD i = 0; i < cellNum; i++)
		{
			if(cellArray[i] != NULL)
				delete cellArray[i];
		}
		SAFE_DELETE_ARRAY(cellArray);
#ifdef _DEBUG
		SAFE_DELETE_ARRAY(line);
		SAFE_DELETE_ARRAY(length);
#endif // _DEBUG

	}

	//���`�S���ؔz����\�z����
	bool initialize(unsigned int level, float left, float top, float right, float bottom)
	{
		//�ݒ�ō����x���ȏ�̋�Ԃ͍��Ȃ�
		if (level >= linear4TreeNS::MAX_LEVEL)return false;

		//level���x��(0��_)�̔z��쐬
		cellNum		= (pow[level + 1] - 1) / 3;
		cellArray	= new Cell<T>*[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>*)*cellNum);

		//�̈��o�^
		this->left			= left;
		this->top			= top;
		this->width			= right - left;
		this->height		= bottom - top;
		this->unitWidth		= width / (1 << level);
		this->unitHeight	= height / (1 << level);
		this->level			= level;

#ifdef _DEBUG
		//DEBUG�p������ԕ`����̐ݒ�
		int splitNum	= (int)((1 << level) + 1);
		splitSum		= 0;
		splitSum += splitNum;//x
		splitSum += splitNum;//z
		line = new Ray[splitSum];
		D3DXVECTOR3 direction;
		length = new float[splitSum];

		//x���n�_���C��
		for (int i = 0; i < splitNum; i++)
		{
			length[i] = Base::between2VectorDirection(
				&direction, 
				D3DXVECTOR3(left + (unitWidth*i), 0, -height / 2), 
				D3DXVECTOR3(left + (unitWidth*i), 0, height / 2));
			line[i].initialize(D3DXVECTOR3(left + (unitWidth*i), 0, -height / 2), direction);
			line[i].color = D3DXCOLOR(255, 0, 0, 255);
		}

		//z���n�_���C��
		for (int i = 0; i < splitNum; i++)
		{
			length[splitNum + i] = Base::between2VectorDirection(
				&direction,
				D3DXVECTOR3(-width / 2, 0, top + (unitHeight*i)) ,
				D3DXVECTOR3(width / 2, 0, top + (unitHeight*i)) );
			line[splitNum + i].initialize(D3DXVECTOR3(-width / 2, 0, top + (unitHeight*i)), direction);
			line[splitNum + i].color = D3DXCOLOR(0, 0, 255, 255);
		}
#endif // _DEBUG

		return true;

	}

	//�I�u�W�F�N�g��o�^����
	bool registerObject(float left, float top, float right, float bottom, ObjectTree<T>* objectTree)
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
	DWORD getAllCollisionList(CollisionList<T>** collisionList)
	{
		//���X�g�i�z��j�͕K������������
		this->collisionList.resetPos();

		//���[�g��Ԃ̑��݂��`�F�b�N
		if (cellArray[0] == NULL)	return 0;	//��Ԃ����݂��Ă��Ȃ�

		//���[�g��Ԃ�����
		list<T*> collisionStac;
		getCollisionList(0, collisionStac);

		*collisionList = &this->collisionList;

		return (DWORD)this->collisionList.getSize();

	}
#if _DEBUG
	//��ԕ������C���`��
	void render()
	{
		for(int i = 0; i < splitSum; i++)
		{
			line[i].render(length[i]);
		}
	}
#endif


protected:
	//��ԓ��ŏՓ˃��X�g���쐬����
	bool getCollisionList(DWORD element, list<T*> &collisionStac)
	{
		typename list<T*>::iterator it;
		//�@��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
		ObjectTree<T>* objectTree1 = cellArray[element]->getFirstObject();
		while (objectTree1 != 0)
		{
			ObjectTree<T>* objectTree2 = objectTree1->next;
			while (objectTree2 != 0)
			{
				//�Փ˃��X�g�쐬
				collisionList.write(objectTree1->object, objectTree2->object);
				objectTree2 = objectTree2->next;
			}
			//�A�Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
			for (it = collisionStac.begin(); it != collisionStac.end(); it++)
			{
				collisionList.write(objectTree1->object, *it);
			}
			objectTree1 = objectTree1->next;
		}

		bool childFlag = false;
		//�B�q��ԂɈړ�
		DWORD objectNum = 0;
		DWORD i, nextElement;
		for (i = 0; i < 4; i++)
		{
			nextElement = element * 4 + 1 + i;
			if (nextElement < cellNum && cellArray[element * 4 + 1 + i])
			{
				if (!childFlag)
				{
					//�C�o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
					objectTree1 = cellArray[element]->getFirstObject();
					while (objectTree1 != 0)
					{
						collisionStac.push_back(objectTree1->object);
						objectNum++;
						objectTree1 = objectTree1->next;
					}
				}
				childFlag = true;
				getCollisionList(element * 4 + 1 + i, collisionStac);	//�q��Ԃ�
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

		//��Ԕԍ��̔r���I�_���a����
		//�������x�����Z�o
		DWORD difference = rightBottom ^ leftTop;
		unsigned int hiLevel = 0;
		unsigned int i;
		for (i = 0; i < level; i++)
		{
			DWORD check = (difference >> (i * 2)) & 0x3;
			if (check != 0)
			{
				hiLevel = i + 1;
			}
		}
		DWORD spaceNum = rightBottom >> (hiLevel * 2);
		DWORD addNum = (pow[level - hiLevel] - 1) / 3;
		spaceNum += addNum;

		if (spaceNum > cellNum)
			return 0xffffffff;

		return spaceNum;
		
	}

	//�r�b�g�����֐�
	DWORD bitSeparate32(DWORD n)
	{
		n = (n | (n << 8)) & 0x00ff00ff;		//0000 0000 1111 1111 0000 0000 1111 1111
		n = (n | (n << 4)) & 0x0f0f0f0f;		//0000 1111 0000 1111 0000 1111 0000 1111
		n = (n | (n << 2)) & 0x33333333;		//0011 0011 0011 0011 0011 0011 0011 0011
		return (n | (n << 1)) & 0x55555555;		//0101 0101 0101 0101 0101 0101 0101 0101
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
#pragma endregion

//===================================================================================================================================
//�y���`�W���ؖ��O��ԁz
//===================================================================================================================================
namespace linear8TreeNS
{
	//�Z�o���F(2<<Level+1)^3/7
	//���x��0��Ԑ��F1
	//���x��1��Ԑ��F9
	//���x��2��Ԑ��F73
	//���x��3��Ԑ��F585
	//���x��4��Ԑ��F4,681
	//���x��5��Ԑ��F37,449
	//���x��6��Ԑ��F299,593
	//���x��7��Ԑ��F2,396,745
	//���x��8��Ԑ��F19,173,961
	//���x��9��Ԑ��F153,391,689
	const int MAX_LEVEL = 9;	//�ő啪�����x��
}

//===================================================================================================================================
//�y���`�W���؊Ǘ��N���X�z
//class Linear8TreeManager :public Base
//{
//protected:
//	Cell<T>**						cellArray;											//���`��ԃ|�C���^�z��
//	unsigned int					pow[linear8TreeNS::MAX_LEVEL + 1];					//�ׂ��搔�l�z��
//	D3DXVECTOR3						width;												//�̈�̕�
//	D3DXVECTOR3						regionMin;											//�̈�̍ŏ��l
//	D3DXVECTOR3						regionMax;											//�̈�̍ő�l
//	D3DXVECTOR3						unit;												//�ŏ��̈�̕ӂ̒���
//	DWORD							cellNum;											//��Ԃ̐�
//	unsigned int					level;												//�ŉ��ʃ��x��
//	CollisionList<T>				collisionList;										//�Փ˃��X�g
//
//public:
//	Linear8TreeManager()																//�R���X�g���N�^
//	virtual ~Linear8TreeManager()														//�f�X�g���N�^
//
//	bool initialize(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 max)				//�y�������z���`�W���ؔz����\�z����
//	bool registerObject(D3DXVECTOR3* min, D3DXVECTOR3* max, ObjectTree<T>* objectTree)	//�I�u�W�F�N�g��o�^����
//	DWORD getAllCollisionList(CollisionList<T>** collisionList)							//�Փ˔��胊�X�g���쐬����
//
//#if _DEBUG
//	void render()																		//�������C���`��
//#endif
//
//protected:
//	bool getCollisionList(DWORD element, list<T*> &collisionStac)						//��ԓ��ŏՓ˃��X�g���쐬����
//	bool createNewCell(DWORD element)													//��Ԃ𐶐�
//	DWORD getMortonNumber(D3DXVECTOR3* min, D3DXVECTOR3* max)							//���W�����Ԕԍ����Z�o
//	DWORD bitSeparateFor3D(DWORD n)														//�r�b�g�����֐�
//	DWORD get3DMortonNumber(BYTE x, BYTE y, BYTE z)										//3D���[�g����Ԕԍ��Z�o�֐�
//	DWORD getPointElement(D3DXVECTOR3 &point)											//���W�����`8���ؗv�f�ԍ��ϊ��֐�
//}
//===================================================================================================================================
#pragma region Linear8TreeManager
template <class T>
class Linear8TreeManager :public Base
{
protected:
	Cell<T>**						cellArray;							//���`��ԃ|�C���^�z��
	unsigned int					pow[linear8TreeNS::MAX_LEVEL + 1];	//�ׂ��搔�l�z��
	D3DXVECTOR3						width;								//�̈�̕�
	D3DXVECTOR3						regionMin;							//�̈�̍ŏ��l
	D3DXVECTOR3						regionMax;							//�̈�̍ő�l
	D3DXVECTOR3						unit;								//�ŏ��̈�̕ӂ̒���
	DWORD							cellNum;							//��Ԃ̐�
	unsigned int					level;								//�ŉ��ʃ��x��
	CollisionList<T>				collisionList;						//�Փ˃��X�g

#ifdef _DEBUG
	Ray*							xLine;								//�`��p���C��
	Ray*							yLine;								//�`��p���C��
	Ray*							zLine;								//�`��p���C��
	float							xLength;								//�`�撷��
	float							yLength;								//�`�撷��
	float							zLength;								//�`�撷��
	int								splitSum;							//�������C�����v
#endif // _DEBUG

public:
	//�R���X�g���N�^
	Linear8TreeManager()
	{
		cellArray	= NULL;
		width		= D3DXVECTOR3(1, 1, 1);
		regionMin	= D3DXVECTOR3(0, 0, 0);
		regionMax	= D3DXVECTOR3(1, 1, 1);
		unit		= D3DXVECTOR3(1, 1, 1);
		cellNum		= 0;
		level		= 0;

#ifdef _DEBUG
		xLine = NULL;
		yLine = NULL;
		zLine = NULL;
		splitSum = 0;
		xLength = 0;
		yLength = 0;
		zLength = 0;
#endif
		//�e���x���ł̋�Ԑ����Z�o
		pow[0] = 1;
		for (int i = 1; i < linear8TreeNS::MAX_LEVEL + 1; i++)
		{
			pow[i] = pow[i - 1] * 8;
		}
	}

	//�f�X�g���N�^
	virtual ~Linear8TreeManager()
	{
		for (DWORD i = 0; i < cellNum; i++)
		{
			if (cellArray[i] != NULL)
				delete cellArray[i];
		}
		SAFE_DELETE_ARRAY(cellArray);
#ifdef _DEBUG
		SAFE_DELETE_ARRAY(xLine);
		SAFE_DELETE_ARRAY(yLine);
		SAFE_DELETE_ARRAY(zLine);
#endif // _DEBUG
	}

	//�y�������z���`�W���ؔz����\�z����
	bool initialize(unsigned int level, D3DXVECTOR3 min, D3DXVECTOR3 max)
	{
		//�ݒ�ō����x���ȏ�̋�Ԃ͍��Ȃ�
		if (level >= linear8TreeNS::MAX_LEVEL)return false;

		//level���x��(0��_)�̔z��쐬
		cellNum = (pow[level + 1] - 1) / 7;
		cellArray = new Cell<T>*[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>*)*cellNum);

		//�̈��o�^
		this->regionMin = min;
		this->regionMax = max;
		this->width		= regionMax - regionMin;
		this->unit		= width/((float)(1<<level));
		this->level		= level;


#ifdef _DEBUG
		//DEBUG�p������ԕ`����̐ݒ�
		int splitNum = (int)((1 << level) + 1);
		splitSum = splitNum * splitNum;
		xLine = new Ray[splitSum];
		yLine = new Ray[splitSum];
		zLine = new Ray[splitSum];
		D3DXVECTOR3 direction;
		xLength = max.x - min.x;
		yLength = max.y - min.y;
		zLength = max.z - min.z;

		//X���n�_���C��
		direction = D3DXVECTOR3(1, 0, 0);
		for (int j = 0; j < splitNum; j++)
		{
			for (int i = 0; i < splitNum; i++)
			{
				xLine[i+ (j*splitNum)].initialize(D3DXVECTOR3(min.x, min.y + (unit.y*j), min.z+(unit.z*i)), direction);
				xLine[i+ (j*splitNum)].color = D3DXCOLOR(255, 0, 0, 255);
			}
		}
		//Y���n�_���C��
		direction = D3DXVECTOR3(0, 1, 0);
		for (int j = 0; j < splitNum; j++)
		{
			for (int i = 0; i < splitNum; i++)
			{
				yLine[i + (j*splitNum)].initialize(D3DXVECTOR3(min.x + (unit.x*j), min.y, min.z+(unit.z*i)), direction);
				yLine[i + (j*splitNum)].color = D3DXCOLOR(0, 255, 0, 255);
			}
		}
		//Z���n�_���C��
		direction = D3DXVECTOR3(0, 0, 1);
		for (int j = 0; j < splitNum; j++)
		{
			for (int i = 0; i < splitNum; i++)
			{
				zLine[i+ (j*splitNum)].initialize(D3DXVECTOR3(min.x + (unit.x*j), min.y + (unit.y*i), min.z), direction);
				zLine[i+ (j*splitNum)].color = D3DXCOLOR(0, 0, 255, 255);
			}
		}
#endif // _DEBUG

		return true;
	}

	//�I�u�W�F�N�g��o�^����
	bool registerObject(D3DXVECTOR3* min,D3DXVECTOR3* max,ObjectTree<T>* objectTree)
	{
		//�I�u�W�F�N�g�̋��E�͈͂���o�^���[�g���ԍ����Z�o
		DWORD element = getMortonNumber(min,max);
		if (element < cellNum)
		{
			//��Ԃ������ꍇ�͐V�K�쐬
			if (!cellArray[element])
			{
				createNewCell(element);
			}
			return cellArray[element]->push(objectTree);
		}
		return false;//�o�^���s
	}

	//�Փ˔��胊�X�g���쐬����
	DWORD getAllCollisionList(CollisionList<T>** collisionList)
	{
		//���X�g�i�z��j�͕K������������
		this->collisionList.resetPos();

		//���[�g��Ԃ̑��݂��`�F�b�N
		if (cellArray[0] == NULL)	return 0;	//��Ԃ����݂��Ă��Ȃ�

		//���[�g��Ԃ�����
		list<T*> collisionStac;
		getCollisionList(0, collisionStac);

		*collisionList = &this->collisionList;

		return (DWORD)this->collisionList.getSize();
	}
#if _DEBUG
	//�������C���`��
	void render()
	{
		for (int i = 0; i < splitSum; i++)
		{
			xLine[i].render(xLength);
		}
		for (int i = 0; i < splitSum; i++)
		{
			yLine[i].render(yLength);
		}
		for (int i = 0; i < splitSum; i++)
		{
			zLine[i].render(zLength);
		}
	}
#endif

protected:
	//��ԓ��ŏՓ˃��X�g���쐬����
	bool getCollisionList(DWORD element, list<T*> &collisionStac)
	{
		typename list<T*>::iterator it;
		//�@��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
		ObjectTree<T>* objectTree1 = cellArray[element]->getFirstObject();
		while (objectTree1 != 0)
		{
			ObjectTree<T>* objectTree2 = objectTree1->next;
			while (objectTree2 != 0)
			{
				//�Փ˃��X�g�쐬
				if (objectTree1->type & objectTree2->target ||
					objectTree2->type & objectTree1->target)
				{
					collisionList.write(objectTree1->object, objectTree2->object);
				}
				objectTree2 = objectTree2->next;
			}
			//�A�Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
			for (it = collisionStac.begin(); it != collisionStac.end(); it++)
			{
				if (objectTree1->type & (*it)->treeCell.target 
					||	(*it)->treeCell.type & objectTree1->target)
				{
					collisionList.write(objectTree1->object, *it);
				}
			}
			objectTree1 = objectTree1->next;
		}

		bool childFlag = false;
		//�B�q��ԂɈړ�
		DWORD objectNum = 0;
		DWORD i, nextElement;
		for (i = 0; i < 8; i++)
		{
			nextElement = element * 8 + 1 + i;
			if (nextElement < cellNum && cellArray[element * 8 + 1 + i])
			{
				if (!childFlag)
				{
					//�C�o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
					objectTree1 = cellArray[element]->getFirstObject();
					while (objectTree1 != 0)
					{
						collisionStac.push_back(objectTree1->object);
						objectNum++;
						objectTree1 = objectTree1->next;
					}
				}
				childFlag = true;
				getCollisionList(element * 8 + 1 + i, collisionStac);	//�q��Ԃ�
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
			element = (element - 1) >> 3;
			if (element >= cellNum)break;

		}
		return true;
	}

	//���W�����Ԕԍ����Z�o
	DWORD getMortonNumber(D3DXVECTOR3* min,D3DXVECTOR3* max)
	{
		//�ŏ����x���ɂ�����e���ʒu�Z�o
		DWORD leftTop		= getPointElement(*min);
		DWORD rightBottom	= getPointElement(*max);

		//��Ԕԍ��̔r���I�_���a����
		//�������x�����Z�o
		DWORD difference = rightBottom ^ leftTop;
		unsigned int hiLevel = 0;
		unsigned int i;
		for (i = 0; i < level; i++)
		{
			DWORD check = (difference >> (i * 3)) & 0x7;
			if (check != 0)
			{
				hiLevel = i + 1;
			}
		}
		DWORD spaceNum = rightBottom >> (hiLevel * 3);
		DWORD addNum = (pow[level - hiLevel] - 1) / 7;
		spaceNum += addNum;

		if (spaceNum > cellNum)
			return 0xffffffff;

		return spaceNum;

	}

	//�r�b�g�����֐�
	DWORD bitSeparateFor3D(DWORD n)
	{
		DWORD s = n;
		s = (s | (s << 8)) & 0x0000f00f;		//0000 0000 0000 0000 1111 0000 0000 1111
		s = (s | (s << 4)) & 0x000c30c3;		//0000 0000 0000 1100 0011 0000 1100 0011
		s = (s | (s << 2)) & 0x00249249;		//0000 0000 0010 0100 1001 0010 0100 1001
		return s;
	}

	//3D���[�g����Ԕԍ��Z�o�֐�
	DWORD get3DMortonNumber(BYTE x, BYTE y,BYTE z)
	{
		return bitSeparateFor3D(x) | bitSeparateFor3D(y) << 1 | bitSeparateFor3D(z) << 2;
	}

	//���W�����`8���ؗv�f�ԍ��ϊ��֐�
	DWORD getPointElement(D3DXVECTOR3 &point)
	{
		return get3DMortonNumber(
			(BYTE)((point.x-regionMin.x)/unit.x),
			(BYTE)((point.y-regionMin.y)/unit.y),
			(BYTE)((point.z-regionMin.z)/unit.z));
	}
};
#pragma endregion

//===================================================================================================================================
//�y��ԃN���X�z
//template<class T>
//class Cell
//{
//protected:
//	ObjectTree<T>* latest;	//	�ŐVObjectTree�ւ̃|�C���^
//
//public:
//	Cell();										//�R���X�g���N�^
//	virtual ~Cell();							//�f�X�g���N�^
//
//	void resetLink(ObjectTree<T>* &objectTree);	//�����N��S�ă��Z�b�g����
//
//	bool push(ObjectTree<T>* &objectTree);		//ObjectTree���v�b�V��
//
//	ObjectTree<T>* getFirstObject();			//��Ԃ̐擪�ɓo�^���ꂽ�I�u�W�F�N�g�c���[���擾
//
//	bool onRemove(ObjectTree<T>* removeObject);	//�폜�����I�u�W�F�N�g�`�F�b�N
//}
//===================================================================================================================================
#pragma region Cell
template<class T>
class Cell
{
protected:
	ObjectTree<T>* latest;	//	�ŐVObjectTree�ւ̃|�C���^

public:
	//�R���X�g���N�^
	Cell():latest(0){}

	//�f�X�g���N�^
	virtual ~Cell()
	{
		if (latest != NULL)
			resetLink(latest);
	}

	//�����N��S�ă��Z�b�g����
	void resetLink(ObjectTree<T>* objectTree)
	{
		if (objectTree->next != 0)
		{
			resetLink(objectTree->next);
		}
		objectTree = 0;
	}

	//ObjectTree���v�b�V��
	bool push(ObjectTree<T>* &objectTree)
	{
		if (objectTree == 0)			return false;	//�����I�u�W�F�N�g�͓o�^���Ȃ�
		if (objectTree->cell == this)	return false;	//�Q�d�o�^�`�F�b�N
		if (latest == 0)
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

	//��Ԃ̐擪�ɓo�^���ꂽ�I�u�W�F�N�g���擾����
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
#pragma endregion
