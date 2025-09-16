/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[
 */

cbuffer cb : register(b0)
{
    float4x4 mvp;
    float4 mulColor;
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float2 uv : TEXCOORD0; //uv���W�B
    float4 worldPos : TEXCOORD1; //���[���h���W
};

//�f�B���N�V�������C�g�\����
struct DirectionLight
{
    float3 direction; //���C�g�̕���
    float3 color; //���C�g�̐F
};

//���C�g�̒萔�o�b�t�@�[
cbuffer LightCB : register(b1)
{
    DirectionLight directionLight; //�f�B���N�V�������C�g
    float3 eyepos; //���_�̈ʒu
    float3 ambientColor; //�A���r�G���g�J���[
    float4x4 mLVP; //���C�g�̃r���[�~�v���W�F�N�V�����s��
}


Texture2D<float4> g_albedoTexture : register(t0); // �A���x�h
Texture2D<float4> g_normalTexture : register(t1); // �@��
Texture2D<float4> g_speculaTexture : register(t2); //�X�y�L�����p���[
Texture2D<float4> g_worldPosTexture : register(t3); //���[���h���W
Texture2D<float4> g_shadowMap : register(t4); // �V���h�E�}�b�v(GBuffer�ł͂Ȃ�)



sampler g_sampler : register(s0);

////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(PSIn psIn, float3 normal, float3 worldPos);
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv);
float CalcShadowPow(float isDrawShadow, PSIn psIn, float3 worldPos);




PSIn VSMain(VSInput vsIn)
{
    PSIn psIn;
    psIn.worldPos = vsIn.pos;
    psIn.pos = mul(mvp, vsIn.pos);
    psIn.uv = vsIn.uv;
    return psIn;
}

float4 PSMain(PSIn psIn) : SV_Target0
{
	//GBuffer�̓��e���g���ă��C�e�B���O
    float4 albedo = g_albedoTexture.Sample(g_sampler, psIn.uv);
    
    float4 normal = g_normalTexture.Sample(g_sampler, psIn.uv);
    normal = (normal * 2.0f) - 1.0f;
    
    float3 worldPos = g_worldPosTexture.Sample(g_sampler, psIn.uv).xyz;
    
    float3 dirLight = CalcLigFromDrectionLight(psIn, normal.xyz, worldPos);
    
    float shadowPow = CalcShadowPow(normal.w, psIn, worldPos);
    
    float3 lig = dirLight + ambientColor;
	
    //�ŏI�I�ȐF
    float4 finalColor = albedo;
    
    finalColor.xyz *= lig;
    finalColor *= shadowPow;
    
    
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
    float3 toEye = eyepos - worldPos;
    toEye = normalize(toEye);

	//���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);

	//���ʔ��˂̋�����0~1�ɂ���
    t = max(0.0f, t);

	//���ʔ��˂̋������i��
    t = pow(t, 10.0f);
    
    float specPower = g_speculaTexture.Sample(g_sampler, uv);
	//���ʔ��ˌ�
    float3 specularLig = lightColor * t * specPower;

    return specularLig;
}

//////////////////////////////////////////////////////////////////////////////////
//�f�B���N�V�������C�g���v�Z
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(PSIn psIn, float3 normal, float3 worldPos)
{
	//�g�U����
    float3 diffDirection = CalcLambertDiffuse(
		directionLight.direction, directionLight.color, normal);
	//���ʔ���
    float3 specDirection = CalcPhongSpecular(
		directionLight.direction, directionLight.color, worldPos, normal, psIn.uv);

	//�ŏI�I�Ȍ�
    return diffDirection + specDirection;
}

//////////////////////////////////////////////////////////////////////////////////
//�e���v�Z
//////////////////////////////////////////////////////////////////////////////////

float CalcShadowPow(float isDrawShadow, PSIn psIn, float3 worldPos)
{
    float shadowPow = 1.0f;
    if (isDrawShadow == 1.0f)
    {
        //���C�g�r���[�X�N���[����Ԃ���UV���W��Ԃɕϊ�
        float4 posInLVP = mul(mLVP, float4(worldPos, 1.0f));
    
        float zInLVP = posInLVP.z / posInLVP.w;
        if (zInLVP >= 0.0f && zInLVP <= 1.0f)
        {

            float2 shadowUV = posInLVP.xy / posInLVP.w;
            shadowUV *= float2(0.5f, -0.5f);
            shadowUV += 0.5f;

	        //UV���W���g���ăV���h�E�}�b�v����e�����T���v�����O
            if (shadowUV.x >= 0.0f && shadowUV.x <= 1.0f &&
                shadowUV.y >= 0.0f && shadowUV.y <= 1.0f)
            {
                //�V���h�E�}�b�v����[�x���T���v�����O
                float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowUV).r;
                if (zInLVP > zInShadowMap)
                {
                    shadowPow = 0.5f; //�e�������Ă���B
                }
            }
        }
    }
    return shadowPow;
}
