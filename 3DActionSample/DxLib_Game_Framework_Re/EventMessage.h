#ifndef EVENT_MESSAGE_H_
#define EVENT_MESSAGE_H_

// �񋓌^�F�C�x���g���b�Z�[�W���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
enum class EventMessage
{
	None,			// �_�~�[
	PlayerDead,		// �v���C���[���S
	EnemyDead,		// �G���S
	PlayerDamage,	// �v���C���[���_���[�W���󂯂�
	EnemyDamage,	// �G���_���[�W���󂯂�
};

#endif // !EVENT_MESSAGE_H_