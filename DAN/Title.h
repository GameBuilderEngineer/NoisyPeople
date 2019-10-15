//===================================================================================================================================
//�yTitle.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/09/20
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
#include "InstancingBillboard.h"
#include "Sound.h"
#include "SoundBase.h"

//#include "SceneEffect.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace titleNS
{
	enum TRANSITION_SCENE_LIST
	{
		GAME,
		CREDIT,
		OPERATION,
		TUTORIAL,
		NUM_TRANSITION_SCENE
	};

}

//===================================================================================================================================
//�y�^�C�g���V�[���N���X�z
//===================================================================================================================================
class Title : public AbstractScene
{
private:
	//Player player[titleNS::PLAYER_TYPE::PLAYER_TYPE_MAX];	//	�v���C���[
	//SceneEffect sceneEffect;								//	�V�[���G�t�F�N�g
	//UITitle uiTitle;										//	�^�C�g��UI
	//InstancingBillboard plane;							//	�C���X�^���V���O�r���{�[�h
	InstancingBillboard plane;

	//�Đ��p�����[�^
	PLAY_PARAMETERS playParameters[3];

public:
	Title(void);
	~Title(void);
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	void updateInput(void);
	virtual void render() override;
	void render3D(Camera _currentCamera);
	void render2D();
	virtual void collisions(void) override;
	virtual void AI(void) override;

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};