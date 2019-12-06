//=============================================
//[UItex.cpp]
//����W��
//=============================================

//=============================================
//�C���N���[�h
//=============================================
#include "UIrank.h"
#include "Sound.h"

//=============================================
//�yusing�錾�z
//=============================================
using namespace uiRankNS;

//=============================================
//�R���X�g���N�^
//=============================================
UIrank::UIrank()
{
	for (int i = 0; i < RANK_MAX; i++)
	{
		rank[basicUiNS::P1][i] = new Sprite;
		rank[basicUiNS::P2][i] = new Sprite;
	}
}

//=============================================
//�f�X�g���N�^
//=============================================
UIrank::~UIrank()
{
	for (int i = 0; i < RANK_MAX; i++)
	{
		delete rank[basicUiNS::P1][i];
		delete rank[basicUiNS::P2][i];
	}
}

//============================
//������
//������:�����N�̎��
//������:1P��2P��(0�Ȃ�P1����ȊO�Ȃ�P2)
//============================
void UIrank::initialize()
{

	//SE�Đ��t���O�̏�����
	playedSE = true;

	//�T�C�Y����
	widthSize = WIDTH_RANK;
	heightSize = HEIGHT_RANK;

	//�����N�̎�ނƃv���C���[�ɉ����ď�����
	rank[basicUiNS::P1][EXCELLENT]->initialize(
				*textureNS::reference(textureNS::RESULT_EXCELLENT)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P1							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F

		
	rank[basicUiNS::P2][EXCELLENT]->initialize(
				*textureNS::reference(textureNS::RESULT_EXCELLENT)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P2							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F
		
	rank[basicUiNS::P1][GREAT]->initialize(
				*textureNS::reference(textureNS::RESULT_GREAT)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P1							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F
		
	rank[basicUiNS::P2][GREAT]->initialize(
				*textureNS::reference(textureNS::RESULT_GREAT)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P2							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F
		
	rank[basicUiNS::P1][CLEARE]->initialize(
				*textureNS::reference(textureNS::RESULT_CLEARE)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P1							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F
		
	rank[basicUiNS::P2][CLEARE]->initialize(
				*textureNS::reference(textureNS::RESULT_CLEARE)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P2							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F
		
	rank[basicUiNS::P1][FAILED]->initialize(
				*textureNS::reference(textureNS::RESULT_FAILED)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P1							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F
		
	rank[basicUiNS::P2][FAILED]->initialize(
				*textureNS::reference(textureNS::RESULT_FAILED)//�e�N�X�`��
				, SpriteNS::CENTER							//���S
				, WIDTH_RANK								//����
				, HEIGHT_RANK								//�c��
				, POSITION_RANK_P2							//�\���ʒu
				, ROTATION_TEX								//��]
				, COLOR_TEX);								//�F
		
}

//============================
//�`��
//������:�����N�̎��
//============================
void UIrank::render(int rankType1,int rankType2)
{
	rank[basicUiNS::P1][rankType1]->render();
	rank[basicUiNS::P2][rankType2]->render();
}

//============================
//�X�V
//������:�����N�̎��
//============================
void UIrank::update(int rankType1, int rankType2)
{
	if (widthSize > END_WIDTH_RANK && heightSize > END_HEIGHT_RANK)
	{
		widthSize -= SHRINK_SPEED;
		heightSize -= SHRINK_SPEED;
		rank[basicUiNS::P1][rankType1]->setSize(widthSize, heightSize);
		rank[basicUiNS::P1][rankType1]->setVertex();
		rank[basicUiNS::P1][rankType1]->render();
		rank[basicUiNS::P2][rankType2]->setSize(widthSize, heightSize);
		rank[basicUiNS::P2][rankType2]->setVertex();
		rank[basicUiNS::P2][rankType2]->render();
	}
	if (widthSize == END_WIDTH_RANK && heightSize == END_HEIGHT_RANK&&playedSE==true)
	{
		PLAY_PARAMETERS playParameters = { 0 };
		playParameters = { ENDPOINT_VOICE_LIST::ENDPOINT_SE, SE_LIST::SE_Score, false,NULL,false,NULL};
		SoundInterface::SE->playSound(&playParameters);
		playedSE = false;
	}
}
