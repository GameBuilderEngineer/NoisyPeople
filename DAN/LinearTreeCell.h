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
		cellArray	= new Cell<T>*()[cellNum];
		ZeroMemory(cellArray, sizeof(Cell<T>)*cellNum);

		//�̈��o�^


	}

};

