#include "PlayerAttack.h"
#include "EventMessage.h"
#include "Damage.h"

// �N���X�F�v���C���[�̍U���̓����蔻��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
PlayerAttack::PlayerAttack(IWorld* world, const Vector3& position, int power, int impact, const IBodyPtr& body) :
	Actor(world, "PlayerAttack", position, body),
	destroy_counter_{ 0.0f }
{
	power_ = power;		// �З͂�ݒ�
	impact_ = impact;	// ���ݒl��ݒ�
}

// �X�V
void PlayerAttack::update(float delta_time)
{
	// 0.2�b�����
	if (destroy_counter_ >= 12.0f)
	{
		die();
	}

	destroy_counter_ += delta_time;		// ���ŃJ�E���^�[�����Z
}

// �`��
void PlayerAttack::draw() const
{
	body_->transform(pose())->draw();	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
}

// �Փ˃��A�N�V����
void PlayerAttack::react(Actor& other)
{
	// �G�ւ̃_���[�W�\���̂𐶐�
	Damage damage{ power_, impact_ };
	// �G�փ_���[�W���b�Z�[�W�𑗂�
	other.handle_message(EventMessage::EnemyDamage, &damage);
	// �G�ɓ�����Ə���
	die();
}