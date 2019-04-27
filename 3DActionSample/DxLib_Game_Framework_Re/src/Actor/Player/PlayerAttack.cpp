#include "PlayerAttack.h"
#include "../../ID/EventMessage.h"
#include "../Damage.h"
#include "../../World/IWorld.h"

// �N���X�F�v���C���[�̍U���̓����蔻��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
PlayerAttack::PlayerAttack(IWorld* world, const Vector3& position, int power, int impact, float hit_stop, const IBodyPtr& body) :
	Actor(world, "PlayerAttack", position, body),
	destroy_counter_{ 0.0f }
{
	power_ = power;			// �З͂�ݒ�
	impact_ = impact;		// ���ݒl��ݒ�
	hit_stop_ = hit_stop;	// �q�b�g�X�g�b�v��ݒ�
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
	Damage damage{ position_, power_, impact_ };
	// �G�փ_���[�W���b�Z�[�W�𑗂�
	other.handle_message(EventMessage::EnemyDamage, &damage);
	// �v���C���[�Ƀq�b�g�X�g�b�v���b�Z�[�W�𑗂�
	world_->send_message(EventMessage::HitStop, &hit_stop_);
	// �G�ɓ�����Ə���
	die();
}