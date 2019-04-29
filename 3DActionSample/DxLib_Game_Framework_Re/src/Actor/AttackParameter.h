#ifndef ATTACK_PARAMETER_H_
#define ATTACK_PARAMETER_H_

#include "../Math/Vector3.h"

// �\���́F�U���p�����[�^
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
struct AttackParameter
{
	Vector3	position{ Vector3::Zero };	// �R���C�_�[�̈ʒu
	int		power{ 0 };					// �U����
	int		impact{ 0 };				// ���ݒl�i�v���C���[�̂ݎg�p�j
	float	hit_stop{ 0.0f };			// �q�b�g�X�g�b�v�i�v���C���[�̂ݎg�p�j
};

#endif // !ATTACK_PARAMETER_H_