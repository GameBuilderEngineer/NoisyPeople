//===================================================================================================================================
//�yResult.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/20
// [�X�V��]2019/12/05
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "AbstractScene.h"
#include "ResultUI.h"
#include "StaticMeshRenderer.h"
#include "TreeManager.h"
#include "GameMaster.h"
//#include "Sprite.h"
//#include "UIResult.h"
//#include "SceneEffect.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace resultNS
{
	const D3DXQUATERNION	CAMERA_RELATIVE_QUATERNION	= D3DXQUATERNION(0.0f, 300.0f, -300.0f, 0.0f);
	const float				CAMERA_SPEED				= 30.0f;

	//�Đ����x
	const float				PLAYBACK_SPEED				= 10.0f;

}

//===================================================================================================================================
//�y���U���g�V�[���N���X�z
//===================================================================================================================================
class Result :public AbstractScene
{
private:
	ResultUI resultUI;

	//�t�B�[���h
	Object*							testField;			//�t�B�[���h
	StaticMeshRenderer*				testFieldRenderer;	//�t�B�[���h�����_���[

	TreeManager*					treeManager;		//�c���[

	float							playbackTimer;		//�Đ��^�C�}�[


public:
	Result(void);
	~Result(void);
	virtual void initialize() override;
	virtual void uninitialize(void) override;
	virtual void update(float _frameTime) override;
	virtual void render() override;
	virtual void collisions(void) override;
	virtual void AI(void) override;

	void render3D(Camera currentCamera);
	void renderUI();

#ifdef _DEBUG
	virtual void createGUI() override;
#endif
};