#ifndef EVENT_MESSAGE_H_
#define EVENT_MESSAGE_H_

// �񋓌^�F�C�x���g���b�Z�[�W���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
enum class EventMessage
{
	None,				// �_�~�[
	PlayerDead,			// �v���C���[���S
	EnemyDead,			// �G���S
	BossDead,			// �{�X�G���S
	PlayerDamage,		// �v���C���[���_���[�W���󂯂�
	EnemyDamage,		// �G���_���[�W���󂯂�
	StageClear,			// �X�e�[�W�N���A
	GameOver,			// �Q�[���I�[�o�[
	HitStop,			// �q�b�g�X�g�b�v
	Camera_Vibration,	// �J�����U��
	EnemyDestroy,		// �S�G�L����������
};

#endif // !EVENT_MESSAGE_H_