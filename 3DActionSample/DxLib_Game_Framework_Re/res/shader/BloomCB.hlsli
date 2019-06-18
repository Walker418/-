#ifndef BLOOM_CB_HLSLI_
#define BLOOM_CB_HLSLI_

// ブルームエフェクト用定数バッファ
cbuffer BloomCB : register(b4)
{
    float   g_BrightPassThreshold;
    float   g_BaseSaturation;
    float   g_BaseIntensity;
    float   g_BloomSaturation;
    float   g_BloomIntensity;
    float4  g_GaussianWeights[2];
}

#endif