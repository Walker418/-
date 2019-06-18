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
    float2 tex_offset = 1.0 / size;
    // 横方向でブラーをかける
    float4 result = 0;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy) * g_GaussianWeights[0].x;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 1, 0.0)) * g_GaussianWeights[0].y;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 2, 0.0)) * g_GaussianWeights[0].z;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 3, 0.0)) * g_GaussianWeights[0].w;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 4, 0.0)) * g_GaussianWeights[1].x;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 1, 0.0)) * g_GaussianWeights[0].y;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 2, 0.0)) * g_GaussianWeights[0].z;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 3, 0.0)) * g_GaussianWeights[0].w;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(tex_offset.x * 4, 0.0)) * g_GaussianWeights[1].x;

    PSOutput.Color0 = float4(result.rgb, 1.0);

    return PSOutput;
}