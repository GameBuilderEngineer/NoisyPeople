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
	credit = new Sprite;

	//�J�n�ʒu�̏�����
	pos = POSITION;

	credit->initialize(
						*textureNS::reference(textureNS::CREDIT)//�e�N�X�`��
						,SpriteNS::CENTER						//���S
						,WIDTH									//����
						,HEIGHT									//�c��
						,POSITION								//�\���ʒu
						,ROTATION								//��]
						,COLOR);								//�F

}

//============================
//�`��
//============================
void CreditTex::render()
{
	credit->render();
}

//============================
//�X�V
//============================
void CreditTex::update()
{
	if (pos.y > MOVE_MAX)
	{
		moveCredit();
	}
}

//============================
//�I������
//============================
void CreditTex::uninitialize()
{
	delete credit;
}

//===========================
// ��ɓ���������
//===========================
void CreditTex::moveCredit()
{

	pos.y -= MOVE_SPEED;

	credit->setPosition(pos);
	credit->setVertex();
	

}