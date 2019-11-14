//====================================
//HP�Q�[�W����[HPguage.cpp]
//����W��
//====================================

//====================================
//�C���N���[�h
//====================================
#include "HPguage.h"

//====================================
// using�錾
//====================================
using namespace hpGuageNS;

//====================================
//�R���X�g���N�^
//====================================
HPguage::HPguage()
{
	hpGuage = new Sprite;
	uvCoord1 = D3DXVECTOR2(0.0, 0.0);
	uvCoord2 = D3DXVECTOR2(1.0, 0.0);
	uvCoord3 = D3DXVECTOR2(0.0, 1.0);
	uvCoord4 = D3DXVECTOR2(1.0, 1.0);
	width = WIDTH_HP_GUAGE;
}

//====================================
//�f�X�g���N�^
//====================================
HPguage::~HPguage()
{
	delete hpGuage;
}

//====================================
//������
//====================================
void HPguage::initialize()
{
	BasicUI::assingPosition(POSITION_HP_GUAGE);
	BasicUI::assingSize(WIDTH_HP_GUAGE, HEIGHT_HP_GUAGE);
	BasicUI::assingUV(uvCoord1, uvCoord2, uvCoord3, uvCoord4);
	BasicUI::changePivot();
	BasicUI::initialize(hpGuage, textureNS::reference(textureNS::UI_HP_GUAGE_P1));
}

//====================================
//�`��
//====================================
void HPguage::render()
{
	
	hpGuage->render();
}

//====================================
//�X�V
//====================================
void HPguage::update(int hp)
{
	changeGage(hp);
}

//====================================
//HP�Q�[�W�̐L�яk�ݏ���
//====================================
void HPguage::changeGage(int hp)
{
	//HP���Q�Ƃ��T�C�Y��ύX
	float hpPersent = (float)hp / 100;//���݂�HP���ő�HP�Ŋ����Ċ������o��
	if (BasicUI::widthSize>WIDTH_HP_GUAGE*hpPersent)
	{
		BasicUI::reductionWidthSize(hpGuage, 1.0f);
	}
}