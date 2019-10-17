//===================================================================================================================================
//�yInstancingStaticMesh.fx�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/23
// [�X�V��]2019/10/17
//===================================================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�O���[�o��
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4		matrixProjection;
float4x4		matrixView;
texture		textureDecal;
float4		diffuse;
float4		ambient				= float4(0.1f, 0.1f, 0.1f, 0.1f);
float3		lightDirection		= float3(1.0f, 1.0f, 1.0f);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//��`
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sampler textureSampler = sampler_state
{
	texture				= <textureDecal>;
	MinFilter			= ANISOTROPIC;
	MagFilter			= POINT;
	MipFilter			= POINT;
	MaxAnisotropy	= 4;
	AddressU			= Wrap;
	AddressV			= Wrap;
};

struct VS_OUT
{
	float4 position				: POSITION;
	float2 uv						: TEXCOORD0;
	float3 normal					: NORMAL;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�o�[�e�b�N�X�E�V�F�[�_�[
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position	: POSITION,
	float2 localUV		: TEXCOORD0,
	float3 normal		: NORMAL,
	float3 pos			: TEXCOORD1)
{
	VS_OUT Out;

	//���_��ۑ�
	Out.position = float4(
		position.x,
		position.y,
		position.z,
		1.0f);

	//���[���h�s���p�ӂ���B�i���݂͈ړ����̂݁j
	float4x4 worldMatrix = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f);

	//���ƂŎ����\��
	//�v�Z��S��GPU���ōs�������������H�H
	//�ړ��s��̓K�p
	//��]�s��̓K�p
	//�X�P�[���s��̓K�p

	//�r���[�s��
	worldMatrix = mul(worldMatrix, matrixView);
	//�v���W�F�N�V�����s��
	worldMatrix = mul(worldMatrix, matrixProjection);

	Out.position = mul(Out.position, worldMatrix);
	
	Out.uv = localUV;

	Out.normal = normal;

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�s�N�Z���E�V�F�[�_�[
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR0
{
	float4 lambert		= saturate(dot(In.normal, lightDirection));
	float4 texel			= tex2D(textureSampler, In.uv);
	float4 finalColor		= texel*lambert*0.5f + ambient*texel + diffuse*lambert + diffuse*ambient;

	return texel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �e�N�j�b�N
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique mainTechnique {
	pass p0 {
		//�X�e�[�g�ݒ�
		Zenable							= TRUE;			//Z�o�b�t�@�L��
		ZWriteEnable					= TRUE;			//Z�o�b�t�@�ւ̏������ݗL��
		//ShadeMode					= GOURAUD;		//�O�[���[�E�V�F�[�f�B���O
		CullMode						= CCW;				//�w�ʂ��J�����O
		//MultiSampleAntialias		= TRUE;			//�A���`�G�C���A�V���O��L��

		//�V�F�[�_�ݒ�
		VertexShader		= compile vs_3_0 VS();
		PixelShader		= compile ps_3_0 PS();
	}
}
