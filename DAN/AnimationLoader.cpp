//===================================================================================================================================
//�yAnimationLoader.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/12
// [�X�V��]2019/09/12
//===================================================================================================================================
#include "AnimationLoader.h"

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
AnimationLoader::AnimationLoader()
{
	
}


//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
AnimationLoader::~AnimationLoader()
{

	for (int i = 0; i < animationPlayerNS::PLAYER_TYPE_MAX; i++)
	{
		for (int j = 0; j < animationPlayerNS::MODEL_TYPE_MAX; j++)
		{
			animation[i][j].release();
		}
	}
}

void AnimationLoader::initialize(LPDIRECT3DDEVICE9 device)
{
	for (int i = 0; i < animationPlayerNS::PLAYER_TYPE_MAX; i++)
	{
		for (int j = 0; j < animationPlayerNS::MODEL_TYPE_MAX; j++)
		{
			animation[i][j].initialize(device, i, j);
		}
	}
}

AnimationPlayer* AnimationLoader::getAnimationModel(int playerIndex,int modelType)
{
	return &animation[playerIndex][modelType];
}