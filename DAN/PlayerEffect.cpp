//===================================================================================================================================
//�yPlayerEffect.cpp�z
// [�쐬��] HAL����GP12A332 11 ���� ��
// [�쐬��] 2019/12/23
// [�X�V��] 2019/12/23
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "PlayerEffect.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace PlayerEffectNS;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
PlayerEffect::PlayerEffect(int managerNo)
{
	//�v���C���[�̃|�C���^
	this->player = NULL;
	//�X�J�C�|�W�V�����̃|�C���^
	this->skyPosition = NULL;

	//�G�t�F�N�V�A�[�̃}�l�[�W���[�ԍ�
	this->managerNo = managerNo;

	distanceFlower = 15.0f;
	distanceFeather = 15.0f;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
PlayerEffect::~PlayerEffect()
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void PlayerEffect::update(float frameTime)
{

}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void PlayerEffect::setPointer(Object* player, D3DXVECTOR3* skyPosition)
{
	//�v���C���[�̃|�C���^
	this->player = player;
	//�V�t�g���C���̃|�C���^
	this->skyPosition = skyPosition;
}

//===================================================================================================================================
//�y�Đ��z
//===================================================================================================================================
void PlayerEffect::play(int i)
{
	switch (i)
	{
	case EFFECT_LIST::DIGHITAL_SHIFT:
		digitalShift = new DigitalShiftEffect(&player->matrixCenter, managerNo);
		effekseerNS::play(managerNo, digitalShift);
		break;
	case EFFECT_LIST::DIGIT_MODE:
		digitMode = new DigitMode(&player->matrixCenter,managerNo);
		effekseerNS::play(managerNo, digitMode);
		break;
	case EFFECT_LIST::SHIFT_TERMINATE:
		shiftTerminate = new ShiftTerminateEffect(&player->matrixCenter,managerNo);
		effekseerNS::play(managerNo, shiftTerminate);
		break;
	case EFFECT_LIST::SKY_JUMP:
		skyJump= new SkyJumpEffect(&player->matrixCenter,skyPosition,managerNo);
		effekseerNS::play(managerNo, skyJump);
		break;
	case EFFECT_LIST::SKY_VISION:
		skyVision = new SkyVisionEffect(&player->matrixCenter,skyPosition,managerNo);
		effekseerNS::play(managerNo, skyVision);
		break;
	}
}

//===================================================================================================================================
//�y��~�z
//===================================================================================================================================
void PlayerEffect::stop(int i)
{
	switch (i)
	{
	case EFFECT_LIST::DIGHITAL_SHIFT:
		effekseerNS::stop(managerNo, digitalShift);
		break;
	case EFFECT_LIST::DIGIT_MODE:
		effekseerNS::stop(managerNo, digitMode);
		break;
	case EFFECT_LIST::SHIFT_TERMINATE:
		effekseerNS::stop(managerNo, shiftTerminate);
		break;
	case EFFECT_LIST::SKY_JUMP:
		effekseerNS::stop(managerNo, skyJump);
		break;
	case EFFECT_LIST::SKY_VISION:
		effekseerNS::stop(managerNo, skyVision);
		break;
	}
}

