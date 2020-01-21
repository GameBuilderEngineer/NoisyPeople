//============================
//[TutorialTex.cpp]
//����W��
//============================

//============================
//�C���N���[�h
//============================
#include "TutorialTex.h"
#include "Input.h"

//============================
//�yusing�錾�z
//============================
using namespace tutorialTex;

//============================
//�O���[�o���ϐ�
//============================

//============================
//������
//============================
void TutorialTex::initialize()
{
	tutorial1 = new Sprite;
	tutorial2 = new Sprite;
	tutorial3_1 = new Sprite;
	tutorial3_2 = new Sprite;
	tutorialStandby1 = new Sprite;
	tutorialStandby2 = new Sprite;

	nextPage = TUTORIAL_2D_SCENE01;//�ŏ��̃`���[�g���A���y�[�W�ŏ�����

	ZeroMemory(renderFlag, sizeof(bool) * 4);

	//1�y�[�W��
	tutorial1->initialize(
		*textureNS::reference(textureNS::TUTORIAL1)//�e�N�X�`��
		, SpriteNS::CENTER						//���S
		, WIDTH									//����
		, HEIGHT									//�c��
		, POSITION								//�\���ʒu
		, ROTATION								//��]
		, COLOR);								//�F

	//2�y�[�W��
	tutorial2->initialize(
		*textureNS::reference(textureNS::TUTORIAL2)//�e�N�X�`��
		, SpriteNS::CENTER						//���S
		, WIDTH									//����
		, HEIGHT								//�c��
		, POSITION								//�\���ʒu
		, ROTATION								//��]
		, COLOR);								//�F

	//3�y�[�W��-1
	tutorial3_1->initialize(
		*textureNS::reference(textureNS::TUTORIAL3)//�e�N�X�`��
		, SpriteNS::CENTER						//���S
		, WIDTH	/ 2								//����
		, HEIGHT								//�c��
		, POSITION_1P								//�\���ʒu
		, ROTATION								//��]
		, COLOR);								//�F
	
	//3�y�[�W��-2
	tutorial3_2->initialize(
		*textureNS::reference(textureNS::TUTORIAL3)//�e�N�X�`��
		, SpriteNS::CENTER						//���S
		, WIDTH / 2								//����
		, HEIGHT								//�c��
		, POSITION_2P							//�\���ʒu
		, ROTATION								//��]
		, COLOR);								//�F

	//�X�^���o�C1
	tutorialStandby1->initialize(
		*textureNS::reference(textureNS::TUTORIAL_STANDBY_1)//�e�N�X�`��
		, SpriteNS::CENTER						//���S
		, WIDTH	/ 2								//����
		, HEIGHT								//�c��
		, POSITION_1P							//�\���ʒu
		, ROTATION								//��]
		, COLOR);								//�F

	//�X�^���o�C2
	tutorialStandby2->initialize(
		*textureNS::reference(textureNS::TUTORIAL_STANDBY_2)//�e�N�X�`��
		, SpriteNS::CENTER						//���S
		, WIDTH	/ 2								//����
		, HEIGHT								//�c��
		, POSITION_2P							//�\���ʒu
		, ROTATION								//��]
		, COLOR);								//�F
}


//============================
//�`��
//nextPage���Q�Ƃ��`��
//============================
void TutorialTex::render()
{
	switch (nextPage)
	{
	case TUTORIAL_2D_SCENE01:
		tutorial1->render();
		break;

	case TUTORIAL_2D_SCENE02:
		tutorial2->render();
		break;
	}
	// ���ʃe�N�X�`��
	if (renderFlag[0]) { tutorial3_1->render(); }
	if (renderFlag[1]) { tutorial3_2->render(); }
	if (renderFlag[2]) { tutorialStandby1->render(); }
	if (renderFlag[3]) { tutorialStandby2->render(); }
}

//============================
//�X�V
//============================
void TutorialTex::update()
{
	
}

//============================
//�I������
//============================
void TutorialTex::uninitialize()
{
	delete  tutorial1;
	delete  tutorial2;
}

//============================
//�^��ʃe�N�X�`���̎擾
//============================
Sprite* TutorialTex::getHalfSlide(int playerNo, int number)
{
	switch(playerNo)
	{
	case 0:
		if (number == 0) { return tutorial3_1; }
		if (number == 1) { return tutorialStandby1; }
		break;

	case 1:
		if (number == 0) { return tutorial3_2; }
		if (number == 1) { return tutorialStandby2; }
		break;
	}
}

void TutorialTex::setRender(int index, bool setting)
{
	renderFlag[index] = setting;
}