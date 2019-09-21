//===================================================================================================================================
//�yResult.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
//#include "Sprite.h"
//#include "UIResult.h"
//#include "SceneEffect.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace resultNS
{
	static const float EFFECT_MAX = 1500;	// �G�t�F�N�g���

	const float SELECT_TIME = (60.0f);
	//const float POSITION_TIME_X = ((uiCharacterSelectNS::POSITION_X_TIME) - 20.0f);
	//const float POSITION_TIME_Y = ((uiCharacterSelectNS::POSITION_Y_TIME) - 125.0f);

	enum PLAYER_TYPE
	{
		PLAYER_1,
		PLAYER_2,
		PLAYER_TYPE_MAX
	};
	const D3DXQUATERNION CAMERA_RELATIVE_QUATERNION[resultNS::PLAYER_TYPE::PLAYER_TYPE_MAX] =
	{
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f),
#ifdef _DEBUG
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#else
		D3DXQUATERNION(0.0f,20.0f,-40.0f,0.0f)
#endif // _DEBUG
	};
}

//===================================================================================================================================
//�y���U���g�V�[���N���X�z
//===================================================================================================================================
class Result :public AbstractScene
{
private:
	//UIResult uiResult;			//	���U���gUI
	//TextManager* textManager;	//	�e�L�X�g�}�l�[�W��
	//SceneEffect sceneEffect;//	�V�[���G�t�F�N�g

public:
	Result(void);
	~Result(void);
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	virtual void render() override;
	virtual void collisions(void) override;
	virtual void AI(void) override;

	//void render3D(Camera _currentCamera);
	void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};