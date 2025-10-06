/*!
 * @brief	スカイキューブ用のシェーダー。
 */

cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

// 頂点シェーダーの入力
struct SVSIn
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

//ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION; //座標
    float3 normal : NORMAL; //法線
    float3 tangent : TANGENT; //接ベクトル
    float3 biNormal : BINORMAL; //従ベクトル
    float2 uv : TEXCOORD0; //UV座標
    float3 worldPos : TEXCOORD1; //ワールド座標
    
    //反射オブジェクトの場合に使う。
    float clip : SV_ClipDistance0; //クリップ距離 
};
cbuffer SkyCubeCb : register(b1)
{
    float luminance; // 輝度
    float4 reflectionPlane; //反射平面の方程式。
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
Texture2D<float4> g_normal : register(t1); //ノーマルマップ
Texture2D<float4> g_spacular : register(t2); //スペキュラマップ
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
    //内積が負ならクリップする。
        psIn.clip = dot(reflectionPlane, posVS);
    }
    else
    {
        psIn.clip = 1.0f; //クリップしない。
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