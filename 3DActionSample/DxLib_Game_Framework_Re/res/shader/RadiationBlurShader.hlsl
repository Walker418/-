#include "DxLibPixelShader.hlsli"

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Position     : SV_POSITION;
	float4 Diffuse      : COLOR0;
	float2 TexCoords0   : TEXCOORD0;
	float2 TexCoords1   : TEXCOORD1;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0       : SV_TARGET0;
};

SamplerState    g_BaseMapSampler : register(s0);
Texture2D       g_BaseMapTexture : register(t0);

cbuffer RadialCB:register(b6){
	float _Strength;	// �ڂ����̋���
	int _SampleCount;	// �����R�}��
	float Dummy;
	float Dummy2;
};

PS_OUTPUT main(PS_INPUT PSInput)
{
	PS_OUTPUT PSOutput;
	// �g�U���˃J���[�̓����_�[�^�[�Q�b�g�p�̃e�N�X�`�����Q��
	float4 TextureDiffuseColor = g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy);
	// �u���[����
	float4 col = 0;
	float2 symmetryUv = PSInput.TexCoords0 - 0.5;
	
	float distance = length(symmetryUv);
	
	float factor = _Strength / _SampleCount * distance;
	
	for (int i = 0; i < _SampleCount; i++) {
		half uvOffset = 1 - factor * i;
		col += g_BaseMapTexture.Sample(g_BaseMapSampler, symmetryUv * uvOffset + 0.5);
	}

	col /= _SampleCount;

	// �o�̓J���[��ݒ�
	PSOutput.Color0 =col;
	
	return PSOutput;
}