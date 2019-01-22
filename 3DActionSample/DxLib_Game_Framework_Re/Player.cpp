#include "Player.h"
#include "GamePad.h"
#include "MathHelper.h"
#include "IWorld.h"
#include "EventMessage.h"
#include <DxLib.h>

// �N���X�F�v���C���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Player::Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	state_timer_{ 0.0f }
{
	rotation_ = rotation;
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
}

// �X�V
void Player::update(float delta_time)
{
	/*
	// ���[�V�����̐؂�ւ��e�X�g
	if (GamePad::trigger(GamePad::Up))
		motion_ = (motion_ + 1) % 25;
	if (GamePad::trigger(GamePad::Down))
		motion_ = ((motion_ - 1) + 25) % 25;

	// ���[�V������ύX
	mesh_.change_motion(motion_);
	// �A�j���[�V�������X�V
	mesh_.update(delta_time);
	// �s���ݒ�
	mesh_.transform(pose());
	*/

	// �v���[���[�̏�Ԃ��X�V
	update_state(delta_time);
	// ���[�V������ύX
	mesh_.change_motion(motion_);
	// ���b�V�����X�V
	mesh_.update(delta_time);
	// �s����v�Z
	mesh_.transform(pose());

	// HP��0�ȉ��ɂȂ�ƁA���S��ԂɈڍs
	if (current_hp_ <= 0 && state_ != PlayerState::Death)
	{
		change_state(PlayerState::Death, MOTION_DEATH);
	}
}

// �`��
void Player::draw() const
{
	mesh_.draw();							// ���b�V����`��
	body_->translate(position_)->draw();	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
}

// �Փ˃��A�N�V����
void Player::react(Actor& other)
{

}

// ���b�Z�[�W����
void Player::handle_message(EventMessage message, void* param)
{

}

// ��Ԃ̍X�V
void Player::update_state(float delta_time)
{
	// ���݂̏�Ԃɉ����āA�v���C���[���X�V
	switch (state_)
	{
	case PlayerState::Normal:
		normal(delta_time);
		break;
	case PlayerState::Slash1:
		slash1(delta_time);
		break;
	case PlayerState::Slash2:
		slash2(delta_time);
		break;
	case PlayerState::Slash3:
		slash3(delta_time);
		break;
	case PlayerState::Damage:
		damage(delta_time);
		break;
	case PlayerState::Guard:
		guard(delta_time);
		break;
	case PlayerState::Blocking:
		blocking(delta_time);
		break;
	case PlayerState::GuardAttack:
		guard_attack(delta_time);
		break;
	case PlayerState::GuardEnd:
		guard_end(delta_time);
		break;
	case PlayerState::Death:
		death(delta_time);
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
void Player::normal(float delta_time)
{
	// X�L�[���������ƁA�U������
	if (GamePad::trigger(GamePad::X))
	{
		// �U����ԂɈڍs
		change_state(PlayerState::Slash1, MOTION_SLASH_1);

		return;
	}

	// R�L�[���������ƁA�K�[�h��ԂɈڍs
	if (GamePad::state(GamePad::R))
	{
		change_state(PlayerState::Guard, MOTION_GUARD_BEGIN);

		return;
	}

	/*
	// ���S�e�X�g
	if (GamePad::trigger(GamePad::R))
	{
		change_state(PlayerState::Death, MOTION_DEATH);

		return;
	}
	*/

	// ============================================================
	// �ȉ��͈ړ�����
	int motion{ MOTION_IDLE };		// �������Ȃ���΁A�ҋ@���[�V�����ɕύX
	velocity_ = Vector3::Zero;		// �ړ��ʂ����Z�b�g
	float forward_speed{ 0.0f };	// �O�������x
	float left_speed{ 0.0f };		// ���������x

	// �O��ړ�
	if (GamePad::state(GamePad::Up))
	{
		forward_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Down))
	{
		forward_speed = -WalkSpeed;
	}
	// ���E�ړ�
	if (GamePad::state(GamePad::Left))
	{
		left_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Right))
	{
		left_speed = -WalkSpeed;
	}
	// �ړ����Ă���΁A���s���[�V�����ɕύX
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_WALK;

	// ��Ԃ��X�V
	change_state(PlayerState::Normal, motion);
	// �ړ��ʂ��v�Z���A�v���C���[���ړ�������
	velocity_ += rotation_.Forward() * forward_speed;
	velocity_ += rotation_.Left() * left_speed;
	position_ += velocity_ * delta_time;

	// �ړ������I��
	// ============================================================
}

// �U���i1�i�ځj�ł̍X�V
void Player::slash1(float delta_time)
{
	// ���[�V�����I���̑O�ɁAX�L�[���������ƁA�U����2�i�K�ڂɈڍs
	if (GamePad::trigger(GamePad::X))
	{
		if (state_timer_ >= mesh_.motion_end_time() + 5.5f && state_timer_ < mesh_.motion_end_time() + 18.0f)
		{
			change_state(PlayerState::Slash2, MOTION_SLASH_2);
			return;
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		normal(delta_time);
	}
}

// �U���i2�i�ځj�ł̍X�V
void Player::slash2(float delta_time)
{
	// ���[�V�����I���̑O�ɁAX�L�[���������ƁA�U����3�i�K�ڂɈڍs
	if (GamePad::trigger(GamePad::X))
	{
		if (state_timer_ >= mesh_.motion_end_time() + 5.0f && state_timer_ < mesh_.motion_end_time() + 18.0f)
		{
			change_state(PlayerState::Slash3, MOTION_SLASH_3);
			return;
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() + 15.0f)
	{
		normal(delta_time);
	}
}

// �U���i3�i�ځj�ł̍X�V
void Player::slash3(float delta_time)
{
	// ���[�V�����Đ��̊ԁA�L�����N�^�[��O�i������
	if (state_timer_ <= mesh_.motion_end_time() + 28.0f)
	{
		velocity_ = rotation_.Forward() * 0.15f;
		position_ += velocity_ * delta_time;
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() + 45.0f)
	{
		normal(delta_time);
	}
}

// ��e��Ԃł̍X�V
void Player::damage(float delta_time)
{
	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time())
	{
		normal(delta_time);
	}
}

// �K�[�h���̍X�V
void Player::guard(float delta_time)
{
	// �K�[�h�J�n�̃��[�V�����I����A�K�[�h�ҋ@���[�V�������Đ����A
	// �ȍ~R�L�[�����ꂽ��A�K�[�h�I����ԂɈڍs
	float guard_ready_time = mesh_.motion_end_time() * 2.0f;	// �K�[�h�J�n�܂ł̎���
	if (state_timer_ >= guard_ready_time)
	{
		mesh_.change_motion(MOTION_GUARD_IDLE);

		// R�L�[�����ꂽ��A�K�[�h�I����ԂɈڍs
		if (!GamePad::state(GamePad::R))
		{
			change_state(PlayerState::GuardEnd, MOTION_GUARD_END);
		}
	}
}

// �K�[�h�ɂ��m�b�N�o�b�N���̍X�V
void Player::blocking(float delta_time)
{

}

// �K�[�h�U���ł̍X�V
void Player::guard_attack(float delta_time)
{
	// ���[�V�����I����A�K�[�h��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time())
	{
		guard(delta_time);
	}
}

// �K�[�h�I�����̍X�V
void Player::guard_end(float delta_time)
{
	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() + 10.0f)
	{
		normal(delta_time);
	}
}

// ���S��Ԃł̍X�V
void Player::death(float delta_time)
{
	// ���[�V�������I������ƁA���S�����L����
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		world_->send_message(EventMessage::PlayerDead);
		die();
	}
}