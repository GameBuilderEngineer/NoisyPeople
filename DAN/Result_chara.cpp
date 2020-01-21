//==========================================
//���U���g��������
//����W��
//==========================================

//==========================================
//�C���N���[�h
//==========================================
#include "Result_chara.h"

//==========================================
// using�錾
//==========================================
using namespace result_CharaNS;
using namespace basicUiNS;

//==========================================
//�R���X�g���N�^
//==========================================
Result_Chara::Result_Chara()
{
	result = new Sprite;
}

//==========================================
//�f�X�g���N�^
//==========================================
Result_Chara::~Result_Chara()
{
	delete result;
}

//==========================================
//������
//==========================================
void Result_Chara::initialize()
{
	BasicUI::assingSize(WIDTH_RESULT, HEIGHT_RESULT);
	BasicUI::assingPosition(POSITION_RESULT);
	BasicUI::initialize(result, textureNS::reference(textureNS::RESULT));
}

//==========================================
//�`��
//==========================================
void Result_Chara::render()
{
	result->render();
}

//==========================================
//�X�V
//==========================================
void Result_Chara::update(float flametime)
{
	if (BasicUI::position.x > END_POS_RESULT)
	{
		BasicUI::changePostion(result, slideLeft,flametime);
	}
}