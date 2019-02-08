#include "PlayerAttack.h"

// �N���X�F�v���C���[�̍U������
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
PlayerAttack::PlayerAttack(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "PlayerAttack", position, body),
	destroy_counter_{ 0.0f }
{
	rotation_ = rotation;
}

// �X�V
void PlayerAttack::update(float delta_time)
{
	if (destroy_counter_ >= 30.0f)
	{
		die();
	}

	destroy_counter_ += delta_time;
}

// �`��
void PlayerAttack::draw() const
{
	body_->transform(pose())->draw();	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
}

// �Փ˃��A�N�V����
void PlayerAttack::react(Actor& other)
{

}