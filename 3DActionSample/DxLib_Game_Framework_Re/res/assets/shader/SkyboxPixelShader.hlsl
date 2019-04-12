// 頂点シェーダーの出力
struct PS_INPUT
{
    float3 TexCoord : TEXCOORD0;    // テクスチャ座標
};

SamplerState g_EnvMapSampler : register(s11); // 環境マップ
TextureCube g_EnvMapTexture : register(t11); // 環境マップ

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    return g_EnvMapTexture.Sample(g_EnvMapSampler, PSInput.TexCoord);
}