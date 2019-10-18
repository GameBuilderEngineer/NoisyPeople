//===================================================================================================================================
//�yAnimationLoader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/12
// [�X�V��]2019/09/12
//===================================================================================================================================
#pragma once
#include "Base.h"
#include "AnimationPlayer.h"

class AnimationLoader :
	public Base
{
private:
	AnimationPlayer animation[animationPlayerNS::PLAYER_TYPE_MAX][animationPlayerNS::MODEL_TYPE_MAX];					//	�A�j���[�V����

public:
	AnimationLoader();
	~AnimationLoader();
	void initialize(LPDIRECT3DDEVICE9 device);
	AnimationPlayer* getAnimationModel(int playerIndex,int modelType);

};

