//�X�L�j���O�p�̒��_�f�[�^���ЂƂ܂Ƃ߁B
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};

//���f���p�̒萔�o�b�t�@
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
}

//���_�V�F�[�_�[�̓���
struct SVSIn
{
    float4 pos : POSITION; //���f���̒��_���W�B
    float2 uv : TEXCOORD0; // UV���W
    SSkinVSIn skinVert; //�X�L���p�̃f�[�^�B
};

//�s�N�Z���V�F�[�_�[�̓���
struct SPSIn
{
    float4 pos : SV_POSITION; //�X�N���[����Ԃł̃s�N�Z���̍��W�B
    float2 uv : TEXCOORD0; // UV���W
};

//�V���h�E�}�b�v�̒萔�o�b�t�@
cbuffer ShadowMapCb : register(b1)
{
    bool isTransparent; //�����x
}

StructuredBuffer<float4x4> g_boneMatrix : register(t3); //�{�[���s��

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

// ���_�V�F�[�_�[�B
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
    psIn.pos = mul(mView, psIn.pos);
    psIn.pos = mul(mProj, psIn.pos);
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

float4 PSShadowCaster(SPSIn psIn) : SV_Target0
{
    //���f���������Ȃ�e��`�悵�Ȃ�
    if (!isTransparent)
    {
        return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 0.0f);
    }
    
    return float4(psIn.pos.z, psIn.pos.z, psIn.pos.z, 1.0f);
}