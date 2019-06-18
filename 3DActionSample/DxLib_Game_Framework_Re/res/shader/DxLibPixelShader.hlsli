#ifndef DX_LIB_PIXEL_SHADER_H_
#define DX_LIB_PIXEL_SHADER_H_

#include "DxLibShaderCommon.hlsli"

#define DX_PIXELLIGHTING_LIGHT_NUM          (6)     // ピクセル単位ライティングで同時に使用できるライトの最大数
#define DX_D3D11_PS_CONST_FILTER_SIZE       (256)   // フィルター用定数バッファのサイズ

// データ型定義 ---------------------------------

// シャドウマップパラメータ
struct DX_D3D11_PS_CONST_SHADOWMAP
{
    float       AdjustDepth;            // 閾値深度補正値
    float       GradationParam;         // グラデーション範囲
    float       Enable_Light0;          // ライト０への適用情報
    float       Enable_Light1;          // ライト１への適用情報

    float       Enable_Light2;          // ライト２への適用情報
    float3      Padding;                // パディング
};

// 定数バッファピクセルシェーダー基本パラメータ
struct DX_D3D11_PS_CONST_BUFFER_BASE
{
    float4      FactorColor;            // アルファ値等

    float       MulAlphaColor;          // カラーにアルファ値を乗算するかどうか( 0.0f:乗算しない  1.0f:乗算する )
    float       AlphaTestRef;           // アルファテストで使用する比較値
    float2      Padding1;

    int         AlphaTestCmpMode;       // アルファテスト比較モード( DX_CMP_NEVER など )
    int3        Padding2;
};

// 定数バッファシャドウマップパラメータ
struct DX_D3D11_PS_CONST_BUFFER_SHADOWMAP
{
    DX_D3D11_PS_CONST_SHADOWMAP Data[3];
};

// 定数定義 ----------------------------------------------------------------------

// 基本パラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_BASE : register(b1)
{
    DX_D3D11_PS_CONST_BUFFER_BASE       g_Base;
};

// シャドウマップパラメータ
cbuffer cbD3D11_CONST_BUFFER_PS_SHADOWMAP : register(b2)
{
    DX_D3D11_PS_CONST_BUFFER_SHADOWMAP  g_ShadowMap;
};

#endif
