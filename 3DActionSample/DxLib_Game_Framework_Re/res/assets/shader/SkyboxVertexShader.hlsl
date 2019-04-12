#include "DxLibVertexShader.hlsli"

// 頂点シェーダーの入力
struct VS_INPUT
{
    float4 Position : POSITION;     // 座標（ローカル空間）
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
    float3 TexCoord : TEXCOORD0;    // テクスチャ座標
    float4 Position : SV_POSITION;  // 座標（プロジェクション空間）
};

// main関数
VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT VSOutout;
    // 頂点座標を視野系座標に変換
    float4 IViewPosition;
    IViewPosition.x = dot(VSInput.Position.xyz, g_Base.ViewMatrix[0].xyz);
    IViewPosition.y = dot(VSInput.Position.xyz, g_Base.ViewMatrix[1].xyz);
    IViewPosition.z = dot(VSInput.Position.xyz, g_Base.ViewMatrix[2].xyz);
    IViewPosition.w = 1.0f;
    // 正規化デバイス座標に変換
    VSOutout.Position.x = dot(IViewPosition, g_Base.ProjectionMatrix[0]);
    VSOutout.Position.y = dot(IViewPosition, g_Base.ProjectionMatrix[1]);
    VSOutout.Position.z = dot(IViewPosition, g_Base.ProjectionMatrix[2]);
    VSOutout.Position.w = dot(IViewPosition, g_Base.ProjectionMatrix[3]);
    // テクスチャ座標を出力
    VSOutout.TexCoord.xyz = VSInput.Position.xyz;

    return VSOutout;
}