#include "BloomCB.hlsli"

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

PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    // �e�N�X�`���T�C�Y���擾
    float2 size;
    g_BaseMapTexture.GetDimensions(size.x, size.y);
    // �e�N�X�`���I�t�Z�b�g���v�Z
    float2 tex_offset = 1.0f / size;
    // ����4�s�N�Z���̕��ς����߂�
    float4 result = 0.0f;
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(-0.5f,  0.5f) * tex_offset);
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2(-0.5f, -0.5f) * tex_offset);
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2( 0.5f,  0.5f) * tex_offset);
    result += g_BaseMapTexture.Sample(g_BaseMapSampler, PSInput.TexCoords0.xy + float2( 0.5f, -0.5f) * tex_offset);
    result *= 0.25;
    // �s�N�Z���̋P�x�����߂�
    float luminance = dot(result.rgb, float3(0.33f, 0.34f, 0.33f));
    // �w�肵���P�x�ȏ�̃s�N�Z���������c��
    luminance = max(0.0f, luminance - g_BrightPassThreshold);
    result.rgb *= sign(luminance);
    PSOutput.Color0 = float4(result.rgb, 1.0f);
    return PSOutput;
}