#include "DxLibPixelShader.hlsli"

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float2  TexCoord    : TEXCOORD0;    // �e�N�X�`�����W
    float3  WTangent    : TEXCOORD1;    // �ڐ��i���[���h��ԁj
    float3  WBinormal   : TEXCOORD2;    // �]�@���i���[���h��ԁj
    float3  WNormal     : TEXCOORD3;    // �@���i���[���h��ԁj
    float4  WPosition   : TEXCOORD4;    // ���_���W�̃x�N�g���i���[���h��ԁj
    float4  Position    : SV_POSITION;  // ���W�i �v���W�F�N�V������ԁj
};

// �萔�o�b�t�@
cbuffer PixelShaderCB : register(b4)
{
    float4  g_WorldViewPosition;        // ���_�̈ʒu
    float4  g_WorldLightPosition;       // ���C�g�̈ʒu
    float4  g_LightColor;               // ���C�g�̃J���[
}

// �A���x�h
SamplerState g_AlbedoMapSampler : register(s0);
Texture2D g_AlbedoMapTexture : register(t0);

// �@���}�b�v
SamplerState g_NormalMapSampler : register(s1);
Texture2D g_NormalMapTexture : register(t1);

// ���^���b�N�E�X���[�X�l�X
SamplerState g_MetallicSmoothnessMapSampler : register(s2);
Texture2D g_MetallicSmoothnessMapTexture : register(t2);

// �A���r�G���g�I�N���[�W����
SamplerState g_AoMapSampler : register(s3);
Texture2D g_AoMapTexture : register(t3);

// IBL���ʔ��˃}�b�v
SamplerState g_IBLSpecularMapSampler : register(s11);
TextureCube g_IBLSpecularMapTexture : register(t11);

// IBL�g�U���˃}�b�v
SamplerState g_IBLDiffuseMapSampler : register(s12);
TextureCube g_IBLDiffuseMapTexture : register(t12);

// IBL BRDF�}�b�v
SamplerState g_IBLBRDFMapSampler : register(s13);
Texture2D g_IBLBRDFMapTexture : register(t13);

// IBL���ʔ��˃}�b�v��MIPMAP��
static const float u_mipmap_scale = 9;

// �~����
static const float PI = 3.1415926f;

// �m�����j�A���烊�j�A��Ԃɕϊ�
float3 ConvertToLinear(float3 color)
{
    return pow(color, 2.2f);
}

// ���j�A����m�����j�A��Ԃɕϊ�
float3 ConvertToNonLinear(float3 color)
{
    return pow(color, 1 / 2.2f);
}

// �X���[�X�l�X�����t�l�X�ɕϊ�
float SmoothnessToPerceptualRoughness(float smoothness)
{
    return (1.0f - smoothness);
}

// ���t�l�X���X���[�X�l�X�ɕϊ�
float PerceptualRoughnessToRoughness(float perceptualRoughness)
{
    return perceptualRoughness * perceptualRoughness;
}

// 5��
float Pow5(float x)
{
    return x * x * x * x * x;
}

// �t���l������
float3 F_Schlick(float cosA, float3 f0)
{
    return f0 + (1.0f - f0) * Pow5(1.0f - cosA);
}

// �}�C�N���t�@�Z�b�g���z�֐� Trowbridge-Reitz(GGX)
float D_GGX(float NoH, float linearRoughness)
{
    float a2 = linearRoughness * linearRoughness;
    float f = (NoH * a2 - NoH) * NoH + 1.0f;

    return a2 / (PI * f * f);
}

// �����֐�
float V_SmithGGXCorrelatedFast(float NoV, float NoL, float linearRoughness)
{
    float a = linearRoughness;
    float GGXV = NoL * (NoV * (1.0f - a) + a);
    float GGXL = NoV * (NoL * (1.0f - a) + a);

    return 0.5f / (GGXV + GGXL + 1e-5);
}

// ���ʔ���BRDF
float SpecularBRDF(float3 specularColor, float NoH, float NoV, float NoL, float LoH, float linearRoughness)
{
    float D = D_GGX(NoH, linearRoughness);
    float V = V_SmithGGXCorrelatedFast(NoV, NoL, linearRoughness);
    float3 F = F_Schlick(LoH, specularColor);

    return D * V * F;
}

// �g�U����
float Fd_Lambert()
{
    return 1.0f / PI;
}

// �g�U����BRDF
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor * Fd_Lambert();
}

// IBL�̌v�Z
float3 ImageBasedLighting(float3 N, float3 R, float NoV, float3 diffuseColor, float3 specularColor, float perceptualRoughness)
{
    float3 Ld = g_IBLDiffuseMapTexture.Sample(g_IBLDiffuseMapSampler, N).rgb * diffuseColor * Fd_Lambert();
    float3 Lld = g_IBLSpecularMapTexture.SampleLevel(g_IBLSpecularMapSampler, R, perceptualRoughness * u_mipmap_scale).rgb;
    float2 Ldfg = g_IBLBRDFMapTexture.Sample(g_IBLBRDFMapSampler, float2(NoV, 1.0 - perceptualRoughness)).xy;
    float3 Lr = (specularColor * Ldfg.x + Ldfg.y) * Lld;

    return Ld + Lr;
}

// ���C���֐�
float4 main(PS_INPUT PSInput) : SV_Target0
{
    // �A���x�h�J���[
    float3 albedoColor = g_AlbedoMapTexture.Sample(g_AlbedoMapSampler, PSInput.TexCoord.xy);
    albedoColor = ConvertToLinear(albedoColor);
    // ���^���b�N�A�X���[�X�l�X
    float4 metallicSmoothness = g_MetallicSmoothnessMapTexture.Sample(g_MetallicSmoothnessMapSampler, PSInput.TexCoord.xy);
    // �A���r�G���g�I�N���[�W����
    float4 ao = g_AoMapTexture.Sample(g_AoMapSampler, PSInput.TexCoord.xy);

    // �g�U���˃J���[���擾�i������j
    float3 diffuseColor = lerp(albedoColor.rgb, 0.0f, metallicSmoothness.r);
    // ���ʔ��˃J���[���擾�i�����j
    float3 specularColor = lerp(0.04f, albedoColor.rgb, metallicSmoothness.r);
    // ���t�l�X�ɕϊ�
    float perceptualRoughness = SmoothnessToPerceptualRoughness(metallicSmoothness.a);
    // ���j�A��Ԃ̃��t�l�X�ɕϊ�
    float linearRoughness = PerceptualRoughnessToRoughness(perceptualRoughness);

    // �e��x�N�g�������߂�
    float3x3 TBN = float3x3(normalize(PSInput.WTangent), normalize(PSInput.WBinormal), normalize(PSInput.WNormal));
    // �@���x�N�g��
    float3 N = mul(normalize(g_NormalMapTexture.Sample(g_NormalMapSampler, PSInput.TexCoord.xy).xyz * 2.0 - 1.0), TBN);

    float3 L = normalize(g_WorldLightPosition - PSInput.WPosition); // ���������̃x�N�g��
    float3 V = normalize(g_WorldViewPosition - PSInput.WPosition); // ���_�����̃x�N�g��
    float3 H = normalize(L + V); // 2�����x�N�g��
    float3 R = reflect(-V, N); // ���˃x�N�g��

    // �e��x�N�g���̓��ς����߂�
    float NoV = abs(dot(N, V));
    float NoL = saturate(dot(N, L));
    float NoH = saturate(dot(N, H));
    float LoV = saturate(dot(L, V));
    float LoH = saturate(dot(L, H));

    // ���ˏƓx���v�Z
    float3 irradiance = NoL * (g_LightColor.rgb * PI);

    // �o�������˗����z�֐��iBRDF�j�̌v�Z
    float3 Fd = DiffuseBRDF(diffuseColor);
    float3 Fr = SpecularBRDF(specularColor, NoH, NoV, NoL, LoH, linearRoughness);
    float3 BRDF = (Fd + Fr) * irradiance;
    // �C���[�W�x�[�X�h���C�e�B���O
    float3 IBL = ImageBasedLighting(N, R, NoV, diffuseColor, specularColor, perceptualRoughness);

    // �ŏI�J���[���v�Z
    float3 finalColor = (BRDF + IBL) * ao.rgb;
    finalColor = ConvertToNonLinear(finalColor);

    // �J���[���o��
    return float4(finalColor, 1.0f);
}