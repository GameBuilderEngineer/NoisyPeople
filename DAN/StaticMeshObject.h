//===================================================================================================================================
//�yObject.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/16
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Object.h"
#include "LinkedList.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace staticMeshObjectNS
{
	enum FILLMODE
	{
		SOLID = (int)D3DFILL_SOLID,
		WIREFRAME = (int)D3DFILL_WIREFRAME,
		POINT = (int)D3DFILL_POINT,
	};

	class ObjectList :public LinkedList<Object*>
	{

	};
}

//===================================================================================================================================
//�y�X�^�e�B�b�N���b�V���I�u�W�F�N�g�z
//===================================================================================================================================
class StaticMeshObject:public Base
{
protected:
	//Data
	LPDIRECT3DDEVICE9							device;					//DirectX9�f�o�C�X
	StaticMesh*										staticMesh;			//�X�^�e�B�b�N���b�V��
	LPDIRECT3DVERTEXDECLARATION9	declaration;			//���_�錾
	bool													onRender;				//�`��t���O
	bool													onTransparent;		//���߃t���O
	bool													didGenerate;			//�����t���O
	bool													didDelete;				//�폜�t���O
	int														objectNum;			//�I�u�W�F�N�g�̐�
	int														fillMode;				//

	//�Z�b�g�X�g���[���p�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				positionBuffer;		//�ʒu�o�b�t�@

	//copy�p�z��
	D3DXVECTOR3*									position;				//�ʒu�z��

	//�I�u�W�F�N�g���X�g
	staticMeshObjectNS::ObjectList* objectList;

public:
	//Method
	StaticMeshObject(StaticMesh* _staticMesh);
	~StaticMeshObject();

	void update();
	void updateBuffer();
	void updatePosition();
	void updateArray();
	void render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//���X�g����
	void generateObject(Object* newObject);							//�V���ȃI�u�W�F�N�g�̐���
	void deleteObject(int instanceNo);										//�`�悷��C���X�^���X���폜
	void updateAccessList();														//�����_���A�N�Z�X�p�z��̍X�V�F�C���X�^���X�̐����܂��͍폜���s�������ɍX�V���s���B

#ifdef _DEBUG
	virtual void outputGUI();
#endif // _DEBUG

	//getter
	StaticMesh* getStaticMesh();
};


