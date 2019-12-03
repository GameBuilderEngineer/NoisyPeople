//===================================================================================================================================
//�yMapObjectTool.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/11/25
// [�X�V��]2019/11/25
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y���C�u�����̃��[�h�z
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"
#include "MapObject.h"

//===================================================================================================================================
//�y�}�N���z
//===================================================================================================================================
#define MPOJ_CHUNK_ID		(4)
#define MPOJ_CHUNK			("MPOJ")
#define MPOJ_CHUNK			("MFMT")
#define MPOJ_FILE_PATH		("mpoj.mpoj")
#define MPOJ_GUI_ID			(3)

//===================================================================================================================================
//�y�\���́z
//===================================================================================================================================
typedef struct //MPOJ�`�����N
{
	char chunkId[MPOJ_CHUNK_ID];
	short size;
	short mpojMax;	
}MPOJ_MPOJ;

typedef struct //MFMT�`�����N
{
	char chunkId[MPOJ_CHUNK_ID];	
	short size;
	short mpojId;
	short mpojType;
	float posX;
	float posY;
	float posZ;
	float dirX;
	float dirY;
	float dirZ;

}MPOJ_MFMT;

typedef struct	//MPOJ�t�@�C���\����
{
	MPOJ_MPOJ mpoj;		//MPOJ�`�����N	
	MPOJ_MFMT *mfmt;		//MFMT�`�����N
}MPOJ_FILE;

//===================================================================================================================================
//�y�}�b�v�I�u�W�F�N�g�c�[���z
//�}�b�v�I�u�W�F�N�g�c�[���p�N���X
//===================================================================================================================================
class MPOJ_TOOLS : public Base
{
public:
	MPOJ_TOOLS();
	~MPOJ_TOOLS();

	//�t�@�C��
	int GetMpojMax(void);									//�}�b�v�I�u�W�F�N�g�̐����擾
	mapObjectNS::MAPOBJECTSET GetMpojSet(short mpojId);				//�}�b�v�I�u�W�F�N�g�Z�b�g�\���̂��擾

#ifdef _DEBUG
	//�ϐ�
	int MpojListboxType;									//�}�b�v�I�u�W�F�N�g�̎��(���X�g�{�b�N�X)
	BoundingSphere *bodyCollide;							//�����蔻��
	Object** object;										//�I�u�W�F�N�g
	int Model[mapObjectNS::MAPOBJECT_TYPE::MAPOBJECT_MAX];
	//GUI
	void outputMpojToolsGUI(int GUIid,						//GUI
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//�����_���[
	void initRender();										//�����_���[�̏�����
	void initObject();										//�I�u�W�F�N�g�̏�����
	void update();											//�X�V
	void render(D3DXMATRIX view,							//�`��
		D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
	void generateMapobj(Object *object, short mpojType,		//�쐬
		D3DXVECTOR3 position, D3DXVECTOR3 dir);
#endif

private:

	//�ϐ�
	MPOJ_FILE mpojFile;										//�}�b�v�I�u�W�F�N�g�\����

	//�}�b�v�I�u�W�F�N�g�t�@�C��
	void OutputMpojFile(void);								//�}�b�v�I�u�W�F�N�g�t�@�C���̏����o������
	void CreatNewMpojFile(void);							//�}�b�v�I�u�W�F�N�g�t�@�C���̐V�K�쐬

#ifdef _DEBUG
	//�ϐ�	
	StaticMeshRenderer** mapobjRenderer;						//�����_���[
	bool needUpdate;										//�X�V�t���O
	int MpojListboxCurrent;									//�}�b�v�I�u�W�F�N�g�I��p(���X�g�{�b�N�X)

	//�����_���[
	void ResetRenderer(void);								//�����_���[�����Z�b�g

	//�}�b�v�I�u�W�F�N�g�t�H�[�}�b�g�\����
	void UpdateMfmt(int oldMpojMax);						//�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂𐮗�
	void DeleteMpojFormat(short mpojId);					//�}�b�v�I�u�W�F�N�g�̃t�H�[�}�b�g�\���̂�����
	void AddMpojFormat(short mpojType,
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

	//�}�b�v�I�u�W�F�N�g�̐ݒu
	void SetMpojType(short mpojId, short mpojType);			//�}�b�v�I�u�W�F�N�g�̎�ނ�ݒ�
	void SetMpojPos(short mpojId, const D3DXVECTOR3 pos);	//�}�b�v�I�u�W�F�N�g�̈ʒu��ݒ�
	void SetMpojDir(short mpojId, const D3DXVECTOR3 dir);	//�}�b�v�I�u�W�F�N�g�̌�����ݒ�
	void SetMpoj(short mpojId, short mpojType,				//�}�b�v�I�u�W�F�N�g�̐ݒu
		const D3DXVECTOR3 pos, const D3DXVECTOR3 dir);

#endif
};