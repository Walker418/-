#include "Player.h"

// �N���X�F�v���C���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Player::Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE }
{
	rotation_ = rotation;
	velocity_ = Vector3::Zero;
	state_ = PlayerState::Normal;
	state_timer_ = 0.0f;
}

// �X�V
void Player::update(float delta_time)
{
	// �v���[���[�̏�Ԃ��X�V
	update_state(delta_time);
	// ���[�V������ύX
	mesh_.change_motion(motion_);
	// ���b�V�����X�V
	mesh_.update(delta_time);
	// �s����v�Z
	mesh_.transform(pose());
}

// �`��
void Player::draw() const
{
	mesh_.draw();	// ���b�V����`��
	body_->draw();	// �R���C�_�[��`��i�����p�j
}

// �Փ˃��A�N�V����
void Player::react(Actor & other)
{

}

// ���b�Z�[�W����
void Player::handle_message(EventMessage message, void* param)
{

}

// ��Ԃ̍X�V
void Player::update_state(float delta_time)
{
	// ��ԑJ��
	switch (state_)
	{
	case Player::PlayerState::Normal:
		normal_update(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// ��ԃ^�C�}�[�̉��Z
}

// ��Ԃ̕ύX
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;
}

// �ʏ��Ԃł̍X�V
void Player::normal_update(float delta_time)
{

}

// �U����Ԃł̍X�V
void Player::attack_update(float delta_time)
{

}

// �K�[�h��Ԃł̍X�V
void Player::guard_update(float delta_time)
{

}

// ��e��Ԃł̍X�V
void Player::damage_update(float delta_time)
{

}

// ���S��Ԃł̍X�V
void Player::dead_update(float delta_time)
{

}