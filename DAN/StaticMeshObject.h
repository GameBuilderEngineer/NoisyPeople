//===================================================================================================================================
//�yObject.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/17
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
//�y�X�^�e�B�b�N���b�V���I�u�W�F�N�g�N���X�z
//===================================================================================================================================
class StaticMeshObject:public Base
{
protected:
	//Data
	LPDIRECT3DDEVICE9					device;					//DirectX9�f�o�C�X
	StaticMesh*							staticMesh;				//�X�^�e�B�b�N���b�V��
	LPDIRECT3DVERTEXDECLARATION9		declaration;			//���_�錾
	bool								onRender;				//�`��t���O
	bool								onTransparent;			//���߃t���O
	bool								onLight;				//���C�g�t���O
	bool								didGenerate;			//�����t���O
	bool								didDelete;				//�폜�t���O
	int									objectNum;				//�I�u�W�F�N�g�̐�
	int									fillMode;				//�`�惂�[�h

	//�Z�b�g�X�g���[���p�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				matrixBuffer;			//���[���h�}�g���b�N�X�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				colorBuffer;			//�J���[�o�b�t�@
	LPDIRECT3DVERTEXBUFFER9				uvBuffer;				//UV�o�b�t�@

	//copy�p�z��
	D3DXMATRIX*							worldMatrix;			//���[���h�}�g���b�N�X�z��
	D3DXCOLOR*							color;					//�J���[�z��
	D3DXVECTOR2*						uv;						//�J���[�z��

	//�I�u�W�F�N�g���X�g
	staticMeshObjectNS::ObjectList*		objectList;

public:
	//Method
	StaticMeshObject(StaticMesh* _staticMesh);
	~StaticMeshObject();

	//��{�֐�
	void update();
	void render(LPD3DXEFFECT effect, D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPosition);

	//���X�g����
	void generateObject(Object* newObject);						//�V���ȃI�u�W�F�N�g�̐���
	void deleteObjectByID(int ID);								//�`�悷��C���X�^���X��ID�ɂ��폜
	void deleteObject(int instanceNo);							//�`�悷��C���X�^���X���폜
	void allDelete();											//�`�悷��C���X�^���X��S�폜
	void updateAccessList();									//�����_���A�N�Z�X�p�z��̍X�V�F�C���X�^���X�̐����܂��͍폜���s�������ɍX�V���s���B

	//�e�o�b�t�@�l�X�V
	void updateWorldMatrix();									//���[���h�}�g���b�N�X�o�b�t�@���X�V����
	void updateColor();											//�J���[�o�b�t�@���X�V����

	//�o�b�t�@�̃��\�[�X�X�V
	void updateBuffer();
	//copy�p�z��̃������X�V
	void updateArray();

#ifdef _DEBUG
	virtual void outputGUI();
#endif // _DEBUG

	//setter
	void enableLight();
	void disableLight();

	//getter
	StaticMesh* getStaticMesh();

};