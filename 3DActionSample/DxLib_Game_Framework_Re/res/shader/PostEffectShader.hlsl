#include "DxLibPixelShader.hlsli"

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
    // 拡散反射カラーはレンダーターゲット用のテクスチャを参照
    float4 TextureDiffuseColor = g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy);
    // グレースケールに変換
    float intensity = dot(TextureDiffuseColor.rgb, float3(0.299f, 0.587f, 0.114f));
    // 出力カラーを設定
    PSOutput.Color0.rgb = intensity;
    PSOutput.Color0.a = 1.0f;

    return PSOutput;
}