#include "DxLibPixelShader.hlsli"

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float2  TexCoord    : TEXCOORD0;    // テクスチャ座標
    float3  WTangent    : TEXCOORD1;    // 接線（ワールド空間）
    float3  WBinormal   : TEXCOORD2;    // 従法線（ワールド空間）
    float3  WNormal     : TEXCOORD3;    // 法線（ワールド空間）
    float4  WPosition   : TEXCOORD4;    // 頂点座標のベクトル（ワールド空間）
    float4  Position    : SV_POSITION;  // 座標（ プロジェクション空間）
};

// 定数バッファ
cbuffer PixelShaderCB : register(b4)
{
    float4  g_WorldViewPosition;        // 視点の位置
    float4  g_WorldLightPosition;       // ライトの位置
    float4  g_LightColor;               // ライトのカラー
}

// アルベド
SamplerState g_AlbedoMapSampler : register(s0);
Texture2D g_AlbedoMapTexture : register(t0);

// 法線マップ
SamplerState g_NormalMapSampler : register(s1);
Texture2D g_NormalMapTexture : register(t1);

// メタリック・スムースネス
SamplerState g_MetallicSmoothnessMapSampler : register(s2);
Texture2D g_MetallicSmoothnessMapTexture : register(t2);

// アンビエントオクルージョン
SamplerState g_AoMapSampler : register(s3);
Texture2D g_AoMapTexture : register(t3);

// IBL鏡面反射マップ
SamplerState g_IBLSpecularMapSampler : register(s11);
TextureCube g_IBLSpecularMapTexture : register(t11);

// IBL拡散反射マップ
SamplerState g_IBLDiffuseMapSampler : register(s12);
TextureCube g_IBLDiffuseMapTexture : register(t12);

// IBL BRDFマップ
SamplerState g_IBLBRDFMapSampler : register(s13);
Texture2D g_IBLBRDFMapTexture : register(t13);

// IBL鏡面反射マップのMIPMAP数
static const float u_mipmap_scale = 9;

// 円周率
static const float PI = 3.1415926f;

// ノンリニアからリニア空間に変換
float3 ConvertToLinear(float3 color)
{
    return pow(color, 2.2f);
}

// リニアからノンリニア空間に変換
float3 ConvertToNonLinear(float3 color)
{
    return pow(color, 1 / 2.2f);
}

// スムースネスをラフネスに変換
float SmoothnessToPerceptualRoughness(float smoothness)
{
    return (1.0f - smoothness);
}

// ラフネスをスムースネスに変換
float PerceptualRoughnessToRoughness(float perceptualRoughness)
{
    return perceptualRoughness * perceptualRoughness;
}

// 5乗
float Pow5(float x)
{
    return x * x * x * x * x;
}

// フレネル反射
float3 F_Schlick(float cosA, float3 f0)
{
    return f0 + (1.0f - f0) * Pow5(1.0f - cosA);
}

// マイクロファセット分布関数 Trowbridge-Reitz(GGX)
float D_GGX(float NoH, float linearRoughness)
{
    float a2 = linearRoughness * linearRoughness;
    float f = (NoH * a2 - NoH) * NoH + 1.0f;

    return a2 / (PI * f * f);
}

// 可視性関数
float V_SmithGGXCorrelatedFast(float NoV, float NoL, float linearRoughness)
{
    float a = linearRoughness;
    float GGXV = NoL * (NoV * (1.0f - a) + a);
    float GGXL = NoV * (NoL * (1.0f - a) + a);

    return 0.5f / (GGXV + GGXL + 1e-5);
}

// 鏡面反射BRDF
float SpecularBRDF(float3 specularColor, float NoH, float NoV, float NoL, float LoH, float linearRoughness)
{
    float D = D_GGX(NoH, linearRoughness);
    float V = V_SmithGGXCorrelatedFast(NoV, NoL, linearRoughness);
    float3 F = F_Schlick(LoH, specularColor);

    return D * V * F;
}

// 拡散反射
float Fd_Lambert()
{
    return 1.0f / PI;
}

// 拡散反射BRDF
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor * Fd_Lambert();
}

// IBLの計算
float3 ImageBasedLighting(float3 N, float3 R, float NoV, float3 diffuseColor, float3 specularColor, float perceptualRoughness)
{
    float3 Ld = g_IBLDiffuseMapTexture.Sample(g_IBLDiffuseMapSampler, N).rgb * diffuseColor * Fd_Lambert();
    float3 Lld = g_IBLSpecularMapTexture.SampleLevel(g_IBLSpecularMapSampler, R, perceptualRoughness * u_mipmap_scale).rgb;
    float2 Ldfg = g_IBLBRDFMapTexture.Sample(g_IBLBRDFMapSampler, float2(NoV, 1.0 - perceptualRoughness)).xy;
    float3 Lr = (specularColor * Ldfg.x + Ldfg.y) * Lld;

    return Ld + Lr;
}

// メイン関数
float4 main(PS_INPUT PSInput) : SV_Target0
{
    // アルベドカラー
    float3 albedoColor = g_AlbedoMapTexture.Sample(g_AlbedoMapSampler, PSInput.TexCoord.xy);
    albedoColor = ConvertToLinear(albedoColor);
    // メタリック、スムースネス
    float4 metallicSmoothness = g_MetallicSmoothnessMapTexture.Sample(g_MetallicSmoothnessMapSampler, PSInput.TexCoord.xy);
    // アンビエントオクルージョン
    float4 ao = g_AoMapTexture.Sample(g_AoMapSampler, PSInput.TexCoord.xy);

    // 拡散反射カラーを取得（非金属）
    float3 diffuseColor = lerp(albedoColor.rgb, 0.0f, metallicSmoothness.r);
    // 鏡面反射カラーを取得（金属）
    float3 specularColor = lerp(0.04f, albedoColor.rgb, metallicSmoothness.r);
    // ラフネスに変換
    float perceptualRoughness = SmoothnessToPerceptualRoughness(metallicSmoothness.a);
    // リニア空間のラフネスに変換
    float linearRoughness = PerceptualRoughnessToRoughness(perceptualRoughness);

    // 各種ベクトルを求める
    float3x3 TBN = float3x3(normalize(PSInput.WTangent), normalize(PSInput.WBinormal), normalize(PSInput.WNormal));
    // 法線ベクトル
    float3 N = mul(normalize(g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoord.xy).xyz * 2.0 - 1.0), TBN);

    float3 L = normalize(g_WorldLightPosition - PSInput.WPosition); // 光源方向のベクトル
    float3 V = normalize(g_WorldViewPosition - PSInput.WPosition); // 視点方向のベクトル
    float3 H = normalize(L + V); // 2等分ベクトル
    float3 R = reflect(-V, N); // 反射ベクトル

    // 各種ベクトルの内積を求める
    float NoV = abs(dot(N, V));
    float NoL = saturate(dot(N, L));
    float NoH = saturate(dot(N, H));
    float LoV = saturate(dot(L, V));
    float LoH = saturate(dot(L, H));

    // 放射照度を計算
    float3 irradiance = NoL * (g_LightColor.rgb * PI);

    // 双方向反射率分布関数（BRDF）の計算
    float3 Fd = DiffuseBRDF(diffuseColor);
    float3 Fr = SpecularBRDF(specularColor, NoH, NoV, NoL, LoH, linearRoughness);
    float3 BRDF = (Fd + Fr) * irradiance;
    // イメージベースドライティング
    float3 IBL = ImageBasedLighting(N, R, NoV, diffuseColor, specularColor, perceptualRoughness);

    // 最終カラーを計算
    float3 finalColor = (BRDF + IBL) * ao.rgb;
    finalColor = ConvertToNonLinear(finalColor);

    // カラーを出力
    return float4(finalColor, 1.0f);
}