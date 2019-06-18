#ifndef BLOOM_CB_H_
#define BLOOM_CB_H_

// ブルームシェーダー用定数バッファ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

struct BloomCB
{
	// 輝度の閾値
	float g_BrightPassThreshold{ 1.0f };
	// 元画像の彩度
	float g_BaseSaturation{ 1.0f };
	// 元画像の輝度
	float g_BaseIntensity{ 1.0f };
	// ブルーム画像の彩度
	float g_BloomSaturation{ 1.0f };
	// ブルーム画像の輝度
	float g_BloomIntensity{ 1.0f };
	// パディング1
	float padding1[3]{ 0.0f };
	// ガウシアンブラーのウェイト値
	float g_GaussianWeights[5]{ 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };
	// パディング2
	float padding2[3]{ 0.0f };
};

#endif // !BLOOM_CB_H_