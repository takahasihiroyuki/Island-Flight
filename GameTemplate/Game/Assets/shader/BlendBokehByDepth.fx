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

PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

Texture2D<float4> bokeTexture : register(t0); // ボケ画像
Texture2D<float4> depthTexture : register(t1); // 深度テクスチャ

sampler Sampler : register(s0);


float4 PSMain(PSInput In) : SV_Target0
{
    // カメラ空間での深度値をサンプリング。
    float depth = depthTexture.Sample(Sampler, In.uv).y;
    // カメラ空間での深度値が200以下ならピクセルキル 
    //      -> ボケ画像を描きこまない。
    //clip(depth - 200.0f);
    // ボケ画像をサンプリング。
    float4 boke = bokeTexture.Sample(Sampler, In.uv);
    // 深度値から不透明度を計算する。
    // 深度値200からボケが始まり、深度値500で最大のボケ具合になる。
    //  -> つまり、深度値500で不透明度が1になる。
    //boke.a = min(1.0f, (depth - 200.0f) / 500.0f);
    float t = saturate(depth);
    boke.a = t * t * (3.0f - 2.0f * t); // Hermite補間
    // ボケ画像を出力。
    return boke;

}
