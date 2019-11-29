//============================
//[CrediTex.cpp]
//����W��
//============================

//============================
//�C���N���[�h
//============================
#include "CreditTex.h"

//============================
//�yusing�錾�z
//============================
using namespace creditTex;

//============================
//�O���[�o���ϐ�
//============================

//============================
//������
//============================
void CreditTex::initialize()
{
	for (int i = 0; i < CREDIT_MAX; i++)
	{
		credit[i] = new Sprite;
	}
	
	credit[0]->initialize(
						*textureNS::reference(textureNS::CREDIT)//�e�N�X�`��
						,SpriteNS::CENTER						//���S
						,WIDTH									//����
						,HEIGHT									//�c��
						,POSITION								//�\���ʒu
						,ROTATION								//��]
						,COLOR);								//�F

	credit[1]->initialize(
						*textureNS::reference(textureNS::CREDIT2)//�e�N�X�`��
						, SpriteNS::CENTER						//���S
						, WIDTH									//����
						, HEIGHT								//�c��
						, POSITION_2							//�\���ʒu
						, ROTATION								//��]
						, COLOR);								//�F
						
	pos[0] = POSITION;
	pos[1] = POSITION_2;
	secneState = 4;
	alpha = 255.0f;
}

//============================
//�`��
//============================
void CreditTex::render()
{
	for (int i = 0; i < CREDIT_MAX; i++)
	{
		credit[i]->render();
	}
}

//============================
//�X�V
//============================
void  CreditTex::update()
{
	if (pos[1].y >= MOVE_MAX)
	{
		moveCredit();
	}

	if (pos[1].y <= -540)
	{
		fadeOut();
	}
}

//============================
//�I������
//============================
void CreditTex::uninitialize()
{
	for (int i = 0; i < CREDIT_MAX; i++)
	{
		delete credit[i];
	}
}

//===========================
// ��ɓ���������
//===========================
void CreditTex::moveCredit()
{
	pos[0].y -= MOVE_SPEED;
	pos[1].y -= MOVE_SPEED;

	credit[0]->setPosition(pos[0]);
	credit[1]->setPosition(pos[1]);
	credit[0]->setVertex();
	credit[1]->setVertex();
}

//===========================
//�t�F�[�h�A�E�g����
//===========================
void CreditTex::fadeOut()
{
	alpha -= 0.5f;
	credit[1]->setAlphaAllVertex(alpha);
	if (alpha <= 0)
	{
		secneState = 2;
	}
}

//===========================
//�V�[���J�ڊǗ��p
//===========================
int CreditTex::getSceneState()
{
	return secneState;
}