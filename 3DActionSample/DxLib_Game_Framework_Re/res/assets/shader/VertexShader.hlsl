#include "DxLibVertexShader.hlsli"

// 頂点シェーダーの入力
struct VS_INPUT
{
    float3  Position        : POSITION;         // 座標（ローカル空間）
    float3  Normal          : NORMAL0;          // 法線
    float4  Diffuse         : COLOR0;           // 拡散光カラー
    float4  Specular        : COLOR1;           // 鏡面反射光カラー
    float4  TexCoords0      : TEXCOORD0;        // テクスチャ座標0
    float4  TexCoords1      : TEXCOORD1;        // テクスチャ座標1
    float3  Tangent         : TANGENT0;         // 接線
    float3  Binormal        : BINORMAL0;        // 従法線
    int4    BlendIndices0   : BLENDINDICES0;    // ボーン処理用、Float型定数配列インデックス0
    float4  BlendWeight0    : BLENDWEIGHT0;     // ボーン処理用、ウエイト値0
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
    float2  TexCoord        : TEXCOORD0;        // テクスチャ座標
    float3  WorldTangent    : TEXCOORD1;        // 接線（ワールド空間）
    float3  WorldBinormal   : TEXCOORD2;        // 従法線（ワールド空間）
    float3  WorldNormal     : TEXCOORD3;        // 法線（ワールド空間）
    float4  WorldPosition   : TEXCOORD4;        // ライト方向のベクトル（ワールド空間）
    float4  Position        : SV_POSITION;      // 座標（プロジェクション空間）
};

// メイン関数
VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT VSOutput; // 結果の出力

    // スキンメッシュ用のブレンド行列を作成
    int4 IBoneFloatIndex;
    IBoneFloatIndex = VSInput.BlendIndices0;
    float4 ILocalWorldMatrix[3];
    ILocalWorldMatrix[0] = g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.x + 0] * VSInput.BlendWeight0.x;
    ILocalWorldMatrix[1] = g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.x + 1] * VSInput.BlendWeight0.x;
    ILocalWorldMatrix[2] = g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.x + 2] * VSInput.BlendWeight0.x;
    ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.y + 0] * VSInput.BlendWeight0.y;
    ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.y + 1] * VSInput.BlendWeight0.y;
    ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.y + 2] * VSInput.BlendWeight0.y;
    ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.z + 0] * VSInput.BlendWeight0.z;
    ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.z + 1] * VSInput.BlendWeight0.z;
    ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.z + 2] * VSInput.BlendWeight0.z;
    ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.w + 0] * VSInput.BlendWeight0.w;
    ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.w + 1] * VSInput.BlendWeight0.w;
    ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.w + 2] * VSInput.BlendWeight0.w;
    // ローカル座標をセット
    float4 ILocalPosition = float4(VSInput.Position, 1.0);
    // 頂点座標をワールド座標に変換
    VSOutput.WorldPosition.x = dot(ILocalPosition, ILocalWorldMatrix[0]);
    VSOutput.WorldPosition.y = dot(ILocalPosition, ILocalWorldMatrix[1]);
    VSOutput.WorldPosition.z = dot(ILocalPosition, ILocalWorldMatrix[2]);
    VSOutput.WorldPosition.w = 1.0f;
    // 頂点座標を視野座標系に変換
    float4 IViewPosition;
    IViewPosition.x = dot(VSOutput.WorldPosition, g_Base.ViewMatrix[0]);
    IViewPosition.y = dot(VSOutput.WorldPosition, g_Base.ViewMatrix[1]);
    IViewPosition.z = dot(VSOutput.WorldPosition, g_Base.ViewMatrix[2]);
    IViewPosition.w = 1.0f;
    // 正規化デバイス座標系に変換
    VSOutput.Position.x = dot(IViewPosition, g_Base.ProjectionMatrix[0]);
    VSOutput.Position.y = dot(IViewPosition, g_Base.ProjectionMatrix[1]);
    VSOutput.Position.z = dot(IViewPosition, g_Base.ProjectionMatrix[2]);
    VSOutput.Position.w = dot(IViewPosition, g_Base.ProjectionMatrix[3]);
    // 法線ベクトルをワールド座標系に変換
    VSOutput.WorldNormal.x = dot(VSInput.Normal, ILocalWorldMatrix[0].xyz);
    VSOutput.WorldNormal.y = dot(VSInput.Normal, ILocalWorldMatrix[1].xyz);
    VSOutput.WorldNormal.z = dot(VSInput.Normal, ILocalWorldMatrix[2].xyz);
    // 接ベクトルをワールド座標系に変換
    VSOutput.WorldTangent.x = dot(VSInput.Tangent, ILocalWorldMatrix[0].xyz);
    VSOutput.WorldTangent.y = dot(VSInput.Tangent, ILocalWorldMatrix[1].xyz);
    VSOutput.WorldTangent.z = dot(VSInput.Tangent, ILocalWorldMatrix[2].xyz);
    // 従法線ベクトルをワールド座標系に変換
    VSOutput.WorldBinormal.x = dot(VSInput.Binormal, ILocalWorldMatrix[0].xyz);
    VSOutput.WorldBinormal.y = dot(VSInput.Binormal, ILocalWorldMatrix[1].xyz);
    VSOutput.WorldBinormal.z = dot(VSInput.Binormal, ILocalWorldMatrix[2].xyz);

    // テクスチャ座標をセット
    VSOutput.TexCoord.xy = VSInput.TexCoords0.xy;

    // 結果を返す
    return VSOutput;
}