//===================================================================================================================================
//yTreeTypeB.cppz
// [์ฌา]HALGP12A332 11 ์ ๗
// [์ฌ๚]2019/10/14
// [XV๚]2019/10/23
//===================================================================================================================================

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "TreeTypeB.h"

//===================================================================================================================================
//yRXgN^z
//===================================================================================================================================
TreeTypeB::TreeTypeB() {
	num = 400;
	renderer = new StaticMeshRenderer(staticMeshNS::reference(staticMeshNS::GREEN_TREE_002));
	initialize();
};

//===================================================================================================================================
//yfXgN^z
//===================================================================================================================================
TreeTypeB::~TreeTypeB() {
	SAFE_DELETE_ARRAY(object);
	SAFE_DELETE(renderer);
};

//===================================================================================================================================
//y๚ปz
//===================================================================================================================================
void TreeTypeB::initialize() {
	object = new Object[num];
	D3DXVECTOR3 position;
	D3DXVECTOR3 deltaScale;
	for (int i = 0; i < num; i++)
	{
		position.x = (float)((rand() % 2000) - 1000);
		position.y = 0.0f;
		position.z = (float)((rand() % 2000) - 1000);
		object[i].initialize(&position);
		
		deltaScale.x = (float)((rand() % 100))*0.01;
		deltaScale.y = (float)((rand() % 100))*0.01;
		deltaScale.z = (float)((rand() % 100))*0.01;

		object[i].scale += deltaScale;

		renderer->registerObject(&object[i]);//_[ึo^
	}
	needUpdate = true;
};

//===================================================================================================================================
//yXVz
//===================================================================================================================================
void TreeTypeB::update()
{
	if (!needUpdate)return;
	renderer->update();
	needUpdate = false;
}

//===================================================================================================================================
//y`ๆz
//===================================================================================================================================
void TreeTypeB::render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon)
{
	renderer->render(*shaderNS::reference(shaderNS::INSTANCE_STATIC_MESH), view, projection, cameraPositon);
}

