#ifndef ACTOR_GROUP_H_
#define ACTOR_GROUP_H_

// �񋓌^�F�A�N�^�[�O���[�v
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
enum class ActorGroup
{
	None,			// ��������
	Player,			// �v���C���[
	PlayerAttack,	// �v���C���[�̍U��
	Enemy,			// �G
	EnemyAttack,	// �G�̍U��
	Effect,			// �G�t�F�N�g
	UI,				// UI
};

#endif // !ACTOR_GROUP_H_