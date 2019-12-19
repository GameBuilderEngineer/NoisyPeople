//===================================
//�d�̓Q�[�W����[ElectGuage.cpp]
//����W��
//===================================

//===================================
//�C���N���[�h
//===================================
#include "ElectGuage.h"
#include "Input.h"

//===================================
// usuing�錾
//===================================
using namespace electGuageNS;

//===================================
//�R���X�g���N�^
//===================================
ElectGuage::ElectGuage()
{
	electGuage = new Sprite;
	width = WIDTH_EN_GUAGE;
}

//===================================
//�f�X�g���N�^
//===================================
ElectGuage::~ElectGuage()
{
	delete electGuage;
}

//===================================
//������
//===================================
void ElectGuage::initialize(int playerType)
{
	switch (playerType)
	{
	case basicUiNS::P1:
		BasicUI::assingColor(COLOR);
		BasicUI::assingSize(WIDTH_EN_GUAGE, HEIGHT_EN_GUAGE);
		BasicUI::assingPosition(POSITION_EN_GUAGE);
		BasicUI::changePivot(SpriteNS::TOP_LEFT);
		BasicUI::initialize(electGuage, textureNS::reference(textureNS::UI_EN_GUAGE_P1));
		break;
	case basicUiNS::P2:
		BasicUI::assingSize(WIDTH_EN_GUAGE, HEIGHT_EN_GUAGE);
		BasicUI::assingPosition(POSITION_EN_GUAGE_P2);
		BasicUI::changePivot(SpriteNS::TOP_RIGHT);
		BasicUI::initialize(electGuage, textureNS::reference(textureNS::UI_EN_GUAGE_P2));
		break;

	}
}

//===================================
//�`��
//===================================
void ElectGuage::render()
{
	
	electGuage->render();
}

//===================================
//�X�V
//�����F�d��
//===================================
void ElectGuage::update(int power)
{
	changeGuage(power);
	if (power < 70)
	{
		BasicUI::changeColor(electGuage, LED_COLOR);
	}
	else
	{
		BasicUI::changeColor(electGuage,COLOR);
	}
}

//===================================
//�Q�[�W�̕ω�����
//�����F���݂̓d��
//===================================
void ElectGuage::changeGuage(int power)
{
	//HP���Q�Ƃ��T�C�Y��ύX
	float enPersent = (float)power / 300;//���݂�EN���ő�HP�Ŋ����Ċ������o��
	/*BasicUI::changeUV(electGuage, D3DXVECTOR2(0, 0), D3DXVECTOR2(enPersent, 0), D3DXVECTOR2(0, 1), D3DXVECTOR2(enPersent, 1));*/
	if (BasicUI::widthSize>WIDTH_EN_GUAGE*enPersent)
	{
		BasicUI::reductionWidthSize(electGuage, 1.5f);
	}
	else if(enPersent <=1.0f &&BasicUI::widthSize<512)
	{
		BasicUI::expansionWidthSize(electGuage, 1.5f);
	}
}