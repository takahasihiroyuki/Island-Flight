

// ブラーをかけるテクスチャの幅
static const float BLUR_TEX_W = 1280.0f;

// ブラーをかけるテクスチャの高さ
static const float BLUR_TEX_H = 720.0f;

// ブラー半径。この数値を大きくすると六角形ボケが大きくなる
static const float BLUR_RADIUS = 8.0f;


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

struct PSVerticalDiagonalBlurOutput
{
    float4 color_0 : SV_Target0; // 垂直ブラーの出力先
    float4 color_1 : SV_Target1; // 斜めブラーの出力先
};

/*!
 * @brief 頂点シェーダー
 */
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

Texture2D<float4> mainRenderTargetTexture : register(t0); // メインレンダリングターゲットのテクスチャ

sampler g_sampler : register(s0);


/////////////////////////////////////////////////////////
// 輝度抽出用
/////////////////////////////////////////////////////////

//輝度抽出用のピクセルシェーダー。
float4 PSSamplingLuminance(PSInput In) : SV_Target0
{
    // メインレンダリングターゲットからカラーをサンプリング
    float4 color = mainRenderTargetTexture.Sample(g_sampler, In.uv);

    // サンプリングしたカラーの明るさを計算
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));

   //カラーの明るさが1以下ならピクセルキルをする
    clip(t - 1.0f);

    return color;
}

/////////////////////////////////////////////////////////
// ボケ画像の平均出力用
/////////////////////////////////////////////////////////


// ボケ画像にアクセスするための変数を追加
Texture2D<float4> g_bokeTexture_0 : register(t0);
Texture2D<float4> g_bokeTexture_1 : register(t1);
Texture2D<float4> g_bokeTexture_2 : register(t2);
Texture2D<float4> g_bokeTexture_3 : register(t3);

//ボケ画像の平均出力用のピクセルシェーダー

float4 PSBloomFinal(PSInput In) : SV_Target0
{
    // ボケ画像をサンプリングして、平均をとって出力する
    float4 combineColor = g_bokeTexture_0.Sample(g_sampler, In.uv);
    combineColor += g_bokeTexture_1.Sample(g_sampler, In.uv);
    combineColor += g_bokeTexture_2.Sample(g_sampler, In.uv);
    combineColor += g_bokeTexture_3.Sample(g_sampler, In.uv);
    combineColor /= 4.0f;
    combineColor.a = 1.0f;
    return combineColor;
}

Texture2D<float4> blurTexture_0 : register(t0); // ブラーテクスチャ_0。1パス目で作成されたテクスチャ
Texture2D<float4> blurTexture_1 : register(t1); // ブラーテクスチャ_1。1パス目で作成されたテクスチャ


PSVerticalDiagonalBlurOutput PSVerticalDiagonalBlur(PSInput pIn)
{
    PSVerticalDiagonalBlurOutput psOut = (PSVerticalDiagonalBlurOutput) 0;

    // ブラーをかけるテクスチャのカラー
    float4 srcColor = mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv);
    
     // ブラーの半径からブラーステップの長さを求める
    float blurStepLen = BLUR_RADIUS / 4.0f;

    // 垂直方向に進める距離を計算（１ステップで）
    float2 uvOffset = float2(0.0f, 1.0f / BLUR_TEX_H);
    uvOffset *= blurStepLen;

    // 縦方向にカラーを取って平均する
    psOut.color_0 += mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset);

    psOut.color_0 += mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset * 2);

    psOut.color_0 += mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset * 3);

    psOut.color_0 += mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset * 4);
    
    psOut.color_0 /= 4.0f;

    // 対角線方向に進める距離を計算
    uvOffset.x = 0.86602f / BLUR_TEX_W;
    uvOffset.y = -0.5f / BLUR_TEX_H;
    uvOffset *= blurStepLen;

    // 対角線方向にカラーをサンプリングして平均化する
    psOut.color_1 = mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset);

    psOut.color_1 += mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset * 2);

    psOut.color_1 += mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset * 3);

    psOut.color_1 += mainRenderTargetTexture.Sample(
        g_sampler, pIn.uv + uvOffset * 4);

    psOut.color_1 += srcColor;
    psOut.color_1 /= 5.0f;

    // 垂直方向に平均化
    psOut.color_1 += psOut.color_0;
    psOut.color_1 /= 2.0f;

    return psOut;

}

float4 PSRhomboidBlur(PSInput pIn) : SV_Target0
{
    // ブラーステップの長さを求める
    float blurStepLen = BLUR_RADIUS / 4.0f;

    // 左斜め下方向へのUVオフセットを計算する
    float2 uvOffset;
    uvOffset.x = 0.86602f / BLUR_TEX_W;
    uvOffset.y = -0.5f / BLUR_TEX_H;
    uvOffset *= blurStepLen;

    // 左斜め下方向にカラーをサンプリングする
    float4 color = blurTexture_0.Sample(
        g_sampler, pIn.uv + uvOffset);

    color += blurTexture_0.Sample(
        g_sampler, pIn.uv + uvOffset * 2);

    color += blurTexture_0.Sample(
        g_sampler, pIn.uv + uvOffset * 3);

    color += blurTexture_0.Sample(
        g_sampler, pIn.uv + uvOffset * 4);

    // 右斜め下方向へのUVオフセットを計算する
    uvOffset.x = -0.86602f / BLUR_TEX_W * blurStepLen;

    // 右斜め下方向にカラーをサンプリングする
    color += blurTexture_1.Sample(
        g_sampler, pIn.uv);

    color += blurTexture_1.Sample(
        g_sampler, pIn.uv + uvOffset);

    color += blurTexture_1.Sample(
        g_sampler, pIn.uv + uvOffset * 2);

    color += blurTexture_1.Sample(
        g_sampler, pIn.uv + uvOffset * 3);

    color += blurTexture_1.Sample(
        g_sampler, pIn.uv + uvOffset * 4);

    // 平均
    color /= 9.0f;

    return color;
}
