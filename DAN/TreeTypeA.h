//===================================================================================================================================
//yTreeTypeA.hz
// [์ฌา]HALGP12A332 11 ์ ๗
// [์ฌ๚]2019/10/16
// [XV๚]2019/10/23
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Base.h"
#include "StaticMeshRenderer.h"

//===================================================================================================================================
//yุ`NXz
//===================================================================================================================================
class TreeTypeA :	public Base
{
private:
	int num;
	Object* object;
	StaticMeshRenderer* renderer;
	bool needUpdate;
public:
	TreeTypeA();
	~TreeTypeA();
	void initialize();
	void update();
	void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);
};

