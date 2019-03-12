#ifndef DAMAGE_H_
#define DAMAGE_H_

#include "../Math/Vector3.h"

// �_���[�W�\����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

struct Damage
{
	Vector3 position{ Vector3::Zero };	// �U������̈ʒu
	int power{ 0 };						// �U����
	int impact{ 0 };					// ���ݒl�i�G�ɑ΂��ėL���j
};

#endif // !DAMAGE_H_