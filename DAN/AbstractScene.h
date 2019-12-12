//===================================================================================================================================
//�yAbstractScene.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/19
// [�X�V��]2019/11/01
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include <string>
#include "Base.h"
#include "Direct3D9.h"
#include "Input.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Light.h"
#include "TextureLoader.h"
#include "StaticMeshLoader.h"
#include "EffekseerManager.h"
#include "Fader.h"
#include "GameMaster.h"
//#include "TextManager.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
//�V�[�����X�g
namespace SceneList
{
	enum {
		NONE_SCENE = -1,
		DEBUG,			//�f�o�b�O�V�[��
		SPLASH,			//�X�v���b�V��
		TITLE,			//�^�C�g��
		TUTORIAL,		//�`���[�g���A��
		CREDIT,			//�N���W�b�g
		GAME,			//�Q�[��
		RESULT,			//���U���g
		CREATE,			//�N���G�C�g
		FINALE			//�t�B�i�[��
	};
}

//===================================================================================================================================
//�y���V�[���N���X�F���ۃC���^�[�t�F�[�X�z
//�e�V�[���̌p�����N���X
//===================================================================================================================================
class AbstractScene :public Base
{
protected:
	//Data
	LPDIRECT3DDEVICE9	device;
	Direct3D9*			direct3D9;
	Input*				input;
	Camera*				camera;
	Light*				light;
	GameMaster*			gameMaster;
	float				sceneTimer;
	float				frameTime = 0.0f;	//�t���[���^�C���ۑ��ϐ�
	float				stopTimer = 0.0f;	//������������
	bool				onChange;
	int					nextScene;	
	std::string			sceneName;
#ifdef _DEBUG
	bool				showGUI;
#endif // _DEBUG

public:
	//VirtualMethod
	AbstractScene();									//�R���X�g���N�^
	virtual			~AbstractScene();					//�f�X�g���N�^
	virtual void	initialize()				= 0;	//������
	virtual void	uninitialize()				= 0;	//�I������
	virtual void	update(float frameTime)		= 0;	//�X�V
	virtual void	render()					= 0;	//�`��
	virtual void	collisions()				= 0;	//�Փˏ���
	virtual void	AI()						= 0;	//AI����
#ifdef _DEBUG
	virtual void	createGUI()					= 0;	//GUI����
	bool*			getShowGUI() { return &showGUI; };
#endif // _DEBUG

	//Method
	void changeScene(int i) { onChange = true; nextScene = i; };		//�V�[���J�ڏ����֐�
	bool checkChangeOrder() { return onChange; };						//�V�[���J�ڗv���֐�
	int checkNextScene() { return nextScene; }							//���V�[���Q�Ɗ֐�
	std::string* getSceneName() { return &sceneName; }					//�V�[�����擾�֐�
	void copyGameMaster(GameMaster* destination);						//�Q�[���Ǘ����̈��p��
	void setGameMaster(GameMaster* resorce);							//�Q�[���Ǘ����̎󂯎��
};
