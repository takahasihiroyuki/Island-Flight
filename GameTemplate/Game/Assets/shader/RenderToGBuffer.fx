// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices   : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos      : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv       : TEXCOORD0;
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B

};

//�s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos      : SV_POSITION; //���W�B
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv       : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
};

// �s�N�Z���V�F�[�_�[����̏o��
struct SPSOut
{
    float4 albedo   : SV_Target0; // �A���x�h
    float4 normal   : SV_Target1; // �@��
    float specPow   : SV_Target2; // �X�y�L�������x
    float3 worldPos : SV_Target3; // ���[���h���W
};


//�V�F�[�_�[���\�[�X
Texture2D<float4> g_albedo : register(t0); //�A���x�h�}�b�v
Texture2D<float4> g_normalMap : register(t1); //�@���}�b�v�ɃA�N�Z�X���邽�߂̕ϐ��B
Texture2D<float4> g_specularMap : register(t2); //�X�y�L�����}�b�v�ɃA�N�Z�X���邽�߂̕ϐ��B
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��B

//�T���v���[�X�e�[�g
sampler g_sampler : register(s0);

float3 CalcNormal(SPSIn psIn);

//�X�L���s����v�Z����B
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

//���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
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

    psIn.pos = mul(m, vsIn.pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.normal = mul(m, vsIn.normal);
    psIn.tangent = normalize(mul(m, vsIn.tangent));
    psIn.biNormal = normalize(mul(m, vsIn.biNormal));

    psIn.uv = vsIn.uv;
    
    return psIn;
}

// �X�L���Ȃ����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, false);
}
// �X�L�����胁�b�V���̒��_�V�F�[�_�[�̃G���g���[�֐��B
SPSIn VSSkinMain(SVSIn vsIn)
{
    return VSMainCore(vsIn, true);
}

//���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
SPSOut PSMain(SPSIn psIn,bool isShadowReciever)
{
    //GBuffer�ɏo��
    SPSOut psOut;
    
    //�A���x�h�J���[�̒��o
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
    clip(psOut.albedo.a - 0.2f); // �s�N�Z���L��
    psOut.albedo.w = psIn.pos.z;

        
    psOut.normal.xyz = CalcNormal(psIn);
    
    psOut.specPow = g_specularMap.Sample(g_sampler, psIn.uv); //�X�y�L�������x�͂Ƃ肠����1.0f�ŌŒ�B
    
    //���[���h���W�̒��o
    psOut.worldPos = psIn.worldPos;
    
    // �V���h�E���V�[�o�[���ǂ����𔻒肷��t���O��w�����Ɋi�[����B
    //�@���}�b�v�̂��͎g��Ȃ��̂ŁA�����Ɋi�[����B
    if (isShadowReciever == true)
    {
        psOut.normal.w = 1.0f;
    }
    else
    {
        psOut.normal.w = 0.0f;
    }
    
    return psOut;
}


SPSOut PSMainShadowReciever(SPSIn psIn) : SV_Target0
{
    return PSMain(psIn, true);
}

SPSOut PSNormalMain(SPSIn psIn) : SV_Target0
{
    return PSMain(psIn, false);
}

//////�֐�/////////////////////////////////////////////////////////////////////

// �@���}�b�v����@�����v�Z����֐�
float3 CalcNormal(SPSIn psIn)
{
    // �@���}�b�v����^���W�F���g�X�y�[�X�̖@�����T���v�����O����
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    localNormal = (localNormal - 0.5f) * 2.0f;
    
    // �^���W�F���g�X�y�[�X�̖@�������[���h�X�y�[�X�ɕϊ�����
    float3 normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + psIn.normal * localNormal.z;
    
    // �o�͂�0�`1�Ɋۂ߂��Ă��܂��}�C�i�X�̒l�������Ă��܂��̂�-1�`1��0�`1�ɕϊ�����
    normal = (normal / 2.0f) + 0.5f;
   
    normal=normalize(normal);

    return normal;
}
