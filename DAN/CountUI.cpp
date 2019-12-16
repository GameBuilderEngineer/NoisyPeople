//===================================================================================================================================
//�ySplashSprite.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/09/23
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "CountUI.h"
#include "TextureLoader.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace CountUINS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
CountUI::CountUI()
{
	initialize(
		*textureNS::reference(textureNS::UI_COUNT_NUM),	//�e�N�X�`���|�C���^
		SpriteNS::CENTER,									//���_
		WIDTH/DIVIDE_WIDTH,									//��
		HEIGHT,												//����
		POSITION,											//�ʒu
		ROTATION,											//��]
		COLOR												//�F
	);

	setSize(WIDTH / DIVIDE_WIDTH*3, HEIGHT*3);

	//UV�l�̐ݒ�
	setUVCoord(
		D3DXVECTOR2(0.0f, 0.0f)					+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(1.0f/DIVIDE_WIDTH, 0.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(0.0f, 1.0f)					+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f),
		D3DXVECTOR2(1.0f/ DIVIDE_WIDTH, 1.0f)	+ D3DXVECTOR2(1.0f/DIVIDE_WIDTH*0,0.0f)
	);

	displayTimer = 0.0f;
	finished = false;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
CountUI::~CountUI()
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void CountUI::update(float frameTime)
{
	//�\�����Ԃ��Ȃ��ꍇ�͍X�V���Ȃ�
	if (displayTimer <= 0)return;

	float rate = 0.0f;

	//�\�����Ԃ̍X�V
	displayTimer -= frameTime;

	if (finished)
	{
		setAlphaAllVertex(255);
	}
	else {
		//�^�C�}�[�ɉ����āA��Ԃ�ω�������
		rate = max(0.0f,displayTimer/displayTime);		//rate�F1.0f��0.0f�ɑJ�ڂ���
		setAlphaAllVertex((int)((float)255 * rate));			//���X�ɓ����ɂȂ�
	}

	setVertex();
}

//===================================================================================================================================
//�y�X�^�[�g���J�E���g�z
//===================================================================================================================================
void CountUI::startCount(int count)
{
	this->count = count;
	switch (count)
	{
	case 3:	case 2:	case 1:
		//�\�����Ԃ̃Z�b�g
		displayTimer = displayTime = COUNT_DISPLAY_TIME;
		//�e�N�X�`���|�C���^�̃Z�b�g
		setTexturePointer(*textureNS::reference(textureNS::UI_COUNT_NUM));
		//�T�C�Y�̃Z�b�g
		setSize(WIDTH / DIVIDE_WIDTH * 3, HEIGHT * 3);
		//UV�l�̐ݒ�
		setUVCoord(
			D3DXVECTOR2(0.0f, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f));
		break;
	case 0:
		//�\�����Ԃ̃Z�b�g
		displayTimer = displayTime = START_DISPLAY_TIME;
		//�e�N�X�`���|�C���^�̃Z�b�g
		setTexturePointer(*textureNS::reference(textureNS::UI_GAME_START));
		//�T�C�Y�̃Z�b�g
		setSize(1024, 512);
		//UV�l�̐ݒ�
		setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
		break;
	}
}

//===================================================================================================================================
//�y�t�B�j�b�V�����J�E���g�z
//===================================================================================================================================
void CountUI::finishCount(int count)
{
	this->count = count;
	switch (count)
	{
	case 10:case 9:case 8:case 7:case 6:case 5:case 4:case 3:case 2:case 1:
		//�\�����Ԃ̃Z�b�g
		displayTimer = displayTime = COUNT_DISPLAY_TIME;
		setTexturePointer(*textureNS::reference(textureNS::UI_COUNT_NUM));
		setSize(WIDTH / DIVIDE_WIDTH * 3, HEIGHT * 3);
		//UV�l�̐ݒ�
		setUVCoord(
			D3DXVECTOR2(0.0f, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 0.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(0.0f, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f),
			D3DXVECTOR2(1.0f / DIVIDE_WIDTH, 1.0f) + D3DXVECTOR2(1.0f / DIVIDE_WIDTH * count, 0.0f));
		break;
	case 0:
		//�\�����Ԃ̃Z�b�g
		displayTimer = displayTime = FINISH_DISPLAY_TIME;
		setTexturePointer(*textureNS::reference(textureNS::UI_TIME_UP));
		setSize(1700, 400);
		setUVCoord(D3DXVECTOR2(0.0, 0.0), D3DXVECTOR2(1.0, 0.0), D3DXVECTOR2(0.0, 1.0), D3DXVECTOR2(1.0, 1.0));
		finished = true;
		break;
	}

}

