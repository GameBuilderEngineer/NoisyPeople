#include "GreeningArea.h"



GreeningArea::GreeningArea()
{
	{//�I�u�W�F�N�g�^�C�v�ƏՓˑΏۂ̎w��
		using namespace ObjectType;
		treeCell.type = GREENING_AREA;
		treeCell.target = TREE;
	}


}


GreeningArea::~GreeningArea()
{
}

void GreeningArea::update(float frameTime)
{

}

void GreeningArea::render()
{
	sphere->render();
}
