#ifndef PLAYER_PARAMETER_H_
#define PLAYER_PARAMETER_H_

// �v���C���[�̃p�����[�^
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
namespace PlayerParameter
{
	const int	HP{ 100 };					// �ő�̗�
	const float	WalkSpeed{ 0.3f };			// ���s���x
	const float	DashSpeed{ 0.8f };			// �_�b�V�����x
	const float	RotateSpeed{ 0.3f };		// ��]���x
	const float	Gravity{ 0.03f };			// �������x

	const int	Power_Atk1{ 3 };			// �U��1�i�ڂ̈З�
	const int	Power_Atk2{ 2 };			// �U��2�i�ڂ̈З�
	const int	Power_Atk3{ 5 };			// �U��3�i�ڂ̈З�
	const int	Power_JumpAtk1{ 5 };		// �W�����v�U��1�i�ڂ̈З�
	const int	Power_JumpAtk2{ 2 };		// �W�����v�U��2�i�ڂ̈З�
	const int	Power_GuardAtk{ 3 };		// �K�[�h�U���̈З�

	const int	Wince_Atk1{ 1 };			// �U��1�i�ڂ̋��ݒl
	const int	Wince_Atk2{ 1 };			// �U��2�i�ڂ̋��ݒl
	const int	Wince_Atk3{ 3 };			// �U��3�i�ڂ̋��ݒl
	const int	Wince_JumpAtk1{ 3 };		// �W�����v�U��1�i�ڂ̋��ݒl
	const int	Wince_JumpAtk2{ 1 };		// �W�����v�U��1�i�ڂ̋��ݒl
	const int	Wince_GuardAtk{ 1 };		// �K�[�h�U���̋��ݒl

	// �q�b�g�X�g�b�v
	const float HitStop_Short{ 6.0f };		// �Z��
	const float HitStop_Long{ 10.0f };		// ����

	const float EvasionTime{ 30.0f };		// �����Ԉێ��t���[����
	const float InvincibleTime{ 12.0f };	// ���G��Ԉێ��t���[����
	const float EvasionSpeed{ 1.2f };		// ������̈ړ����x
};

#endif // !PLAYER_PARAMETER_H_