#ifndef EVENT_MESSAGE_H_
#define EVENT_MESSAGE_H_

// �񋓌^�F�C�x���g���b�Z�[�W���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
enum class EventMessage
{
	None,			// �_�~�[
	PlayerDead,		// �v���C���[���S
	EnemyDead,		// �G���S
	BossDead,		// �{�X�G���S
	PlayerDamage,	// �v���C���[���_���[�W���󂯂�
	EnemyDamage,	// �G���_���[�W���󂯂�
	StageClear,		// �X�e�[�W�N���A
	GameOver,		// �Q�[���I�[�o�[
};

#endif // !EVENT_MESSAGE_H_