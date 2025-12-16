cbuffer cb : register(b0)
{
    float4x4 mvp; // MVP行列
    float4 mulColor; // 乗算カラー
};

struct VSInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

cbuffer FogCB : register(b1)
{
    float3 fogCollar;
    float pad;
    float fogDistanceScale;
};

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

Texture2D<float4> depthTexture : register(t0); // 深度テクスチャ

sampler Sampler : register(s0);


float4 PSMain(PSInput In) : SV_Target0
{
    float depth = depthTexture.Sample(Sampler, In.uv).y;
    float4 fog = float4(fogCollar.xyz, 0);
    float fogFactor = 1.0 - exp(-depth * fogDistanceScale);
    
    fog.a = fogFactor;
    
    return fog;

}
