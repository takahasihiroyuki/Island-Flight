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
    float4 pos : POSITION; //���f���̒��_���W�B
    float2 uv : TEXCOORD0; //UV���W�B
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
    float3 normal : NORMAL; //�@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;

};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float2 uv : TEXCOORD0; //uv���W�B
    float3 worldPos : TEXCOORD1; //���[���h���W
    float3 normal : NORMAL; //�@��
    float3 tangent : TANGENT; // �ڃx�N�g��
    float3 biNormal : BINORMAL; // �]�x�N�g��
    float4 posRefCamViewProj : TEXCOORD3; // ���˗p�J�����̃r���[���e�s������������W
    float4 refClip : TEXCOORDn;
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
    float3 cameraEyePos; //�J�����̍��W
    float3 ambientColor; //�A���r�G���g�J���[
    float4x4 mLVP; //���C�g�r���[���e�s��B
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

//�C�p�̒萔�o�b�t�@�[
cbuffer LightCb : register(b1)
{
    float4x4 ReflectionCameraVP; // ���˗p�J�����r���[���e�s��
    Light light;
    //���˂̊����̉����l�A�K�����̒l�ȏ�͔��˂���B�i�^�ォ�猩�����˗��j
    float baseReflectance; // ��{���˗�

}

////////////////////////////////////////////////
// �O���[�o���ϐ��B
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B
sampler g_sampler : register(s0); //�T���v���X�e�[�g�B
Texture2D<float4> g_normalMap : register(t1); //�@���}�b�v�ɃA�N�Z�X���邽�߂̕ϐ��B
Texture2D<float4> g_specularMap : register(t2); //�X�y�L�����}�b�v�ɃA�N�Z�X���邽�߂̕ϐ��B
Texture2D<float4> g_refLect : register(t10); // ���˃}�b�v


////////////////////////////////////////////////
// �֐���`�B
////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(SPSIn psIn, float3 normal);
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv);
float2 CalcReflectUV(float4 clip);
float ComputeFresnel(float3 normal, float3 viewDir, float baseReflectance);

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
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;

    float4 worldPos = mul(mWorld, vsIn.pos);
    psIn.worldPos = worldPos;
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, worldPos);
    psIn.pos = mul(mProj, psIn.pos);
    
    psIn.uv = vsIn.uv;
   
    psIn.normal = mul(mWorld, vsIn.normal);
    
    //�ڃx�N�g���Ə]�x�N�g�������[���h��Ԃɕϊ�����
    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    
    //�N���b�v���W��p�ӂ��Ă����B
    psIn.refClip = mul(ReflectionCameraVP, float4(psIn.worldPos.xyz, 1.0));
    
    return psIn;
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

    
    float2 uvR = CalcReflectUV(psIn.refClip);
    uvR = saturate(uvR);
    float4 reflect = g_refLect.Sample(g_sampler, uvR);


    
    //���C�g�̌v�Z
    float3 directionLight = CalcLigFromDrectionLight(psIn, normal);
    float3 lig = directionLight;
    
    //�ŏI�I�ȐF
    float4 finalColor = albedoColor;
    albedoColor = float4(0.20f,0.35f,0.47f,0.0f);
    finalColor.xyz *= directionLight;
    //finalColor.xyz = float3(1.0f, 0.0f, 1.0f); //�����𑫂�
    finalColor = reflect;
    
    //�t���l�����˗����v�Z
    float flesnel = ComputeFresnel(normal, normalize(light.cameraEyePos - psIn.worldPos), baseReflectance);
    
    finalColor = lerp(albedoColor, reflect, flesnel);
    
    
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
    float3 toEye = light.cameraEyePos - worldPos;
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


float2 CalcReflectUV(float4 clip)
{
    //���ꂩ�珜�Z����̂ŁA0�����h�~���邽�߂ɐ�Βl�̍ő�l��1e-6�Ő�������B
    float w = max(abs(clip.w), 1e-6);
    //rcp��1/x���v�Z����֐��B(���ʂɂ����葁��)
    float invW = rcp(w);
    float2 uv = clip.xy * invW * 0.5f + 0.5f; // NDC��[0,1]
    uv.y = 1.0f - uv.y;
    return uv;
}

float ComputeFresnel(float3 normal, float3 viewDir, float baseReflectance)
{
    
    float cosTheta = saturate(dot(normalize(normal), -normalize(viewDir)));
    
    //�p�x�ɂ�蔽�˗��̌W���B
    float angleFactor = pow(1.0f - cosTheta, 5.0f);
    //angleFactor�̊����̏���B
    float remainingReflectance = 1 - baseReflectance;

    //�t���l�����˗�
    return baseReflectance + remainingReflectance * angleFactor;
}