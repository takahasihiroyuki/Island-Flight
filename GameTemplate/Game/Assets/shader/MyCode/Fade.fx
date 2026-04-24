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

cbuffer Fade : register(b1)
{
    float3 FadeCollar;
    float pad;
    float fade; //現在のフェード量（0～1）

};

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

sampler Sampler : register(s0);


float4 PSMain(PSInput In) : SV_Target0
{
    float4 finalCollar;
    finalCollar.rgb = FadeCollar;
    finalCollar.a = fade;
    
    
    return finalCollar;

}
