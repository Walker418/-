#ifndef SHADER_ID_H_
#define SHADER_ID_H_

// 列挙型：シェーダーID
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

enum class ShaderID
{
	PS_BrightPass,
	PS_GaussianBlurH,
	PS_GaussianBlurV,
	PS_BloomCombine,

	PS_RadiationBlur,
};

#endif // !SHADER_ID_H_