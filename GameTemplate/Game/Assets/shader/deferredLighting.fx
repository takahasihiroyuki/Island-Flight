/*!
 * @brief	スプライト用のシェーダー
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
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float2 uv : TEXCOORD0; //uv座標。
    float4 worldPos : TEXCOORD1; //ワールド座標
};

//ディレクションライト構造体
struct DirectionLight
{
    float3 direction; //ライトの方向
    float3 color; //ライトの色
};

//ライトの定数バッファー
cbuffer LightCB : register(b1)
{
    DirectionLight directionLight; //ディレクションライト
    float3 eyepos; //視点の位置
    float3 ambientColor; //アンビエントカラー
    float4x4 mLVP; //ライトのビュー×プロジェクション行列
}


Texture2D<float4> g_albedoTexture : register(t0); // アルベド
Texture2D<float4> g_normalTexture : register(t1); // 法線
Texture2D<float4> g_speculaTexture : register(t2); //スペキュラパワー
Texture2D<float4> g_worldPosTexture : register(t3); //ワールド座標
Texture2D<float4> g_shadowMap : register(t4); // シャドウマップ(GBufferではない)



sampler g_sampler : register(s0);

////////////////////////////////////////////////
// 関数定義。
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
	//GBufferの内容を使ってライティング
    float4 albedo = g_albedoTexture.Sample(g_sampler, psIn.uv);
    
    float4 normal = g_normalTexture.Sample(g_sampler, psIn.uv);
    normal = (normal * 2.0f) - 1.0f;
    
    float3 worldPos = g_worldPosTexture.Sample(g_sampler, psIn.uv).xyz;
    
    float3 dirLight = CalcLigFromDrectionLight(psIn, normal.xyz, worldPos);
    
    float shadowPow = CalcShadowPow(normal.w, psIn, worldPos);
    
    float3 lig = dirLight + ambientColor;
	
    //最終的な色
    float4 finalColor = albedo;
    
    finalColor.xyz *= lig;
    finalColor *= shadowPow;
    
    
    return finalColor;
}

//////////////////////////////////////////////////////////////////////////////////
//Lambert拡散反射を計算
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算し、ライトの影響度を求める
    float t = dot(normal, lightDirection);
	//内積の結果の-1をかける
    t *= -1.0f;
	//内積の結果が0以下なら0にする
    if (t < 0.0f)
    {
        t = 0.0f;
    }
    
   
	//ライトの影響度を返す
    return lightColor * t;
}

//////////////////////////////////////////////////////////////////////////////////
//phong鏡面反射を計算
//////////////////////////////////////////////////////////////////////////////////
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv)
{
	//反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);
	//光が当たったサーフェイス(表面)から視点に伸びるベクトルを求める
    float3 toEye = eyepos - worldPos;
    toEye = normalize(toEye);

	//鏡面反射の強さを求める
    float t = dot(refVec, toEye);

	//鏡面反射の強さを0~1にする
    t = max(0.0f, t);

	//鏡面反射の強さを絞る
    t = pow(t, 10.0f);
    
    float specPower = g_speculaTexture.Sample(g_sampler, uv);
	//鏡面反射光
    float3 specularLig = lightColor * t * specPower;

    return specularLig;
}

//////////////////////////////////////////////////////////////////////////////////
//ディレクションライトを計算
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(PSIn psIn, float3 normal, float3 worldPos)
{
	//拡散反射
    float3 diffDirection = CalcLambertDiffuse(
		directionLight.direction, directionLight.color, normal);
	//鏡面反射
    float3 specDirection = CalcPhongSpecular(
		directionLight.direction, directionLight.color, worldPos, normal, psIn.uv);

	//最終的な光
    return diffDirection + specDirection;
}

//////////////////////////////////////////////////////////////////////////////////
//影を計算
//////////////////////////////////////////////////////////////////////////////////

float CalcShadowPow(float isDrawShadow, PSIn psIn, float3 worldPos)
{
    float shadowPow = 1.0f;
    if (isDrawShadow == 1.0f)
    {
        //ライトビュースクリーン区間からUV座標空間に変換
        float4 posInLVP = mul(mLVP, float4(worldPos, 1.0f));
    
        float zInLVP = posInLVP.z / posInLVP.w;
        if (zInLVP >= 0.0f && zInLVP <= 1.0f)
        {

            float2 shadowUV = posInLVP.xy / posInLVP.w;
            shadowUV *= float2(0.5f, -0.5f);
            shadowUV += 0.5f;

	        //UV座標を使ってシャドウマップから影情報をサンプリング
            if (shadowUV.x >= 0.0f && shadowUV.x <= 1.0f &&
                shadowUV.y >= 0.0f && shadowUV.y <= 1.0f)
            {
                //シャドウマップから深度をサンプリング
                float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowUV).r;
                if (zInLVP > zInShadowMap)
                {
                    shadowPow = 0.5f; //影が落ちている。
                }
            }
        }
    }
    return shadowPow;
}
