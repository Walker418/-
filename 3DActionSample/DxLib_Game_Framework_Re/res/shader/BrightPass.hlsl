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

SamplerState    g_BaseMapSampler : register(s0);
Texture2D       g_BaseMapTexture : register(t0);

PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    // テクスチャサイズを取得
    float2 size;
    g_BaseMapTexture.GetDimensions(size.x, size.y);
    // テクスチャオフセットを計算
    float2 tex_offset = 1.0f / size;
    // 周囲4ピクセルの平均を求める
    float4 result = 0.0f;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(-0.5f,  0.5f) * tex_offset);
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(-0.5f, -0.5f) * tex_offset);
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2( 0.5f,  0.5f) * tex_offset);
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2( 0.5f, -0.5f) * tex_offset);
    result *= 0.25;
    // ピクセルの輝度を求める
    float luminance = dot(result.rgb, float3(0.33f, 0.34f, 0.33f));
    // 指定した輝度以上のピクセルだけを残す
    luminance = max(0.0f, luminance - g_BrightPassThreshold);
    result.rgb *= sign(luminance);
    PSOutput.Color0 = float4(result.rgb, 1.0f);
    return PSOutput;
}