#include "EnemyAttack.h"
#include "../../ID/EventMessage.h"
#include "../Damage.h"

// �N���X�F�G�̍U���̓����蔻��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
EnemyAttack::EnemyAttack(IWorld* world, const Vector3& position, int power, const IBodyPtr& body) :
	Actor(world, "EnemyAttack", position, body),
	destroy_counter_{ 0.0f }
{
	power_ = power;		// �З͂�ݒ�
}

// �X�V
void EnemyAttack::update(float delta_time)
{
	// 0.2�b�����
	if (destroy_counter_ >= 12.0f)
	{
		die();
	}

	destroy_counter_ += delta_time;		// ���ŃJ�E���^�[�����Z
}

// �`��
void EnemyAttack::draw() const
{
	body_->transform(pose())->draw();	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
}

// �Փ˃��A�N�V����
void EnemyAttack::react(Actor& other)
{
	// �v���C���[�ւ̃_���[�W�\���̂𐶐�
	Damage damage = { position_, power_ };
	// �v���C���[�փ_���[�W���b�Z�[�W�𑗂�
	other.handle_message(EventMessage::PlayerDamage, &damage);
	// �v���C���[�ɓ�����Ə���
	die();
}