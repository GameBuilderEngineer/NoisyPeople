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

	nextPage = TUTORIAL_2D_SCENE01;//�ŏ��̃`���[�g���A���y�[�W�ŏ�����

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
