//===================================================================================================================================
//�yTitle.h�z
// [�쐬��]HAL����GP12A332 25 ���� �W��
// [�쐬��]2019/09/20
// [�X�V��]2019/10/17
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
#include "TestEffect.h"
#include "Sound.h"
#include "SoundBase.h"

//#include "SceneEffect.h"
#include "TitleUI.h"

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
	TestEffect* testEffect;

	TitleUI titleUI;										//�^�C�g��UI
	int selectStateMemory;
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