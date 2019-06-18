#ifndef DX_LIB_SHADER_COMMON_HLSLI_
#define DX_LIB_SHADER_COMMON_HLSLI_

#define DX_D3D11_COMMON_CONST_LIGHT_NUM         (6)     // 共通パラメータのライトの最大数

// 構造体定義 --------------------------------------------------------------------

// マテリアルパラメータ
struct DX_D3D11_CONST_MATERIAL
{
    float4      Diffuse;                // ディフューズカラー
    float4      Specular;               // スペキュラカラー
    float4      Ambient_Emissive;       // マテリアルエミッシブカラー + マテリアルアンビエントカラー * グローバルアンビエントカラー
    float       Power;                  // スペキュラの強さ
    float3      Padding;                // パディング
};

// フォグパラメータ
struct DX_D3D11_VS_CONST_FOG
{
    float       LinearAdd;              // フォグ用パラメータ end / ( end - start )
    float       LinearDiv;              // フォグ用パラメータ -1  / ( end - start )
    float       Density;                // フォグ用パラメータ density
    float       E;                      // フォグ用パラメータ 自然対数の低
    float4      Color;                  // カラー
};

// ライトパラメータ
struct DX_D3D11_CONST_LIGHT
{
    int         Type;                   // ライトタイプ( DX_LIGHTTYPE_POINT など )
    int3        Padding1;               // パディング１

    float3      Position;               // 座標( ビュー空間 )
    float       RangePow2;              // 有効距離の２乗

    float3      Direction;              // 方向( ビュー空間 )
    float       FallOff;                // スポットライト用FallOff

    float3      Diffuse;                // ディフューズカラー
    float       SpotParam0;             // スポットライト用パラメータ０( cos( Phi / 2.0f ) )

    float3      Specular;               // スペキュラカラー
    float       SpotParam1;             // スポットライト用パラメータ１( 1.0f / ( cos( Theta / 2.0f ) - cos( Phi / 2.0f ) ) )

    float4      Ambient;                // アンビエントカラーとマテリアルのアンビエントカラーを乗算したもの

    float       Attenuation0;           // 距離による減衰処理用パラメータ０
    float       Attenuation1;           // 距離による減衰処理用パラメータ１
    float       Attenuation2;           // 距離による減衰処理用パラメータ２
    float       Padding2;               // パディング２
};

// ピクセルシェーダー・頂点シェーダー共通パラメータ
struct DX_D3D11_CONST_BUFFER_COMMON
{
    DX_D3D11_CONST_LIGHT        Light[DX_D3D11_COMMON_CONST_LIGHT_NUM];         // ライトパラメータ
    DX_D3D11_CONST_MATERIAL     Material;                                       // マテリアルパラメータ
    DX_D3D11_VS_CONST_FOG       Fog;                                            // フォグパラメータ
};

// 頂点シェーダー・ピクセルシェーダー共通パラメータ
cbuffer cbD3D11_CONST_BUFFER_COMMON : register(b0)
{
    DX_D3D11_CONST_BUFFER_COMMON    g_Common;
};

#endif
