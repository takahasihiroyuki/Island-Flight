/*!
 * @brief	�V���v���ȃ��f���V�F�[�_�[�B
 */

////////////////////////////////////////////////
// �\����
////////////////////////////////////////////////
//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn
{
    float4 pos : POSITION;                  //���f���̒��_���W�B
    float2 uv : TEXCOORD0;                  //UV���W�B
    SSkinVSIn skinVert;                     //�X�L���p�̃f�[�^�B
    float3 normal : NORMAL;                 //�@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn
{
    float4 pos : SV_POSITION;               //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float2 uv : TEXCOORD0;                  //uv���W�B
    float3 worldPos : TEXCOORD1;            //���[���h���W
    float3 normal : NORMAL;                 //�@��
    float3 tangent : TANGENT;               // �ڃx�N�g��
    float3 biNormal : BINORMAL;             // �]�x�N�g��
    
    //���s�N�Z���V�F�[�_�[�ł͎g��Ȃ��B
    float clip0 : SV_ClipDistance0;         //�C�ӂ̕��ʂŃN���b�v���邽�߂̒l�B
};

//�f�B���N�V�������C�g�\����
struct DirectionLight
{
    float3 direction; //���C�g�̕���
    float3 color; //���C�g�̐F
};

struct Light
{
    DirectionLight directionLight; //�f�B���N�V�������C�g
    float3 eyepos; //���_�̈ʒu
    float3 ambientColor; //�A���r�G���g�J���[
    float4x4 mLVP; //���C�g�̃r���[�~�v���W�F�N�V�����s��
};

////////////////////////////////////////////////
// �萔�o�b�t�@�B
////////////////////////////////////////////////
//���f���p�̒萔�o�b�t�@
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//���C�g�̒萔�o�b�t�@�[
cbuffer LightCB : register(b1)
{
    float4 reflectionPlane; //���˕��ʂ̕������B
    Light light;
}

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B
sampler g_sampler : register(s0); //�T���v���X�e�[�g�B
Texture2D<float4> g_normalMap : register(t1); //�@���}�b�v�ɃA�N�Z�X���邽�߂̕ϐ��B
Texture2D<float4> g_specularMap : register(t2); //�X�y�L�����}�b�v�ɃA�N�Z�X���邽�߂̕ϐ��B


////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(SPSIn psIn, float3 normal);
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv);

/// <summary>
//�X�L���s����v�Z����B
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
    float4x4 skinning = 0;
    float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// ���_�V�F�[�_�[�̃R�A�֐��B
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
    SPSIn psIn;
    
    float4x4 m;
    if (hasSkin)
    {
        m = CalcSkinMatrix(vsIn.skinVert);
    }
    else
    {
        m = mWorld;
    }
    psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    
    float4 posVS = psIn.pos;
    
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = mul(m, vsIn.normal);
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));

    psIn.uv = vsIn.uv;

    //���ς����Ȃ�N���b�v����B
    psIn.clip0 = dot(reflectionPlane, posVS);
    
    return psIn;
}

/// <summary>
/// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
/// <summary>
/// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}
/// <summary>
/// �s�N�Z���V�F�[�_�[�̃G���g���[�֐��B
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float3 ligDirection = light.directionLight.direction;
    
    //�A���x�h�e�N�X�`���̃T���v�����O
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //�@���}�b�v�̃T���v�����O
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    //0�`1�͈̔͂�-1�`1�͈̔͂ɂ���B
    localNormal = (localNormal - 0.5f) * 2.0f;
    //�ڃx�N�g����Ԃ��烏�[���h��Ԃɕϊ�����
    float3 normal = psIn.normal;
    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;


    
    //���C�g�̌v�Z
    float3 directionLight = CalcLigFromDrectionLight(psIn, normal);
    float3 lig = directionLight + light.ambientColor;
    
    //�ŏI�I�ȐF
    float4 finalColor = albedoColor;
    finalColor.xyz *= lig;
    
    return finalColor;
}

float4 PSSkyCubeMain(SPSIn psIn) : SV_Target0
{
    float3 ligDirection = light.directionLight.direction;
    
    //�A���x�h�e�N�X�`���̃T���v�����O
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //�@���}�b�v�̃T���v�����O
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    //0�`1�͈̔͂�-1�`1�͈̔͂ɂ���B
    localNormal = (localNormal - 0.5f) * 2.0f;
    //�ڃx�N�g����Ԃ��烏�[���h��Ԃɕϊ�����
    float3 normal = psIn.normal;
    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;


    
    //���C�g�̌v�Z
    float3 directionLight = CalcLigFromDrectionLight(psIn, normal);
    float3 lig = directionLight + light.ambientColor;
    
    //�ŏI�I�ȐF
    float4 finalColor = albedoColor;
    finalColor.xyz *= lig;
    
    return finalColor;
}

//////////////////////////////////////////////////////////////////////////////////
//Lambert�g�U���˂��v�Z
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//�s�N�Z���̖@���ƃ��C�g�̕����̓��ς��v�Z���A���C�g�̉e���x�����߂�
    float t = dot(normal, lightDirection);
	//���ς̌��ʂ�-1��������
    t *= -1.0f;
	//���ς̌��ʂ�0�ȉ��Ȃ�0�ɂ���
    if (t < 0.0f)
    {
        t = 0.0f;
    }
	//���C�g�̉e���x��Ԃ�
    return lightColor * t;
}

//////////////////////////////////////////////////////////////////////////////////
//phong���ʔ��˂��v�Z
//////////////////////////////////////////////////////////////////////////////////
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv)
{
	//���˃x�N�g�������߂�
    float3 refVec = reflect(lightDirection, normal);
	//�������������T�[�t�F�C�X(�\��)���王�_�ɐL�т�x�N�g�������߂�
    float3 toEye = light.eyepos - worldPos;
    toEye = normalize(toEye);

	//���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);

	//���ʔ��˂̋�����0~1�ɂ���
    t = max(0.0f, t);

	//���ʔ��˂̋������i��
    t = pow(t, 10.0f);

    //�X�y�L�����}�b�v����X�y�L�������˂̋������T���v�����O
    float specPower = g_specularMap.Sample(g_sampler, uv).r;
    
	//���ʔ��ˌ�
    float3 specularLig = lightColor * t * specPower;

    return specularLig;
}

//////////////////////////////////////////////////////////////////////////////////
//�f�B���N�V�������C�g���v�Z
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(SPSIn psIn, float3 normal)
{
	//�g�U����
    float3 diffDirection = CalcLambertDiffuse(
		light.directionLight.direction, light.directionLight.color, normal);
	//���ʔ���
    float3 specDirection = CalcPhongSpecular(
		light.directionLight.direction, light.directionLight.color, psIn.worldPos, normal, psIn.uv);

	//�ŏI�I�Ȍ�
    return diffDirection + specDirection;
}
