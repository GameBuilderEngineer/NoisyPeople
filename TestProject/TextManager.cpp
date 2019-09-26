//============================================================================================================================================
// Document
//============================================================================================================================================
// TextManager.cpp
// HAL���� GP-12A-332 09 �T���v��
// 2019/08/02
//============================================================================================================================================
#include "TextManager.h"
//============================================================================================================================================
// Using Declaration
// using�錾
//============================================================================================================================================
using namespace textManagerNS;
//============================================================================================================================================
// Constructor
// �R���X�g���N�^
//============================================================================================================================================
TextManager::TextManager(void)
{
	// �f�B���N�g���̐ݒ�
	setDataDirectory();

	// ������
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		switch (i)
		{
		case NEW_RODIN_CHARACTER_SELECT_TIME:
		case NEW_RODIN_RESULT_CHINGIN_BACK:
		case NEW_RODIN_RESULT_CHINGIN:
		case NEW_RODIN_GAME_TIME:
			dataName[i] = { "FOT-NewRodinPro-UB.otf" };		//	�t�H���g�f�[�^���\�[�X�̖��O
			fontName[i] = { "FOT-�j���[���_�� Pro UB" };	//	�t�H���g�̖��O
			break;
		case FUTURA:
			dataName[i] = { "unicode.futurabb.ttf" };	//	�t�H���g�f�[�^���\�[�X�̖��O
			fontName[i] = { "Futura-Black" };			//	�t�H���g�̖��O
		default:
			break;
		}
	}

	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		// �t�H���g�f�[�^�̓ǂݍ���
		AddFontResourceEx(dataName[i], FR_PRIVATE, NULL);
		// �C���X�^���X�쐬
		text[i] = new Text();
	}

	return;
}
//============================================================================================================================================
// Destructor
// �f�X�g���N�^
//============================================================================================================================================
TextManager::~TextManager(void)
{
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		// �t�H���g�f�[�^�폜
		RemoveFontResourceEx(dataName[i], FR_PRIVATE, NULL);
		// �C���X�^���X�폜
		SAFE_DELETE(text[i]);
	}

	return;
}
//============================================================================================================================================
// initialize
// ������
//============================================================================================================================================
void TextManager::initialize(LPDIRECT3DDEVICE9 _device)
{
	// ������
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		switch (i)
		{
		case NEW_RODIN_CHARACTER_SELECT_TIME:
			text[i]->initialize(_device, 80, 45, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
			break;
		case NEW_RODIN_RESULT_CHINGIN_BACK:
			text[i]->initialize(_device, 160, 90, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
			break;
		case NEW_RODIN_RESULT_CHINGIN:
			text[i]->initialize(_device, 160, 90, D3DXCOLOR(255, 255, 255, 255), fontName[i]);
			break;
		case NEW_RODIN_GAME_TIME:
			text[i]->initialize(_device, 48, 27, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
			break;
		case FUTURA:
			text[i]->initialize(_device, 48, 27, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
		default:
			break;
		}
	}

	return;
}