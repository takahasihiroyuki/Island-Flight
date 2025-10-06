/*!
 * @brief	�X�J�C�L���[�u�p�̃V�F�[�_�[�B
 */

cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// ���_�V�F�[�_�[�̓���
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

//�s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; //���W
    float3 normal : NORMAL; //�@��
    float3 tangent : TANGENT; //�ڃx�N�g��
    float3 biNormal : BINORMAL; //�]�x�N�g��
    float2 uv : TEXCOORD0; //UV���W
    float3 worldPos : TEXCOORD1; //���[���h���W
    
    //���˃I�u�W�F�N�g�̏ꍇ�Ɏg���B
    float clip : SV_ClipDistance0; //�N���b�v���� 
};
cbuffer SkyCubeCb : register(b1)
{
    float luminance; // �P�x
    float4 reflectionPlane; //���˕��ʂ̕������B
};

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
Texture2D<float4> g_normal : register(t1); //�m�[�}���}�b�v
Texture2D<float4> g_spacular : register(t2); //�X�y�L�����}�b�v
TextureCube<float4> g_skyCubeMap : register(t10);
sampler g_sampler : register(s0);

SPSIn VSMainCore(SVSIn vsIn, bool isReflectionModel)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.normal = mul(mWorld, vsIn.normal);
    psIn.normal = normalize(psIn.normal);
    psIn.pos = mul(mView, psIn.pos);
    float4 posVS = psIn.pos;
    psIn.pos = mul(mProj, psIn.pos);

    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    
    psIn.uv = vsIn.uv;
    
    if (isReflectionModel)
    {
    //���ς����Ȃ�N���b�v����B
        psIn.clip = dot(reflectionPlane, posVS);
    }
    else
    {
        psIn.clip = 1.0f; //�N���b�v���Ȃ��B
    }

    
    return psIn;
    
}

SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}

SPSIn VSReflectionMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 albedoColor;
   
    float3 normal = normalize(psIn.normal);
    albedoColor = g_skyCubeMap.Sample(g_sampler, normal * -1.0f) * luminance;
    float4 finalColor = albedoColor * luminance;
    return finalColor;
}