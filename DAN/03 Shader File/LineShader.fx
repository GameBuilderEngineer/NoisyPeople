//===================================================================================================================================
//【LineShader.fx】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/10/28
// [更新日]2019/10/28
//===================================================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4	matrixProjection;
float4x4	matrixView;
float4		color;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定義
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_OUT
{
	float4 position : POSITION;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(float4 position : POSITION)
{
	VS_OUT Out;

	Out.position = float4(
		position.x,
		position.y,
		position.z,
		1.0f);

	float4x4 matrixWorld = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	matrixWorld = mul(matrixWorld, matrixView);
	matrixWorld = mul(matrixWorld, matrixProjection);

	Out.position = mul(Out.position, matrixWorld);

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR0
{
	return color;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テクニック
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique main {
	pass p0 {
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
