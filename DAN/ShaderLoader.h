//===================================================================================================================================
//�yShaderLoader.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/28
//===================================================================================================================================
#pragma once
#include "Base.h"

namespace shaderNS
{
	enum SHADER_NUMBER {
		TOON,
		INSTANCE_BILLBOARD,
		INSTANCE_STATIC_MESH,
		BLUR,
		MAX_SHADER,
	};

	//�O���Q�Ɗ֐�
	LPD3DXEFFECT* reference(int shaderNo);
}

class ShaderLoader :public Base
{
private:
	const char* shaderName[shaderNS::MAX_SHADER];
	LPD3DXEFFECT effect[shaderNS::MAX_SHADER];
public:
	ShaderLoader();
	~ShaderLoader();
	void load(LPDIRECT3DDEVICE9 device);
	void release();

	LPD3DXEFFECT* getEffect(int shaderNo) { return &effect[shaderNo]; }
};

