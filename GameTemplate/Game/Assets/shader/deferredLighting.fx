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
    float3 worldPos : TEXCOORD1; //���[���h���W
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
    float3 eyepos;                 //���_�̈ʒu
    float3 ambientColor;           //�A���r�G���g�J���[
}


Texture2D<float4> g_albedoTexture : register(t0); // �A���x�h
Texture2D<float4> g_normalTexture : register(t1); // �@��
Texture2D<float4> g_speculaTexture: register(t2); //�X�y�L�����}�b�v�ɃA�N�Z�X���邽�߂̕ϐ��B


sampler g_sampler : register(s0);

////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(PSIn psIn, float3 normal);
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv);




PSIn VSMain(VSInput vsIn)
{
    PSIn psIn;
    psIn.pos = mul(mvp, vsIn.pos);
    psIn.worldPos = psIn.pos.xyz;
    psIn.uv = vsIn.uv;
    return psIn;
}

float4 PSMain(PSIn In) : SV_Target0
{
	//GBuffer�̓��e���g���ă��C�e�B���O
    float4 albedo = g_albedoTexture.Sample(g_sampler, In.uv);
    
    float3 normal = g_normalTexture.Sample(g_sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;
    
    float3 dirLight = CalcLigFromDrectionLight(In, normal);
    
    float3 lig = dirLight + ambientColor;
	
    //�ŏI�I�ȐF
    float4 finalColor= albedo;
    
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
    float3 toEye = eyepos - worldPos;
    toEye = normalize(toEye);

	//���ʔ��˂̋��������߂�
    float t = dot(refVec, toEye);

	//���ʔ��˂̋�����0~1�ɂ���
    t = max(0.0f, t);

	//���ʔ��˂̋������i��
    t = pow(t, 10.0f);
    
    float specPower = g_speculaTexture.Sample(g_sampler,uv);
	//���ʔ��ˌ�
    float3 specularLig = lightColor * t * specPower;

    return specularLig;
}

//////////////////////////////////////////////////////////////////////////////////
//�f�B���N�V�������C�g���v�Z
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(PSIn psIn, float3 normal)
{
	//�g�U����
    float3 diffDirection = CalcLambertDiffuse(
		directionLight.direction, directionLight.color, normal);
	//���ʔ���
    float3 specDirection = CalcPhongSpecular(
		directionLight.direction, directionLight.color, psIn.worldPos, normal, psIn.uv);

	//�ŏI�I�Ȍ�
    return diffDirection + specDirection;
}