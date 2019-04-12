// ���_�V�F�[�_�[�̏o��
struct PS_INPUT
{
    float3 TexCoord : TEXCOORD0;    // �e�N�X�`�����W
};

SamplerState g_EnvMapSampler : register(s11); // ���}�b�v
TextureCube g_EnvMapTexture : register(t11); // ���}�b�v

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    return g_EnvMapTexture.Sample(g_EnvMapSampler, PSInput.TexCoord);
}