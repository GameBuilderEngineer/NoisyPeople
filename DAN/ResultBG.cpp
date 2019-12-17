//======================================
//���U���gBG����[ResultBG.cpp]
//����W��
//======================================

//======================================
//�C���N���[�h
//======================================
#include "ResultBG.h"

//======================================
// using�錾
//======================================
using namespace resultBGNS;

//======================================
//�R���X�g���N�^
//======================================
ResultBG::ResultBG()
{
	resultBG = new Sprite;
}

//======================================
//�f�X�g���N�^
//======================================
ResultBG::~ResultBG()
{
	delete resultBG;
}

//======================================
//������
//======================================
void ResultBG::initialize()
{
	BasicUI::assingPosition(BG_POSITION);
	BasicUI::assingSize(WIDTH_BG, HEIGHT_BG);
	BasicUI::assingColor(COLOR);
	BasicUI::initialize(resultBG, textureNS::reference(textureNS::RESULT_BG));
}

//=====================================
//�`��
//=====================================
void ResultBG::render()
{
	resultBG->render();
}

//=====================================
//�X�V
//======================================
void ResultBG::update()
{

}