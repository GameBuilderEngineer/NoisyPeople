///////////////////////////////////////////////////////////////////////////////////
//�O���[�o��
///////////////////////////////////////////////////////////////////////////////////
float4x4	matrixProjection;
float4x4	matrixView;
float4		lightDirection;
float3		eyePosition;		//���_����J�����ւ̃x�N�g��
float2		waveMove;			//�g�̈ړ��ʒu(UV)
float		height;				//�g�̍���
texture		textureDecal;
texture		normalMap;
//float4		diffuse;
//float		alphaValue;
//float4		lightDirection = float4(-1.0f, 1.0f, -1.0f, 0.2f);

sampler textureSampler = sampler_state
{
    Texture = <textureDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
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
	//float4 diffuse		: COLOR0;
	//float4 color			: COLOR1;
    //float3 lightDirection	: TEXCOORD1;//���C�g�̕����i�o�[�e�b�N�X�V�F�[�_�[�Ŕ��]���ĕۑ��j
    float3 eye				: TEXCOORD2;//���_����J�����ւ̃x�N�g���i�ڋ�ԓ��e�j
    float2 bumpUV			: TEXCOORD3;//�o���v�e�N�X�`��UV
};

///////////////////////////////////////////////////////////////////////////////////
//�o�[�e�b�N�X�E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position		: POSITION0,
	float3 normal		: NORMAL,
	float3 tangent		: TANGENT0,
	float2 localUV		: TEXCOORD0,
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

	//�@�����x�N�g�����v�Z
	//float3 eye = normalize(eyePos.xyz - pos.xyz);
	//�A�����x�N�g���𒸓_���W�n�ɕϊ�����
	//Out.eye.x = dot(eye, tangent);
	//Out.eye.y = dot(eye, binormal);
	//Out.eye.z = dot(eye, normal);
	//Out.eye = normalize(Out.eye);
	//�B���_���W->���C�g�̈ʒu�x�N�g��
	//float3 light = -lightDirection.xyz;
	//�C���C�g�x�N�g���𒸓_���W�n�ɕϊ�����
	//Out.light.x = dot(light, tangent);
	//Out.light.y = dot(light, binormal);
	//Out.light.z = dot(light, normal);
	//Out.light = normalize(Out.light);


	//�ڋ�Ԃ̍쐬
	float3 N = normal;			//�@��
	N.y += height;				//�g�̍�����ǉ�
	float3 T = tangent;			//�ڐ�
	float3 B = cross(N,T);		//�]�@��
	//float3 B = binormal;		//�]�@��

	//���_����J�����ւ̃x�N�g��
	float3 eye = eyePosition - position.xyz;	
	//�J�����ւ̃x�N�g����ڋ�Ԃ֓��e����
	Out.eye.x = dot(eye,T);
	Out.eye.y = dot(eye,B);
	Out.eye.z = dot(eye,N);
	//Out.eye = normalize(Out.eye);

	//���C�g�̌�����ۑ�
	//float3 L = -lightDirection.xyz;//���C�g�̌����𔽓]
	//���C�g�̕�����ڋ�Ԃ֓��e
	//Out.lightDirection.x = dot(L,T);
	//Out.lightDirection.y = dot(L,B);
	//Out.lightDirection.z = dot(L,N);
	//Out.lightDirection = normalize(Out.lightDirection);

	//�o���v�}�b�v�e�N�X�`��UV���W
	Out.bumpUV	=	localUV;
	Out.bumpUV	+=	waveMove;		//�g�̈ړ�

	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//�s�N�Z���E�V�F�[�_�[
///////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR
{   
	//�@�@���}�b�v���Q�Ƃ��A�@�����擾����
	//�@���}�b�v��0.0f�`1.0f�͈̔͂ɕۑ����Ă���̂�-1.0f�`1.0f�͈̔͂ɕϊ�����
	//float3 normal = 2.0f * tex2D(normalSampler,In.bumpUV).xyz -1.0f;
	//�A�t�H���V�F�[�f�B���O�ɂ��X�y�L�����[�̐F���v�Z����
	//�n�[�t�x�N�g���̌v�Z
	//float3 H = normalize(In.light + In.eye);
	//�B�X�y�L�����[�J���[���v�Z����
	//float3 S = pow(max(0.0f,dot(normal,H)),specular)* specularPower
	//�C��������
	//return tex2D(textureSampler, In.localUV)*max(ambient,dot(normal,In.light))+S;

	//float4 colorValue		= (float4)0;
	//float3 light			= (float3)0;
	//float3 reflectVector	= (float3)0;
	//float3 normalVector		= (float3)0;
	//float3 sightVector		= (float3)0;

	//colorValue = float4(0.2, 0.2, 0.7, 1);							//��

	//light = lightDirection.xyz;

	//normalVector = tex2D(normalSampler, In.bumpUV).xyz;
	//normalVector.y = (1 - normalVector.y) * height;
	//normalVector = normalVector * 2 - 1;
	//normal = normalVector;
	//normal			= tex2D( normalSampler, In.bumpUV).xyz;	//�o���v�e�N�X�`���̓ǂݎ��
	//normal.y		= (1-normal.y)*height;					//�g�̍����̐ݒ�
	//normal			= normal*2-1;							//-1.0f�`1.0f�͈̔͂ɕϊ�

	//���邳(�f�B�t���[�Y��)
	//float bright = saturate(dot(light, normal));

	//�X�y�L�����̌v�Z
	//float3 H = normalize(In.eye + light);		//�n�[�t�x�N�g��
	//float specular = max(0, dot(H, normal));	//���ˌ�
	//specular = pow(specular, 12);



	//���˃x�N�g�����Z�o����
	//reflect�֐�:���˃x�N�g��v��Ԃ� 
	//�� i:���˃x�N�g��,n:�T�[�t�F�X�@�� reflect(i,n){ v = i - 2*dot(i,n)*n }
	//reflectVector	= reflect(-normalize(In.eye), normalVector);
	
	//�����x�N�g��
	//sightVector = In.eye - eyePosition;

	//�F�̉��Z
	//���ˋ��x
	//float4 intensity = dot(light, normal);							//�f�B�t���[�Y��
	//float4 intensity;// = bright;							//�f�B�t���[�Y��
	//					+ pow(dot(reflectVector,sightVector),12);	// �X�y�L�����[��(�Ɩ����f���́@�t�H�� phong)
	//float4 intensity = dot(light, normalVector);
	//	+ pow(dot(reflectVector, sightVector), 12);
	//intensity += colorValue;
	//return intensity;
	//intensity = color * bright;
	//intensity.xyz += specular;

	//return intensity; 

	//float4 texel = tex2D(normalSampler, In.bumpUV);
	float4 texel = tex2D(textureSampler, In.bumpUV);
	float4 finalColor = texel;// *In.diffuse;
	return finalColor;

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