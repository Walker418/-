#include "BloomCB.hlsli"

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Position     : SV_POSITION;
    float4 Diffuse      : COLOR0;
    float2 TexCoords0   : TEXCOORD0;
    float2 TexCoords1   : TEXCOORD1;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Color0       : SV_TARGET0;
};

// 元画像テクスチャ
SamplerState    g_BaseSampler : register(s0);
Texture2D       g_BaseTexture : register(t0);
// ブルームテクスチャ
SamplerState    g_BloomSampler : register(s1);
Texture2D       g_BloomTexture : register(t1);

// 彩度の調節
float3 AdjustSaturation(float3 color, float saturation)
{
    // 色をグレースケールに変換
    float grey = dot(color, float3(0.3f, 0.59f, 0.11f));
    // 彩度を調整して返す
    return lerp(grey, color, saturation);
}

PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    // ベースカラーを調整
    float4 base_color = g_BaseTexture.Sample(g_BaseSampler, PSInput.TexCoords0.xy);
    base_color.rgb = AdjustSaturation(base_color.rgb, g_BaseSaturation).rgb * g_BaseIntensity;
    // ブルームカラーを調整
    float4 bloom_color = g_BloomTexture.Sample(g_BloomSampler, PSInput.TexCoords0.xy);
    bloom_color.rgb = AdjustSaturation(bloom_color.rgb, g_BloomSaturation).rgb * g_BloomIntensity;
    // ベースカラーとブルームカラーを合成
    float3 result = base_color.rgb + bloom_color.rgb;
    // 最終カラーを出力
    PSOutput.Color0 = float4(result.rgb, 1.0f);

    return PSOutput;
}