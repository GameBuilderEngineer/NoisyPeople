//==========================================
//�Ή��{����������
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "WholeGreening.h"

//==========================================
// using�錾
//==========================================
using namespace wholeGreenig_CharaNS;
using namespace basicUiNS;

//==========================================
//�R���X�g���N�^
//==========================================
WholeGreening::WholeGreening()
{
	greening = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
WholeGreening::~WholeGreening()
{
	delete greening;
}

//==========================================
//������
//==========================================
void WholeGreening::initialize()
{
		BasicUI::assingSize(WIDTH_GREENIG, HEIGHT_GREENIG);
		BasicUI::assingPosition(POSITION_GREENIG);
		BasicUI::initialize(greening, textureNS::reference(textureNS::RESULT_RYOKUKA));
}

//==========================================
//�`��
//==========================================
void WholeGreening::render()
{
	greening->render();
}

//==========================================
//�X�V
//==========================================
void WholeGreening::update()
{
	
}