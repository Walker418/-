#ifndef PIXEL_SHADER_CB_
#define PIXEL_SHADER_CB_

#include <DxLib.h>

struct PixelShaderCB
{
	VECTOR	ViewPosition;	// ���_�̈ʒu
	float	padding0;		// �p�f�B���O
	VECTOR	LightPosition;	// ���C�g�̈ʒu
	float	padding1;		// �p�f�B���O
	VECTOR	LightColor;		// ���C�g�̐F
	float	padding2;		// �p�f�B���O
};

#endif // !PIXEL_SHADER_CB_