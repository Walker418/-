#ifndef PIXEL_SHADER_CB_
#define PIXEL_SHADER_CB_

#include <DxLib.h>

struct PixelShaderCB
{
	VECTOR	ViewPosition;	// 視点の位置
	float	padding0;		// パディング
	VECTOR	LightPosition;	// ライトの位置
	float	padding1;		// パディング
	VECTOR	LightColor;		// ライトの色
	float	padding2;		// パディング
};

#endif // !PIXEL_SHADER_CB_