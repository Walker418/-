#ifndef PLAYER_STATE_ID_H_
#define PLAYER_STATE_ID_H_

// �񋓌^�F�v���C���[�̏��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
enum class PlayerState
{
	Normal,			// �ʏ�
	Slash1,			// �U���i1�i�ځj
	Slash2,			// �U���i2�i�ځj
	Slash3,			// �U���i3�i�ځj
	JumpAttack1,	// �W�����v�U���i1�i�ځj
	JumpAttack2,	// �W�����v�U���i2�i�ځj
	Damage,			// ����
	Guard,			// �K�[�h
	Blocking,		// �K�[�h�ɂ��m�b�N�o�b�N
	GuardAttack,	// �K�[�h�U��
	GuardEnd,		// �K�[�h�I��
	Death,			// ���S
	ForwardEvasion,	// �O���
	LeftEvasion,	// �����
	RightEvasion,	// �E���
};

#endif // !PLAYER_STATE_ID_H_