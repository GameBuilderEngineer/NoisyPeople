///////////////////////////////////////////////////////////////////////////////////
//�O���[�o��
///////////////////////////////////////////////////////////////////////////////////
//float4x4 mWorld: World;
//float4x4 mWorldViewProj : WorldViewProjection;	
float3 lightPosition = {100.0f, 100.0f, -100.0f};	//���C�g�ʒu
//float4 vLightDir;
//float4 vColor;
float3 eyePosition;		//���_����J�����ւ̃x�N�g��
float2 waveMove;		//�g�̈ړ��ʒu(UV)
float height;			//�g�̍���
//texture DecaleTex;
texture normalMap;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4	matrixProjection;
float4x4	matrixView;
texture		textureDecal;
float4		diffuse;
float		alphaValue;
float4		lightDirection = float4(-1.0f, 1.0f, -1.0f, 0.2f);

//�P�ʍs��
#define IDENTITY_MATRIX float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)

//�t�s��֐�
float4x4 inverse(float4x4 m) {
	float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
	float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
	float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
	float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

	float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
	float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
	float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
	float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

	float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
	float idet = 1.0f / det;

	float4x4 ret;

	ret[0][0] = t11 * idet;
	ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
	ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
	ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

	ret[1][0] = t12 * idet;
	ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
	ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
	ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

	ret[2][0] = t13 * idet;
	ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
	ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
	ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

	ret[3][0] = t14 * idet;
	ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
	ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
	ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

	return ret;
}

sampler textureSampler = sampler_state
{
    Texture = <textureDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};
sampler normalSampler = sampler_state
{
    Texture = <normalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

///////////////////////////////////////////////////////////////////////////////////
//�^��`
///////////////////////////////////////////////////////////////////////////////////
struct VS_OUT
{
    float4 position			: POSITION;
	float2 uv				: TEXCOORD0;
	//float4 diffuse			: COLOR0;
	float4 color			: COLOR1;
    //float4 Color: COLOR0;    
    //float2 Tex: TEXCOORD0;
    float3 lightDirection	: TEXCOORD1;//���C�g�̕����i�o�[�e�b�N�X�V�F�[�_�[�Ŕ��]���ĕۑ��j
    float3 toEye			: TEXCOORD2;//���_����J�����ւ̃x�N�g���i�ڋ�ԓ��e�j
    float2 bumpUV			: TEXCOORD3;//�o���v�e�N�X�`��UV
    float3 lightVector		: TEXCOORD4;//���_���烉�C�g�܂ł̃x�N�g��
    float3 tangentNormal	: TEXCOORD5;//�ڋ�Ԗ@��
    float3 tangentView		: TEXCOORD6;//�ڋ�ԋt�r���[
};

///////////////////////////////////////////////////////////////////////////////////
//�o�[�e�b�N�X�E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position		: POSITION0,
	float2 localUV		: TEXCOORD0,
	float3 normal		: NORMAL0,
	float3 tangent		: TANGENT0,
	float4 matrix1		: TEXCOORD1,
	float4 matrix2		: TEXCOORD2,
	float4 matrix3		: TEXCOORD3,
	float4 matrix4		: TEXCOORD4
)
{
	VS_OUT Out=(VS_OUT)0;

	//���_��ۑ�
	Out.position = position;

	//���[���h�s���p�ӂ���B
	float4x4 worldMatrix = float4x4(
		matrix1,
		matrix2,
		matrix3,
		matrix4);

	//*�r���[�s��
	worldMatrix = mul(worldMatrix, matrixView);
	//*�v���W�F�N�V�����s��
	worldMatrix = mul(worldMatrix, matrixProjection);
	//*���[���h�s��
	Out.position = mul(Out.position, worldMatrix);

	//�t���[���h�s��
	float4x4 inverseWorld = inverse(worldMatrix);

	//UV���W
	Out.uv = localUV;

	//�A���r�G���g�J���[
	//float ambient = lightDirection.w;

	//�����o�[�g���Z�J���[
	//float4 lambert = max(ambient, saturate(dot(normal, lightDirection)));
	//Out.diffuse = lambert * diffuse;
	//Out.color = color;
	//Out.Color = vColor;

	//Out.Tex = Texcoord;

	normal.y += height;			//�g�̍�����ݒ�

	//�ڋ�Ԃ̍쐬
	float3 N = normal;			//�@��
	float3 T = tangent;			//�ڐ�
	float3 B = cross(N,T);		//�]�@��

	//���_����J�����ւ̃x�N�g��
	float3 toEye = eyePosition - position.xyz;	
	//�J�����ւ̃x�N�g����ڋ�Ԃ֓��e����
	Out.toEye.x = dot(toEye,T);
	Out.toEye.y = dot(toEye,B);
	Out.toEye.z = dot(toEye,N);

	//���C�g�̌�����ۑ�
	float3 L = -mul( lightDirection, inverseWorld).xyz;//���C�g�̌����𔽓]
	//���C�g�̕�����ڋ�Ԃ֓��e
	Out.lightDirection.x = dot(L,T);
	Out.lightDirection.y = dot(L,B);
	Out.lightDirection.z = dot(L,N);

	//�o���v�}�b�v�e�N�X�`��UV���W
	Out.bumpUV	=	localUV;	
	Out.bumpUV	+=	waveMove;		//�g�̈ړ�

	//���[���h�s��i�]�u/�t�j
	float4x4 WIT;	// world inverse transpose
	WIT = inverseWorld;
	WIT = transpose(WIT);

	//�ڋ�Ԗ@��
	float3 tangentNormal = normalize(normal);
	Out.tangentNormal = mul(tangentNormal, WIT).xyz;//���[���h��Ԃ֕␳����

	//���_���烉�C�g�܂ł̃x�N�g��
	float4 f4Position		= float4(position.xyz,1);
    float3 worldPosition	= mul(f4Position, worldMatrix).xyz;//���[���h��ԏ�̒��_�ʒu
    Out.lightVector			= normalize(lightPosition - worldPosition);
	
	//���_���[���h�ʒu����r���[�s��ʒu�ւ̃x�N�g���i�����I�ɂ͒��_�ʒu����J�����ʒu�ւ̃x�N�g���j
	//���T���v���ɏ����Ă������̂ō��͂��̂܂܏��������AtoEye�Ɠ����C������B
	float4x4 viewInverse = inverse(matrixView);//�r���[�s��i�t�j
	Out.tangentView = normalize(viewInverse[3].xyz - worldPosition);
	
	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//�s�N�Z���E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR
{   
	float4 color			= (float4)0;
	float3 light			= (float3)0;
	float3 reflectVector	= (float3)0;
	float3 normal			= (float3)0;
	float3 sightVector		= (float3)0;

	light = In.lightDirection.xyz;

	normal			= tex2D( normalSampler, In.bumpUV).xyz;	//�o���v�e�N�X�`���̓ǂݎ��
	normal.y		= (1-normal.y)*height;					//�g�̍����̐ݒ�
	normal			= normal*2-1;							//

	//���˃x�N�g�����Z�o����
	reflectVector	= reflect(-normalize(In.toEye), normal);//reflect�֐�:���˃x�N�g��v��Ԃ� �� i:���˃x�N�g��,n:�T�[�t�F�X�@�� reflect(i,n){ v = i - 2*dot(i,n)*n }
	
	//�����x�N�g��
	sightVector = In.toEye - eyePosition;

	//�F�̉��Z
	//���ˋ��x
	float4 intensity =	dot(light,normal)							//�f�B�t���[�Y��
						+ pow(dot(reflectVector,sightVector),12);	// �X�y�L�����[��(�Ɩ����f���́@�t�H�� phong)
	color = float4(0.2,0.2,0.7,1);							//��
	intensity += color;

	return intensity; 

	//float4 texel = tex2D(textureSampler, In.uv);
	//float4 finalColor = texel * In.diffuse;
	//return finalColor;

}
///////////////////////////////////////////////////////////////////////////////////
// �e�N�j�b�N
///////////////////////////////////////////////////////////////////////////////////
technique tecOcean
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();   
    }
}