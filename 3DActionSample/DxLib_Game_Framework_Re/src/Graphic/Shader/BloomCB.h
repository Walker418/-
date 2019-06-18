#ifndef BLOOM_CB_H_
#define BLOOM_CB_H_

// �u���[���V�F�[�_�[�p�萔�o�b�t�@
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

struct BloomCB
{
	// �P�x��臒l
	float g_BrightPassThreshold{ 1.0f };
	// ���摜�̍ʓx
	float g_BaseSaturation{ 1.0f };
	// ���摜�̋P�x
	float g_BaseIntensity{ 1.0f };
	// �u���[���摜�̍ʓx
	float g_BloomSaturation{ 1.0f };
	// �u���[���摜�̋P�x
	float g_BloomIntensity{ 1.0f };
	// �p�f�B���O1
	float padding1[3]{ 0.0f };
	// �K�E�V�A���u���[�̃E�F�C�g�l
	float g_GaussianWeights[5]{ 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };
	// �p�f�B���O2
	float padding2[3]{ 0.0f };
};

#endif // !BLOOM_CB_H_