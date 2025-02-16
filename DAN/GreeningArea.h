//===================================================================================================================================
//yGreeningArea.hz
// [์ฌา]HALGP12A332 11 ์ ๗
// [์ฌ๚]2019/11/15
// [XV๚]2019/11/15
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Object.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//yผO๓ิz
//===================================================================================================================================
namespace GreeningAreaNS
{
	//ฮปGtFNg
	class GreeningEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		D3DXVECTOR3* syncScale;
		GreeningEffect(D3DXVECTOR3* syncPosition,D3DXVECTOR3* syncScale) {
			this->syncPosition = syncPosition;
			this->syncScale = syncScale;
			effectNo = effekseerNS::GREENING;
		}
		virtual void update() {
			position = *syncPosition;
			scale = *syncScale;
			Instance::update();
		};
	};

	//อุปGtFNg
	class DeadingEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		D3DXVECTOR3* syncScale;
		DeadingEffect(D3DXVECTOR3* syncPosition,D3DXVECTOR3* syncScale) {
			this->syncPosition = syncPosition;
			this->syncScale = syncScale;
			effectNo = effekseerNS::DEADING;
		}
		virtual void update() {
			position = *syncPosition;
			scale = *syncScale;
			Instance::update();
		};
	};

	enum MODE
	{
		GREENING_MODE,
		DEAD_MODE,
	};
}



//===================================================================================================================================
//yฮปออNXFีหป่๐sคฝ฿ษ์ฌz
//===================================================================================================================================
class GreeningArea :public Object
{
private:

public:
	int playerNo;
	int mode;
	GreeningArea();
	~GreeningArea();
	void update(float frameTime);
	void render();	
};

