#include "DxLibVertexShader.hlsli"

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
    float3  Position        : POSITION;         // ���W�i���[�J����ԁj
    float3  Normal          : NORMAL0;          // �@��
    float4  Diffuse         : COLOR0;           // �g�U���J���[
    float4  Specular        : COLOR1;           // ���ʔ��ˌ��J���[
    float4  TexCoords0      : TEXCOORD0;        // �e�N�X�`�����W0
    float4  TexCoords1      : TEXCOORD1;        // �e�N�X�`�����W1
    float3  Tangent         : TANGENT0;         // �ڐ�
    float3  Binormal        : BINORMAL0;        // �]�@��
    int4    BlendIndices0   : BLENDINDICES0;    // �{�[�������p�AFloat�^�萔�z��C���f�b�N�X0
    float4  BlendWeight0    : BLENDWEIGHT0;     // �{�[�������p�A�E�G�C�g�l0
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
    float2  TexCoord        : TEXCOORD0;        // �e�N�X�`�����W
    float3  WorldTangent    : TEXCOORD1;        // �ڐ��i���[���h��ԁj
    float3  WorldBinormal   : TEXCOORD2;        // �]�@���i���[���h��ԁj
    float3  WorldNormal     : TEXCOORD3;        // �@���i���[���h��ԁj
    float4  WorldPosition   : TEXCOORD4;        // ���C�g�����̃x�N�g���i���[���h��ԁj
    float4  Position        : SV_POSITION;      // ���W�i�v���W�F�N�V������ԁj
};

// ���C���֐�
VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT VSOutput; // ���ʂ̏o��

    // �X�L�����b�V���p�̃u�����h�s����쐬
    int4 IBoneFloatIndex;
    IBoneFloatIndex = VSInput.BlendIndices0;
    float4 ILocalWorldMatrix[3];
    ILocalWorldMatrix[0] = g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.x + 0] * VSInput.BlendWeight0.x;
    ILocalWorldMatrix[1] = g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.x + 1] * VSInput.BlendWeight0.x;
    ILocalWorldMatrix[2] = g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.x + 2] * VSInput.BlendWeight0.x;
    ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.y + 0] * VSInput.BlendWeight0.y;
    ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.y + 1] * VSInput.BlendWeight0.y;
    ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.y + 2] * VSInput.BlendWeight0.y;
    ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.z + 0] * VSInput.BlendWeight0.z;
    ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.z + 1] * VSInput.BlendWeight0.z;
    ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.z + 2] * VSInput.BlendWeight0.z;
    ILocalWorldMatrix[0] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.w + 0] * VSInput.BlendWeight0.w;
    ILocalWorldMatrix[1] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.w + 1] * VSInput.BlendWeight0.w;
    ILocalWorldMatrix[2] += g_LocalWorldMatrix.Matrix[ IBoneFloatIndex.w + 2] * VSInput.BlendWeight0.w;
    // ���[�J�����W���Z�b�g
    float4 ILocalPosition = float4(VSInput.Position, 1.0);
    // ���_���W�����[���h���W�ɕϊ�
    VSOutput.WorldPosition.x = dot(ILocalPosition, ILocalWorldMatrix[0]);
    VSOutput.WorldPosition.y = dot(ILocalPosition, ILocalWorldMatrix[1]);
    VSOutput.WorldPosition.z = dot(ILocalPosition, ILocalWorldMatrix[2]);
    VSOutput.WorldPosition.w = 1.0f;
    // ���_���W��������W�n�ɕϊ�
    float4 IViewPosition;
    IViewPosition.x = dot(VSOutput.WorldPosition, g_Base.ViewMatrix[0]);
    IViewPosition.y = dot(VSOutput.WorldPosition, g_Base.ViewMatrix[1]);
    IViewPosition.z = dot(VSOutput.WorldPosition, g_Base.ViewMatrix[2]);
    IViewPosition.w = 1.0f;
    // ���K���f�o�C�X���W�n�ɕϊ�
    VSOutput.Position.x = dot(IViewPosition, g_Base.ProjectionMatrix[0]);
    VSOutput.Position.y = dot(IViewPosition, g_Base.ProjectionMatrix[1]);
    VSOutput.Position.z = dot(IViewPosition, g_Base.ProjectionMatrix[2]);
    VSOutput.Position.w = dot(IViewPosition, g_Base.ProjectionMatrix[3]);
    // �@���x�N�g�������[���h���W�n�ɕϊ�
    VSOutput.WorldNormal.x = dot(VSInput.Normal, ILocalWorldMatrix[0].xyz);
    VSOutput.WorldNormal.y = dot(VSInput.Normal, ILocalWorldMatrix[1].xyz);
    VSOutput.WorldNormal.z = dot(VSInput.Normal, ILocalWorldMatrix[2].xyz);
    // �ڃx�N�g�������[���h���W�n�ɕϊ�
    VSOutput.WorldTangent.x = dot(VSInput.Tangent, ILocalWorldMatrix[0].xyz);
    VSOutput.WorldTangent.y = dot(VSInput.Tangent, ILocalWorldMatrix[1].xyz);
    VSOutput.WorldTangent.z = dot(VSInput.Tangent, ILocalWorldMatrix[2].xyz);
    // �]�@���x�N�g�������[���h���W�n�ɕϊ�
    VSOutput.WorldBinormal.x = dot(VSInput.Binormal, ILocalWorldMatrix[0].xyz);
    VSOutput.WorldBinormal.y = dot(VSInput.Binormal, ILocalWorldMatrix[1].xyz);
    VSOutput.WorldBinormal.z = dot(VSInput.Binormal, ILocalWorldMatrix[2].xyz);

    // �e�N�X�`�����W���Z�b�g
    VSOutput.TexCoord.xy = VSInput.TexCoords0.xy;

    // ���ʂ�Ԃ�
    return VSOutput;
}