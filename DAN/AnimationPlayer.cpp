//============================================================================================================================================
// Document
//============================================================================================================================================
// AnimationPlayer.cpp
// HAL���� GP-12A-332 09 �T���v��
// 2019/09/07
//============================================================================================================================================
#include "AnimationPlayer.h"
//============================================================================================================================================
// Using Declaration
// using�錾
//============================================================================================================================================
using namespace animationPlayerNS;
//============================================================================================================================================
// Global Variable
// �O���[�o���ϐ�
//============================================================================================================================================
bool initializeFlag = false;
//============================================================================================================================================
// Constructor
// �R���X�g���N�^
//============================================================================================================================================
AnimationPlayer::AnimationPlayer(void)
{
	animation = NULL;				//	�A�j���[�V����
	animationID = { NULL,NULL };	//	�A�j���[�V����ID
	flag.moveBan = false;			//	�ړ��֎~�t���O

	return;
}
//============================================================================================================================================
// Destructor
// �f�X�g���N�^
//============================================================================================================================================
AnimationPlayer::~AnimationPlayer(void)
{
	return;
}
//============================================================================================================================================
// initialize
// ������
//============================================================================================================================================
HRESULT AnimationPlayer::initialize(LPDIRECT3DDEVICE9 _device, int _playerIndex, int _modelType)
{
	// �ϐ��ɑ��
	playerIndex = _playerIndex;
	modelType = _modelType;

	// �L�[�e�[�u���̐ݒ�
	switch (_playerIndex)
	{
	case PLAYER_TYPE::PLAYER_1:
		keyTable = KEY_TABLE_PLAYER_1;
		break;
	case PLAYER_TYPE::PLAYER_2:
		keyTable = KEY_TABLE_PLAYER_2;
		break;
	default:
		keyTable = KEY_TABLE_NONE;
		break;
	}

	// AI�A�j���[�V�����t���O�̏�����
	for (int i = 0; i < AI_FLAG_MAX; i++)
	{
		aiAnimFlag[i] = false;
	}

	// �A�j���[�V�����Z�b�g��
	const char* animationSetName[animationNS::TYPE::TYPE_MAX] =
	{
		"IdleGeneral",
		"Idle",
		"Shooting",
		"Run",
		"RunFast",
		"Jump",
		"Installation",
		"Standing",
		"Slash",
		"Down",
		"DownPose",
		"Revival",
		"Recursion",
		"Floating",
		"Falling",
		"Landing"
	};

	// �A�j���[�V�������쐬
	animation = createObject();

	// �f�B���N�g���ݒ�
	setVisualDirectory();

	// X�t�@�C����ǂݍ���
	switch (_modelType)
	{
	case MODEL_TYPE::ADAM:
		loadXFile(_device, animation, ("Character_Adam.x"));
		break;
	case MODEL_TYPE::EVE:
		loadXFile(_device, animation, ("Character_Eve.x"));
		break;
	default:
		break;
	}

	// �A�j���[�V�����R�[���o�b�N�̐ݒ�
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::JUMP]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::INSTALLATION]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::STANDING]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::SLASH]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::DOWN]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::REVIVAL]);
	setCallBackKeyFrame(animation, animationSetName[animationNS::TYPE::LANDING]);

	// �A�j���[�V�����Z�b�g�̏�����
	for (int i = 0; i < animation->animationSetMax; i++)
	{
		animation->initialize(animation, animationSetName[i], i);
	}

	// �p�����[�^������
	animation->animationSpeed = 1.0f;

	// �A�j���[�V�����V�t�g�^�C���̏�����
	animation->setShiftTime(animation, animationNS::TYPE::IDLE_GENERAL, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::IDLE, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::SHOOTING, 0.1f);
	animation->setShiftTime(animation, animationNS::TYPE::RUN, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::RUN_FAST, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::JUMP, 0.5f);
	animation->setShiftTime(animation, animationNS::TYPE::INSTALLATION, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::STANDING, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::SLASH, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::DOWN, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::DOWN_POSE, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::REVIVAL, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::RECURSION, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::FLOATING, 1.0f);
	animation->setShiftTime(animation, animationNS::TYPE::FALLING, 0.25f);
	animation->setShiftTime(animation, animationNS::TYPE::LANDING, 0.25f);

	return S_OK;
}
//============================================================================================================================================
// release
// ���
//============================================================================================================================================
void AnimationPlayer::release(void)
{
	if (playerIndex == animationPlayerNS::PLAYER_2)
	{
		int unko = 9;
	}

	// �A�j���[�V�����̉��
	animation->release(animation);
	SAFE_DELETE(animation);

	return;
}
//============================================================================================================================================
// update
// �X�V
//============================================================================================================================================
void AnimationPlayer::update(Input* _input, int _state)
{
	// �A�j���[�V����ID�̍X�V
	updateAnimationID(_input, _state);

	// �A�j���[�V�����̍X�V
	animation->update(animation, animationNS::TIME_PER_FRAME);

	return;
}
//============================================================================================================================================
// updateAnimationID
// �X�V - �A�j���[�V����ID
//============================================================================================================================================
void AnimationPlayer::updateAnimationID(Input* _input, int _state)
{
	// �A�j���[�V�����Z�b�g( �� )�̍X�V
	switch (animationID.current)
	{
	case animationNS::TYPE::IDLE:
		updateAnimationIDCurrentIdle(_input, _state);
		break;
	case animationNS::TYPE::SHOOTING:
		updateAnimationIDCurrentShooting(_input, _state);
		break;
	case animationNS::TYPE::RUN:
		updateAnimationIDCurrentRun(_input, _state);
		break;
	case animationNS::TYPE::RUN_FAST:
		updateAnimationIDCurrentRunFast(_input, _state);
		break;
	case animationNS::TYPE::JUMP:
		updateAnimationIDCurrentJump(_input, _state);
		break;
	case animationNS::TYPE::INSTALLATION:
		updateAnimationIDCurrentInstallation(_input, _state);
		break;
	case animationNS::TYPE::STANDING:
		updateAnimationIDCurrentStanding(_input, _state);
		break;
	case animationNS::TYPE::SLASH:
		updateAnimationIDCurrentSlash(_input, _state);
		break;
	case animationNS::TYPE::DOWN:
		updateAnimationIDCurrentDown(_input);
		break;
	case animationNS::TYPE::DOWN_POSE:
		updateAnimationIDCurrentDownPose(_input);
		break;
	case animationNS::TYPE::REVIVAL:
		updateAnimationIDCurrentRevival(_input, _state);
		break;
	case animationNS::TYPE::RECURSION:
		updateAnimationIDCurrentRecursion(_input);
		break;
	case animationNS::TYPE::FLOATING:
		updateAnimationIDCurrentFloating(_input);
		break;
	case animationNS::TYPE::FALLING:
		updateAnimationIDCurrentFalling(_input);
		break;
	case animationNS::TYPE::LANDING:
		updateAnimationIDCurrentLanding(_input, _state);
		break;
	default:
		break;
	}

	// �A�j���[�V�����Z�b�g( �� )�̍X�V
	switch (animationID.next)
	{
	case animationNS::TYPE::IDLE:
		updateAnimationIDNextIdle();
		break;
	case animationNS::TYPE::SHOOTING:
		updateAnimationIDNextShooting();
		break;
	case animationNS::TYPE::RUN:
		updateAnimationIDNextRun();
		break;
	case animationNS::TYPE::RUN_FAST:
		updateAnimationIDNextRunFast();
		break;
	case animationNS::TYPE::JUMP:
		updateAnimationIDNextJump();
		break;
	case animationNS::TYPE::INSTALLATION:
		updateAnimationIDNextInstallation();
		break;
	case animationNS::TYPE::STANDING:
		updateAnimationIDNextStanding();
		break;
	case animationNS::TYPE::SLASH:
		updateAnimationIDNextSlash();
		break;
	case animationNS::TYPE::DOWN:
		updateAnimationIDNextDown();
		break;
	case animationNS::TYPE::DOWN_POSE:
		updateAnimationIDNextDownPose();
		break;
	case animationNS::TYPE::REVIVAL:
		updateAnimationIDNextRevival();
		break;
	case animationNS::TYPE::RECURSION:
		updateAnimationIDNextRecursion();
		break;
	case animationNS::TYPE::FLOATING:
		updateAnimationIDNextFloating();
		break;
	case animationNS::TYPE::FALLING:
		updateAnimationIDNextFalling();
		break;
	case animationNS::TYPE::LANDING:
		updateAnimationIDNextLanding();
		break;
	default:
		break;
	}

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentIdle
// �X�V - �A�j���[�V����ID ( ���F�A�C�h�� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentIdle(Input* _input, int _state)
{
	if (playerIndex == animationPlayerNS::PLAYER_2)
	{
		int unko = 9;
	}

	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// �W�����v
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// �������[�E�p�C���ݒu
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// �������[�E���C���ؒf
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// ����E��������
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// �ˌ�
	if (_input->getMouseLButton() || _input->getController()[playerIndex]->isButton(virtualControllerNS::BUTTONS::R1) ||
		aiAnimFlag[SHOOTING])
	{
		animationID.next = animationNS::TYPE::SHOOTING;
		return;
	}
	// �A�C�h��
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentShooting
// �X�V - �A�j���[�V����ID ( ���F�ˌ� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentShooting(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// �W�����v
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// �������[�E�p�C���ݒu
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		aiAnimFlag[INSTALLATION] = false;
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// �������[�E���C���ؒf
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		aiAnimFlag[SLASH] = false;
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// ����E��������
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		aiAnimFlag[MOVE] = false;
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// �ˌ�
	if (_input->getMouseLButton() || _input->getController()[playerIndex]->isButton(virtualControllerNS::BUTTONS::R1) ||
		aiAnimFlag[SHOOTING])
	{
		aiAnimFlag[SHOOTING] = false;
		animationID.next = animationNS::TYPE::SHOOTING;
		return;
	}
	// �A�C�h��
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRun
// �X�V - �A�j���[�V����ID ( ���F���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRun(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// �W�����v
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// �������[�E�p�C���ݒu
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// �������[�E���C���ؒf
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// ����E��������
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// �A�C�h��
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRunFast
// �X�V - �A�j���[�V����ID ( ���F�������� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRunFast(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}
	// �W�����v
	if (_input->isKeyDown(keyTable.jump) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::B))
	{
		animationID.next = animationNS::TYPE::JUMP;
		return;
	}
	// �������[�E�p�C���ݒu
	if (_input->getMouseRButtonTrigger() || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::L1) ||
		aiAnimFlag[INSTALLATION])
	{
		animationID.next = animationNS::TYPE::INSTALLATION;
		return;
	}
	// �������[�E���C���ؒf
	if (_input->getMouseWheelState() == inputNS::DOWN || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::X) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000) && (_state == STATE_TYPE::GROUND) ||
		aiAnimFlag[SLASH])
	{
		animationID.next = animationNS::TYPE::SLASH;
		return;
	}
	// ����E��������
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}
	// �A�C�h��
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentJump
// �X�V - �A�j���[�V����ID ( ���F�W�����v )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentJump(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::JUMP;

	if (!animation->flag.animationPlayEnd) { return; }

	setFlagJump(false);

	// ����E��������
	if (_input->isKeyDown(keyTable.front) || _input->isKeyDown(keyTable.back) || _input->isKeyDown(keyTable.left) || _input->isKeyDown(keyTable.right) ||
		_input->getController()[playerIndex]->checkConnect() || aiAnimFlag[MOVE])
	{
		animationID.next = animationNS::TYPE::RUN_FAST;
		if (_input->isKeyDown(keyTable.dash) || _input->getController()[playerIndex]->wasButton(virtualControllerNS::BUTTONS::Y)) { return; }
		animationID.next = animationNS::TYPE::RUN;

		return;
	}

	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentInstallation
// �X�V - �A�j���[�V����ID ( ���F���� �� �������[�E�p�C���ݒu )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentInstallation(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}

	animationID.next = animationNS::TYPE::INSTALLATION;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::STANDING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentStanding
// �X�V - �A�j���[�V����ID ( ���F�������[�E�p�C���ݒu �� ���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentStanding(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		if (getFlagInstallation()) { setFlagInstallation(false); }
		return;
	}

	animationID.next = animationNS::TYPE::STANDING;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::IDLE;
	if (getFlagInstallation()) { setFlagInstallation(false); }

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentSlash
// �X�V - �A�j���[�V����ID ( ���F�������[�E���C���ؒf )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentSlash(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::SLASH;

	if (!animation->flag.animationPlayEnd) { return; }

	if (getFlagMoveBan()) { setFlagMoveBan(false); }
	if (getFlagSlash()) { setFlagSlash(false); }
	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentDown
// �X�V - �A�j���[�V����ID ( ���F�_�E�� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentDown(Input* _input)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		animation->flag.animationOn = true;
		return;
	}
	// ����
	if (flag.revival)
	{
		animationID.next = animationNS::TYPE::REVIVAL;
		animation->flag.animationOn = true;
		return;
	}

	animationID.next = animationNS::TYPE::DOWN;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::DOWN_POSE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentDownPose
// �X�V - �A�j���[�V����ID ( ���F�_�E���|�[�Y )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentDownPose(Input* _input)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		animation->flag.animationOn = true;
		return;
	}
	// ����
	if (flag.revival)
	{
		animationID.next = animationNS::TYPE::REVIVAL;
		animation->flag.animationOn = true;
		return;
	}

	animationID.next = animationNS::TYPE::DOWN_POSE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRevival
// �X�V - �A�j���[�V����ID ( ���F���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRevival(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::REVIVAL;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentRecursion
// �X�V - �A�j���[�V����ID ( ���F���J�[�W���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentRecursion(Input* _input)
{
	animationID.next = animationNS::TYPE::RECURSION;

	if (flag.recursion) { return; }

	animationID.next = animationNS::TYPE::FLOATING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentFloating
// �X�V - �A�j���[�V����ID ( ���F���V )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentFloating(Input* _input)
{
	animationID.next = animationNS::TYPE::FLOATING;

	if (!flag.falling) { return; }

	animationID.next = animationNS::TYPE::FALLING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentFalling
// �X�V - �A�j���[�V����ID ( ���F���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentFalling(Input* _input)
{
	animationID.next = animationNS::TYPE::FALLING;

	if (!flag.landing) { return; }

	animationID.next = animationNS::TYPE::LANDING;

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentLanding
// �X�V - �A�j���[�V����ID ( ���F���n )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentLanding(Input* _input, int _state)
{
	// �탊�J�[�W����
	if (flag.recursion)
	{
		animationID.next = animationNS::TYPE::RECURSION;
		return;
	}
	// �_�E��
	if (_state == STATE_TYPE::DOWN)
	{
		animationID.next = animationNS::TYPE::DOWN;
		return;
	}

	animationID.next = animationNS::TYPE::LANDING;

	if (!animation->flag.animationPlayEnd) { return; }

	animationID.next = animationNS::TYPE::IDLE;

	return;
}
//============================================================================================================================================
// updateAnimationIDNextIdle
// �X�V - �A�j���[�V����ID ( ���F�A�C�h�� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextIdle(void)
{
	if (animationID.current == animationNS::TYPE::IDLE) { return; }

	animationID.current = animationNS::TYPE::IDLE;
	animation->animationSpeed = 1.0f;
	if (getFlagRevival()) { setFlagRevival(false); }
	if (getFlagLanding()) { setFlagLanding(false); }
	if (getFlagMoveBan()) { setFlagMoveBan(false); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::IDLE);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextShooting
// �X�V - �A�j���[�V����ID ( ���F�ˌ� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextShooting(void)
{
	if (animationID.current == animationNS::TYPE::SHOOTING) { return; }

	animationID.current = animationNS::TYPE::SHOOTING;
	animation->animationSpeed = 1.0f;
	if (!getFlagGunRender()) { setFlagGunRender(true); }
	switching(animation, animationNS::TYPE::SHOOTING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRun
// �X�V - �A�j���[�V����ID ( ���F���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRun(void)
{
	if (animationID.current == animationNS::TYPE::RUN) { return; }

	animationID.current = animationNS::TYPE::RUN;
	animation->animationSpeed = 1.0f;
	if (!getFlagGunRender()) { setFlagGunRender(true); }
	switching(animation, animationNS::TYPE::RUN);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRunFast
// �X�V - �A�j���[�V����ID ( ���F�������� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRunFast(void)
{
	if (animationID.current == animationNS::TYPE::RUN_FAST) { return; }

	animationID.current = animationNS::TYPE::RUN_FAST;
	animation->animationSpeed = 1.0f;
	if (!getFlagGunRender()) { setFlagGunRender(true); }
	switching(animation, animationNS::TYPE::RUN_FAST);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextJump
// �X�V - �A�j���[�V����ID ( ���F�W�����v )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextJump(void)
{
	if (animationID.current == animationNS::TYPE::JUMP) { return; }

	animationID.current = animationNS::TYPE::JUMP;
	animation->animationSpeed = 0.75f;
	if (!getFlagJump()) { setFlagJump(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::JUMP);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextInstallation
// �X�V - �A�j���[�V����ID ( ���F���� �� �������[�E�p�C���ݒu )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextInstallation(void)
{
	if (animationID.current == animationNS::TYPE::INSTALLATION) { return; }

	animationID.current = animationNS::TYPE::INSTALLATION;
	animation->animationSpeed = 1.5f;
	if (!getFlagMoveBan()) { setFlagMoveBan(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::INSTALLATION);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextStanding
// �X�V - �A�j���[�V����ID ( ���F�������[�E�p�C���ݒu �� ���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextStanding(void)
{
	if (animationID.current == animationNS::TYPE::STANDING) { return; }

	animationID.current = animationNS::TYPE::STANDING;
	animation->animationSpeed = 1.5f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::STANDING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextSlash
// �X�V - �A�j���[�V����ID ( ���F�������[�E���C���ؒf )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextSlash(void)
{
	if (animationID.current == animationNS::TYPE::SLASH) { return; }

	animationID.current = animationNS::TYPE::SLASH;
	animation->animationSpeed = 2.0f;
	if (!getFlagMoveBan()) { setFlagMoveBan(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	if (!getFlagSlash()) { setFlagSlash(true); }
	switching(animation, animationNS::TYPE::SLASH);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextDown
// �X�V - �A�j���[�V����ID ( ���F�_�E�� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextDown(void)
{
	if (animationID.current == animationNS::TYPE::DOWN) { return; }

	animationID.current = animationNS::TYPE::DOWN;
	animation->animationSpeed = 1.0f;
	if (!getFlagMoveBan()) { setFlagMoveBan(true); }
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::DOWN);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextDownPose
// �X�V - �A�j���[�V����ID ( ���F�_�E���|�[�Y )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextDownPose(void)
{
	if (animationID.current == animationNS::TYPE::DOWN_POSE) { return; }

	animationID.current = animationNS::TYPE::DOWN_POSE;
	animation->animationSpeed = 1.0f;
	animation->flag.animationOn = false;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::DOWN_POSE);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRevival
// �X�V - �A�j���[�V����ID ( ���F���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRevival(void)
{
	if (animationID.current == animationNS::TYPE::REVIVAL) { return; }

	animationID.current = animationNS::TYPE::REVIVAL;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::REVIVAL);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextRecursion
// �X�V - �A�j���[�V����ID ( ���F���J�[�W���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextRecursion(void)
{
	if (animationID.current == animationNS::TYPE::RECURSION) { return; }

	animationID.current = animationNS::TYPE::RECURSION;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::RECURSION);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextFloating
// �X�V - �A�j���[�V����ID ( ���F���V )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextFloating(void)
{
	if (animationID.current == animationNS::TYPE::FLOATING) { return; }

	animationID.current = animationNS::TYPE::FLOATING;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::FLOATING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextFalling
// �X�V - �A�j���[�V����ID ( ���F���� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextFalling(void)
{
	if (animationID.current == animationNS::TYPE::FALLING) { return; }

	animationID.current = animationNS::TYPE::FALLING;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	switching(animation, animationNS::TYPE::FALLING);

	return;
}
//============================================================================================================================================
// updateAnimationIDNextLanding
// �X�V - �A�j���[�V����ID ( ���F���n )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextLanding(void)
{
	if (animationID.current == animationNS::TYPE::LANDING) { return; }

	animationID.current = animationNS::TYPE::LANDING;
	animation->animationSpeed = 1.0f;
	if (getFlagGunRender()) { setFlagGunRender(false); }
	if (getFlagFalling()) { setFlagFalling(false); }
	switching(animation, animationNS::TYPE::LANDING);

	return;
}
//============================================================================================================================================
// updateTitle
// �X�V - �^�C�g��
//============================================================================================================================================
void AnimationPlayer::updateTitle(void)
{
	// �A�j���[�V�����Z�b�g( �� )�̍X�V
	updateAnimationIDCurrentIdleGeneral();

	// �A�j���[�V�����Z�b�g( �� )�̍X�V
	updateAnimationIDNextIdleGeneral();

	// �A�j���[�V�����̍X�V
	animation->update(animation, animationNS::TIME_PER_FRAME);

	return;
}
//============================================================================================================================================
// updateAnimationIDCurrentIdleGeneral
// �X�V - �A�j���[�V����ID ( ���F�ėp�A�C�h�� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDCurrentIdleGeneral(void)
{
	// �A�C�h��
	animationID.next = animationNS::TYPE::IDLE_GENERAL;

	return;
}
//============================================================================================================================================
// updateAnimationIDNextIdleGeneral
// �X�V - �A�j���[�V����ID ( ���F�ėp�A�C�h�� )
//============================================================================================================================================
void AnimationPlayer::updateAnimationIDNextIdleGeneral(void)
{
	if (animationID.current == animationNS::TYPE::IDLE_GENERAL) { return; }

	animationID.current = animationNS::TYPE::IDLE_GENERAL;
	animation->animationSpeed = 1.0f;
	switching(animation, animationNS::TYPE::IDLE_GENERAL);

	return;
}
//============================================================================================================================================
// render
// �`��
//============================================================================================================================================
void AnimationPlayer::render(LPDIRECT3DDEVICE9 _device, D3DXMATRIX _matrixRotation,D3DXMATRIX _matrixPosition, StaticMeshLoader* _staticMeshLoader)
{
	D3DMATERIAL9 materialDefault;	//	�}�e���A��

	D3DXMATRIX matrixWorld;

	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, D3DXToRadian(180.0f));
	D3DXMatrixMultiply(&rotation, &rotation,&_matrixRotation);

	D3DXMatrixMultiply(&matrixWorld, &rotation, &_matrixPosition);

	// ���C�e�B���O���[�h��ݒ�
	_device->SetRenderState(D3DRS_LIGHTING, true);

	// ���f���^�C�v�ŃJ�����O�ݒ�̐؂�ւ�
	switch (modelType)
	{
	case MODEL_TYPE::ADAM: _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); break;
	case MODEL_TYPE::EVE: _device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); break;
	default: break;
	}

	// ���[���h�}�g���N�X�̐ݒ�
	_device->SetTransform(D3DTS_WORLD, &matrixWorld);

	// ���݂̃}�e���A�����擾
	_device->GetMaterial(&materialDefault);

	// �A�j���[�V�����̕`��
	animation->render(_device, animation, &matrixWorld);

	// �}�e���A����߂�
	_device->SetMaterial(&materialDefault);

	// ���C�e�B���O���[�h��ݒ�
	_device->SetRenderState(D3DRS_LIGHTING, false);
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	return;
}
//============================================================================================================================================
// setAnimationConfiguration
// �ݒ� - �A�j���[�V�����ݒ�
//============================================================================================================================================
void AnimationPlayer::setAnimationConfiguration(int _scene)
{
	switch (_scene)
	{
	case SCENE_TYPE::TITLE:
	case SCENE_TYPE::CHARACTER_SELECT:
		animationID.current = animationNS::TYPE::IDLE_GENERAL;
		//animationID.next = animationNS::TYPE::IDLE_GENERAL;
		animation->switching(animation, animationNS::TYPE::IDLE_GENERAL);
		animation->flag.animationOn = true;
		animation->flag.animationPlayEnd = false;
		//animation->update(animation, animationNS::TIME_PER_FRAME);
		break;
	case SCENE_TYPE::GAME:
		animationID.current = animationNS::TYPE::IDLE;
		//animationID.next = animationNS::TYPE::IDLE;
		animation->switching(animation, animationNS::TYPE::IDLE);
		animation->flag.animationOn = true;
		animation->flag.animationPlayEnd = false;
		//animation->update(animation, animationNS::TIME_PER_FRAME);
		break;
	default:
		break;
	}

	return;
}
//============================================================================================================================================
// resetAnimation
// ���Z�b�g
//============================================================================================================================================
void AnimationPlayer::resetAnimation(void)
{
	flag.moveBan = false;
	flag.gunRenderFlag = false;
	flag.jump = false;
	flag.installation = false;
	flag.slash = false;
	flag.recursion = false;
	flag.falling = false;
	flag.landing = false;
	flag.revival = false;

	return;
}