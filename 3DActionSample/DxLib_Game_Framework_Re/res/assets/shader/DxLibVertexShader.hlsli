#ifndef DX_LIB_VERTEX_SHADER_HLSLI_
#define DX_LIB_VERTEX_SHADER_HLSLI_

#include "DxLibShaderCommon.hlsli"

// マクロ定義 -----------------------------------

#define DX_VERTEXLIGHTING_LIGHT_NUM                     3           // 頂点単位ライティングで同時に使用できるライトの最大数
#define DX_D3D11_VS_CONST_TEXTURE_MATRIX_NUM            3           // テクスチャ座標変換行列の転置行列の数
#define DX_D3D11_VS_CONST_WORLD_MAT_NUM                 54          // トライアングルリスト一つで同時に使用するローカル→ワールド行列の最大数

// データ型定義 ---------------------------------

// 基本パラメータ
struct DX_D3D11_VS_CONST_BUFFER_BASE
{
    float4      AntiViewportMatrix[4];              // アンチビューポート行列
    float4      ProjectionMatrix[4];                // ビュー　→　プロジェクション行列
    float4      ViewMatrix[3];                      // ワールド　→　ビュー行列
    float4      LocalWorldMatrix[3];                // ローカル　→　ワールド行列

    float4      ToonOutLineSize;                    // トゥーンの輪郭線の大きさ
    float       DiffuseSource;                      // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
    float       SpecularSource;                     // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
    float       MulSpecularColor;                   // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
    float       Padding;
};

// その他の行列
struct DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX
{
    float4      ShadowMapLightViewProjectionMatrix[3][4];                   // シャドウマップ用のライトビュー行列とライト射影行列を乗算したもの
    float4      TextureMatrix[DX_D3D11_VS_CONST_TEXTURE_MATRIX_NUM][2]; // テクスチャ座標操作用行列
};

// スキニングメッシュ用の　ローカル　→　ワールド行列
struct DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX
{
    float4      Matrix[DX_D3D11_VS_CONST_WORLD_MAT_NUM * 3];                    // ローカル　→　ワールド行列
};

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_VS_BASE : register(b1)
{
    DX_D3D11_VS_CONST_BUFFER_BASE               g_Base;
};

// その他の行列
cbuffer cbD3D11_CONST_BUFFER_VS_OTHERMATRIX : register(b2)
{
    DX_D3D11_VS_CONST_BUFFER_OTHERMATRIX        g_OtherMatrix;
};

// スキニングメッシュ用の　ローカル　→　ワールド行列
cbuffer cbD3D11_CONST_BUFFER_VS_LOCALWORLDMATRIX : register(b3)
{
    DX_D3D11_VS_CONST_BUFFER_LOCALWORLDMATRIX   g_LocalWorldMatrix;
};

#endif

