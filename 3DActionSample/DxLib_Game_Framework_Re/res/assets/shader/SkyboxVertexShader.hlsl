#include "DxLibVertexShader.hlsli"

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
    float4 Position : POSITION;     // ���W�i���[�J����ԁj
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
    float3 TexCoord : TEXCOORD0;    // �e�N�X�`�����W
    float4 Position : SV_POSITION;  // ���W�i�v���W�F�N�V������ԁj
};

// main�֐�
VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT VSOutout;
    // ���_���W������n���W�ɕϊ�
    float4 IViewPosition;
    IViewPosition.x = dot(VSInput.Position.xyz, g_Base.ViewMatrix[0].xyz);
    IViewPosition.y = dot(VSInput.Position.xyz, g_Base.ViewMatrix[1].xyz);
    IViewPosition.z = dot(VSInput.Position.xyz, g_Base.ViewMatrix[2].xyz);
    IViewPosition.w = 1.0f;
    // ���K���f�o�C�X���W�ɕϊ�
    VSOutout.Position.x = dot(IViewPosition, g_Base.ProjectionMatrix[0]);
    VSOutout.Position.y = dot(IViewPosition, g_Base.ProjectionMatrix[1]);
    VSOutout.Position.z = dot(IViewPosition, g_Base.ProjectionMatrix[2]);
    VSOutout.Position.w = dot(IViewPosition, g_Base.ProjectionMatrix[3]);
    // �e�N�X�`�����W���o��
    VSOutout.TexCoord.xyz = VSInput.Position.xyz;

    return VSOutout;
}