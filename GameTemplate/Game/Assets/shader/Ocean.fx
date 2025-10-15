/*!
 * @brief	シンプルなモデルシェーダー。
 */

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn
{
    int4 Indices : BLENDINDICES0;
    float4 Weights : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn
{
    float4 pos : POSITION; //モデルの頂点座標。
    float2 uv : TEXCOORD0; //UV座標。
    SSkinVSIn skinVert; //スキン用のデータ。
    float3 normal : NORMAL; //法線
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;

};
//ピクセルシェーダーへの入力。
struct SPSIn
{
    float4 pos : SV_POSITION; //スクリーン空間でのピクセルの座標。
    float2 uv : TEXCOORD0; //uv座標。
    float3 worldPos : TEXCOORD1; //ワールド座標
    float3 normal : NORMAL; //法線
    float3 tangent : TANGENT; // 接ベクトル
    float3 biNormal : BINORMAL; // 従ベクトル
    float4 posRefCamViewProj : TEXCOORD3; // 反射用カメラのビュー投影行列をかけた座標
    float4 refClip : TEXCOORDn;
};

//ディレクションライト構造体
struct DirectionLight
{
    float3 direction; //ライトの方向
    float3 color; //ライトの色
};

struct Light
{
    DirectionLight directionLight; //ディレクションライト
    float3 cameraEyePos; //カメラの座標
    float3 ambientColor; //アンビエントカラー
    float4x4 mLVP; //ライトビュー投影行列。
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

//海用の定数バッファー
cbuffer LightCb : register(b1)
{
    float4x4 ReflectionCameraVP; // 反射用カメラビュー投影行列
    Light light;
    //反射の割合の下限値、必ずこの値以上は反射する。（真上から見た反射率）
    float baseReflectance; // 基本反射率

}

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0); //アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3); //ボーン行列。
sampler g_sampler : register(s0); //サンプラステート。
Texture2D<float4> g_normalMap : register(t1); //法線マップにアクセスするための変数。
Texture2D<float4> g_specularMap : register(t2); //スペキュラマップにアクセスするための変数。
Texture2D<float4> g_refLect : register(t10); // 反射マップ


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(SPSIn psIn, float3 normal);
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv);
float2 CalcReflectUV(float4 clip);
float ComputeFresnel(float3 normal, float3 viewDir, float baseReflectance);

/// <summary>
//スキン行列を計算する。
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
/// 頂点シェーダーのコア関数。
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
    
    //接ベクトルと従ベクトルをワールド空間に変換する
    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    
    //クリップ座標を用意しておく。
    psIn.refClip = mul(ReflectionCameraVP, float4(psIn.worldPos.xyz, 1.0));
    
    return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    
    float3 ligDirection = light.directionLight.direction;
    
    //アルベドテクスチャのサンプリング
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
    
    //法線マップのサンプリング
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    //0～1の範囲を-1～1の範囲にする。
    localNormal = (localNormal - 0.5f) * 2.0f;
    //接ベクトル空間からワールド空間に変換する
    float3 normal = psIn.normal;
    normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + normal * localNormal.z;

    
    float2 uvR = CalcReflectUV(psIn.refClip);
    uvR = saturate(uvR);
    float4 reflect = g_refLect.Sample(g_sampler, uvR);


    
    //ライトの計算
    float3 directionLight = CalcLigFromDrectionLight(psIn, normal);
    float3 lig = directionLight;
    
    //最終的な色
    float4 finalColor = albedoColor;
    albedoColor = float4(0.20f,0.35f,0.47f,0.0f);
    finalColor.xyz *= directionLight;
    //finalColor.xyz = float3(1.0f, 0.0f, 1.0f); //環境光を足す
    finalColor = reflect;
    
    //フレネル反射率を計算
    float flesnel = ComputeFresnel(normal, normalize(light.cameraEyePos - psIn.worldPos), baseReflectance);
    
    finalColor = lerp(albedoColor, reflect, flesnel);
    
    
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
    float3 toEye = light.cameraEyePos - worldPos;
    toEye = normalize(toEye);

	//鏡面反射の強さを求める
    float t = dot(refVec, toEye);

	//鏡面反射の強さを0~1にする
    t = max(0.0f, t);

	//鏡面反射の強さを絞る
    t = pow(t, 10.0f);

    //スペキュラマップからスペキュラ反射の強さをサンプリング
    float specPower = g_specularMap.Sample(g_sampler, uv).r;
    
	//鏡面反射光
    float3 specularLig = lightColor * t * specPower;

    return specularLig;
}

//////////////////////////////////////////////////////////////////////////////////
//ディレクションライトを計算
//////////////////////////////////////////////////////////////////////////////////
float3 CalcLigFromDrectionLight(SPSIn psIn, float3 normal)
{
	//拡散反射
    float3 diffDirection = CalcLambertDiffuse(
		light.directionLight.direction, light.directionLight.color, normal);
	//鏡面反射
    float3 specDirection = CalcPhongSpecular(
		light.directionLight.direction, light.directionLight.color, psIn.worldPos, normal, psIn.uv);

	//最終的な光
    return diffDirection + specDirection;
}


float2 CalcReflectUV(float4 clip)
{
    //これから除算するので、0割りを防止するために絶対値の最大値を1e-6で制限する。
    float w = max(abs(clip.w), 1e-6);
    //rcpは1/xを計算する関数。(普通にするより早い)
    float invW = rcp(w);
    float2 uv = clip.xy * invW * 0.5f + 0.5f; // NDC→[0,1]
    uv.y = 1.0f - uv.y;
    return uv;
}

float ComputeFresnel(float3 normal, float3 viewDir, float baseReflectance)
{
    
    float cosTheta = saturate(dot(normalize(normal), -normalize(viewDir)));
    
    //角度により反射率の係数。
    float angleFactor = pow(1.0f - cosTheta, 5.0f);
    //angleFactorの割合の上限。
    float remainingReflectance = 1 - baseReflectance;

    //フレネル反射率
    return baseReflectance + remainingReflectance * angleFactor;
}