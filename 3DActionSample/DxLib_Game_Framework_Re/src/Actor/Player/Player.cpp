#include "Player.h"
#include "../../Math/MathHelper.h"
#include "../../Math/Quaternion.h"
#include "../../World/IWorld.h"
#include "../../ID/EventMessage.h"
#include "../../Field/Field.h"
#include "../Body/Line.h"
#include "../Body/BoundingSphere.h"
#include "../ActorGroup.h"
#include "PlayerAttack.h"
#include "../Damage.h"
#include "PlayerInput.h"

// �N���X�F�v���C���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Player::Player(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	state_timer_{ 0.0f },
	is_ground_{ false },
	is_guard_{ false },
	attack_on_{ false },
	jump_attack_started_{ false },
	invincible_timer_{ 0.0f },
	skip_timer_{ 0.0f }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
}

// �X�V
void Player::update(float delta_time)
{
	// ��������
	velocity_ += Vector3::Down * Gravity;		// �d�͉����x���v�Z
	position_.y += velocity_.y * delta_time;	// y�����W���v�Z
	// �n�ʂƂ̐ڐG����
	intersect_ground();
	// �ǂƂ̐ڐG����
	intersect_wall();

	// �K�[�h��Ԃ����Z�b�g
	is_guard_ = false;

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
		return;
	}

	// ���G���ԃ^�C�}�[���X�V
	if (invincible_timer_ > 0.0f)	invincible_timer_ -= delta_time;
}

// �`��
void Player::draw() const
{
	mesh_.draw();	// ���b�V����`��

	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
	body_->transform(pose())->draw();
	// �����ŕ����������i�f�o�b�O���[�h�̂݁j
	unsigned int Cr;
	Cr = GetColor(255, 0, 0);
	DrawLine3D(position_, position_ + pose().Forward() * 10.0f, Cr);
}

// �Փ˃��A�N�V����
void Player::react(Actor& other)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == PlayerState::Death) return;
}

// ���b�Z�[�W����
void Player::handle_message(EventMessage message, void* param)
{
	// ���G���Ԓ��A���݂⎀�S��Ԃł͔������Ȃ�
	if (is_invincible() || state_ == PlayerState::Death) return;

	// �G����_���[�W���󂯂�
	if (message == EventMessage::PlayerDamage)
	{
		// ���b�Z�[�W����G�̍U�����擾
		Damage* damage = (Damage*)param;
		Vector3 atk_pos = damage->position;

		// �K�[�h����
		if (is_guard_ && can_block(atk_pos))
		{
			// �U������̈ʒu���v���C���[���O�̏ꍇ�A�K�[�h����������
			change_state(PlayerState::Blocking, MOTION_GUARD_BLOCK);
			return;
		}

		// �K�[�h���������Ȃ��ꍇ�A�_���[�W���v�Z����
		current_hp_ -= damage->power;						// �_���[�W�v�Z
		// �X�[�p�[�A�[�}�[��ԂłȂ���΁A���ݏ�Ԃֈڍs
		if (!is_super_armor())
		{
			change_state(PlayerState::Damage, MOTION_IMPACT);
		}
	}
}

// �̗͂̎擾
int Player::get_HP()
{
	return current_hp_;
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
	case PlayerState::JumpAttack1:
		jump_attack1(delta_time);
		break;
	case PlayerState::JumpAttack2:
		jump_attack2(delta_time);
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
	case PlayerState::ForwardEvasion:
		forward_evasion(delta_time);
		break;
	case PlayerState::LeftEvasion:
		left_evasion(delta_time);
		break;
	case PlayerState::RightEvasion:
		right_evasion(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// ��ԃ^�C�}�[�����Z
}

// ��Ԃ̕ύX
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
	jump_attack_started_ = false;

	if (state == PlayerState::Normal)	mesh_.reset_speed();	// �ʏ��Ԃɖ߂�Ƃ��A�K�����[�V�������x�����Z�b�g
}

// �ʏ��Ԃł̍X�V
void Player::normal(float delta_time)
{
	// �U���A�K�[�h�̍s���͐ڒn��Ԃł����ڍs�ł��Ȃ�
	if (is_ground_)
	{
		// �W�����v�U����Ԃւ̈ڍs
		if (PlayerInput::jump_attack())
		{
			mesh_.change_speed(1.5f);	// ���̃��[�V�������x��ݒ�
			change_state(PlayerState::JumpAttack1, MOTION_JUMP_ATTACK);
			return;
		}

		// �U����Ԃւ̈ڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// ���̃��[�V�������x��ݒ�
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}

		// �K�[�h��Ԃւ̈ڍs
		if (PlayerInput::guard())
		{
			mesh_.change_speed(1.5f);	// ���̃��[�V�������x��ݒ�
			change_state(PlayerState::Guard, MOTION_GUARD_BEGIN);
			return;
		}
	}

	// ============================================================
	// �ȉ��͈ړ�����
	int motion{ MOTION_IDLE };		// �������Ȃ���΁A�ҋ@���[�V�����ɕύX

	// �J�������擾
	auto camera = world_->camera()->pose();
	// �J�����̐��ʃx�N�g�����擾
	auto camera_forward = camera.Forward();
	// �J������y�������𖳎�����
	camera_forward.y = 0;
	// ���K��
	camera_forward.Normalize();

	// x,z���ړ��ʂ����Z�b�g
	velocity_.x = 0;
	velocity_.z = 0;
	float forward_speed{ 0.0f };	// �O�������x
	float left_speed{ 0.0f };		// ���������x

	// �ړ��͐ڒn��Ԃł����ł��Ȃ�
	if (is_ground_)
	{
		// WASD�ɂ��ړ�
		// �O��ړ�
		if (PlayerInput::move_forward())		// �O
		{
			forward_speed = DashSpeed;
		}
		else if (PlayerInput::move_backward())	// ��
		{
			forward_speed = -DashSpeed;
		}
		// ���E�ړ�
		if (PlayerInput::move_left())			// ��
		{
			left_speed = DashSpeed;
		}
		else if (PlayerInput::move_right())		// �E
		{
			left_speed = -DashSpeed;
		}

		// ���X�e�B�b�N�ɂ��ړ��iWASD���삵�Ă��Ȃ���ΗL���j
		if (!PlayerInput::keyboard_move())
		{
			forward_speed = DashSpeed * PlayerInput::L_stick_move().y;	// �O��
			left_speed = -DashSpeed * PlayerInput::L_stick_move().x;	// ���E
		}
	}
	// �ړ����Ă���΁A���s���[�V�����ɕύX
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_DASH;

	// ��Ԃ��X�V
	change_state(PlayerState::Normal, motion);

	// �ړ��ʂ��v�Z���A�v���C���[���ړ�������
	velocity_ += camera_forward * forward_speed;	// �O�㑬�x�����Z
	velocity_ += camera.Left() * left_speed;		// ���E���x�����Z
	position_ += velocity_ * delta_time;			// ���̈ʒu���v�Z

	// �v���C���[����]������
	if (velocity_.x != 0.0f || velocity_.z != 0.0f)		// �ړ����Ă����
	{
		// rotation_ = Matrix::CreateWorld(Vector3::Zero, Vector3(velocity_.x, 0.0f, velocity_.z).Normalize(), Vector3::Up);

		Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, Vector3(velocity_.x, 0.0f, velocity_.z).Normalize(), Vector3::Up);	// �V����������ݒ�
		rotation_ = Matrix::Lerp(rotation_, new_rotation, RotateSpeed);	// ��Ԃŕ�����]������
	}

	// �ړ������I��
	// ============================================================

	// ���
	if (PlayerInput::evasion() && is_ground_)
	{
		ready_to_skip();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// �U���i1�i�ځj�ł̍X�V
void Player::slash1(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= 25.0f && !attack_on_)
	{

		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 12.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 3, 1));
		mesh_.change_speed(1.4f);	// �ȍ~�̃��[�V�������x�������x���ɂ���
	}

	// ���[�V�����I���̑O�ɁA���̍U���ւ̈ڍs
	if (state_timer_ > 35.0f && state_timer_ <= 45.0f && is_ground_)
	{
		// �U�����͂����ƁA�U����2�i�K�ڂɈڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash2, MOTION_SLASH_2);
			return;
		}
	}

	// ���[�V�����I���̑O�ɁA����ւ̈ڍs
	if (state_timer_ > 33.0f && state_timer_ < 53.0f && is_ground_)
	{
		// ����+�����͂����ƁA�����ԂɈڍs
		// �L�[�{�[�h����ɂ�����
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// �p�b�h����ɂ�����
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= 60.0f)
	{
		normal(delta_time);
	}
}

// �U���i2�i�ځj�ł̍X�V
void Player::slash2(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= 10.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 12.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 2, 1));
		mesh_.reset_speed();	// �ȍ~�̃��[�V�������x�������x���ɂ���
	}

	// ���[�V�����I���̑O�ɁA���̍U���ւ̈ڍs
	if (state_timer_ > 31.0f && state_timer_ < 45.0f && is_ground_)
	{
		// �U�����͂����ƁA�U����3�i�K�ڂɈڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash3, MOTION_SLASH_3);
			return;
		}
	}

	// ���[�V�����I���̑O�ɁA����ւ̈ڍs
	if (state_timer_ > 20.0f && state_timer_ < 45.0f && is_ground_)
	{
		// ����+�����͂����ƁA�����ԂɈڍs
		// �L�[�{�[�h����ɂ�����
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// �p�b�h����ɂ�����
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= 45.0f)
	{
		normal(delta_time);
	}
}

// �U���i3�i�ځj�ł̍X�V
void Player::slash3(float delta_time)
{
	// �n�ʂɗ��ꂽ��A�ʏ��Ԃɖ߂�
	if (!is_ground_)
	{
		normal(delta_time);
	}

	// ���[�V�����Đ��̊ԁA�L�����N�^�[��O�i������
	if (state_timer_ <= 69.0f)
	{
		velocity_ = rotation_.Forward() * 0.22f;
		position_ += velocity_ * delta_time;
	}

	// �U������𔭐�
	if (state_timer_ >= 43.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 15.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 5, 3));
	}

	// ���[�V�����I���̑O�ɁA����ւ̈ڍs
	if (state_timer_ > 50.0f && state_timer_ <= mesh_.motion_end_time() + 70.0f && is_ground_)
	{
		// ����+�����͂����ƁA�����ԂɈڍs
		// �L�[�{�[�h����ɂ�����
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// �p�b�h����ɂ�����
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= 72.0f)
	{
		normal(delta_time);
	}
}

// �W�����v�U���i1�i�ځj�ł̍X�V
void Player::jump_attack1(float delta_time)
{
	// ���[�V�����Đ��̊ԁA�L�����N�^�[��O�i������
	if (state_timer_ < 45.0f)
	{
		velocity_ = rotation_.Forward() * 0.7f;
		position_ += velocity_ * delta_time;
	}

	// �U������𔭐�
	if (state_timer_ >= 45.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 13.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 5, 3));
	}

	// ���[�V�����I���̑O�ɁA���̍U���ւ̈ڍs
	if (state_timer_ > 50.0f && state_timer_ < 65.0f && is_ground_)
	{
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::JumpAttack2, MOTION_SLASH_2);
			return;
		}
	}

	// ���[�V�����I���̑O�ɁA����ւ̈ڍs
	if (state_timer_ > 46.0f && state_timer_ < mesh_.motion_end_time() + 70.0f && is_ground_)
	{
		// ����+�����͂����ƁA�����ԂɈڍs
		// �L�[�{�[�h����ɂ�����
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// �p�b�h����ɂ�����
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= 70.0f)
	{
		normal(delta_time);
	}
}

// �W�����v�U���i2�i�ځj�ł̍X�V
void Player::jump_attack2(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= 10.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 12.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 2, 1));
		mesh_.reset_speed();	// �ȍ~�̃��[�V�������x�������x���ɂ���
	}

	// ���[�V�����I���̑O�ɁA���̍U���ւ̈ڍs
	if (state_timer_ > 25.0f && state_timer_ < 43.0f && is_ground_)
	{
		// �U�����͂����ƁA�U����1�i�K�ڂɈڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// ���̃��[�V�������x��ݒ�
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}
	}

	// ���[�V�����I���̑O�ɁA����ւ̈ڍs
	if (state_timer_ > 20.0f && state_timer_ < 45.0f && is_ground_)
	{
		// ����+�����͂����ƁA�����ԂɈڍs
		// �L�[�{�[�h����ɂ�����
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// �p�b�h����ɂ�����
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= 45.0f)
	{
		normal(delta_time);
	}
}

// ��e��Ԃł̍X�V
void Player::damage(float delta_time)
{
	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		normal(delta_time);
	}
}

// �K�[�h���̍X�V
void Player::guard(float delta_time)
{
	// �K�[�h����̔����̓��[�V�����̍Đ���葁���i0.1�b�ŗL���j
	if (state_timer_ >= 6.0f)
	{
		is_guard_ = true;				// �K�[�h�����L����
	}

	// �K�[�h�U���ւ̈ڍs
	if (PlayerInput::attack())
	{
		mesh_.change_speed(1.5f);
		change_state(PlayerState::GuardAttack, MOTION_GUARD_SLASH);
		return;
	}

	// �K�[�h�J�n�̃��[�V�����I����A�K�[�h�ҋ@���[�V�����Ɉڍs
	if (state_timer_ >= mesh_.motion_end_time())
	{
		mesh_.reset_speed();
		motion_ = MOTION_GUARD_IDLE;
	}

	// R�L�[�����ꂽ��A�K�[�h�I����ԂɈڍs
	if (PlayerInput::guard_end())
	{
		mesh_.reset_speed();
		change_state(PlayerState::GuardEnd, MOTION_GUARD_END);
	}
}

// �K�[�h�ɂ��m�b�N�o�b�N���̍X�V
void Player::blocking(float delta_time)
{
	is_guard_ = true;				// �K�[�h��Ԃ�L����

	// ���[�V�����I����A�K�[�h��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{

		state_ = PlayerState::Guard;
		guard(delta_time);
	}
}

// �K�[�h�U���ł̍X�V
void Player::guard_attack(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= 35.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 15.0f + Vector3(0.0f, 9.5f, 0.0f);
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, attack_position, 3, 1));
	}

	// ���[�V�����I���̑O�ɁA���̍U���ւ̈ڍs
	if (state_timer_ > 45.0f && state_timer_ < 60.0f)
	{
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// ���̃��[�V�������x��ݒ�
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}
	}

	// ���[�V�����I���̑O�ɁA����ւ̈ڍs
	if (state_timer_ > 45.0f && state_timer_ < 60.0f && is_ground_)
	{
		// ����+�����͂����ƁA�����ԂɈڍs
		// �L�[�{�[�h����ɂ�����
		if (!PlayerInput::gamepad_move() && PlayerInput::evasion())
		{
			attack_to_evasion_keyboard();
		}

		// �p�b�h����ɂ�����
		if (!PlayerInput::keyboard_move() && PlayerInput::evasion())
		{
			attack_to_evasion_pad();
		}
	}

	// �ʏ��Ԃւ̈ڍs
	if (state_timer_ >= mesh_.motion_end_time() + 15.0f)
	{
		normal(delta_time);
	}
}

// �K�[�h�I�����̍X�V
void Player::guard_end(float delta_time)
{
	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() + 4.0f)
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

// �O�����Ԃł̍X�V
void Player::forward_evasion(float delta_time)
{
	// �n�ʂɗ��ꂽ��A�ʏ��Ԃɖ߂�
	if (!is_ground_)
	{
		invincible_timer_ = 0.0f;	// ���G���Ԃ������I��
		normal(delta_time);
	}

	// �v���C���[�̍��W���ړ�
	velocity_ = rotation_.Forward() * 1.2f;
	position_ += velocity_ * delta_time;

	// ����I����A�ʏ��Ԃɖ߂�
	if (skip_timer_ <= 0.0f)
	{
		normal(delta_time);
	}

	// ����^�C�}�[�̍X�V
	skip_timer_ -= delta_time;
}

// �������Ԃł̍X�V
void Player::left_evasion(float delta_time)
{
	// �n�ʂɗ��ꂽ��A�ʏ��Ԃɖ߂�
	if (!is_ground_)
	{
		invincible_timer_ = 0.0f;	// ���G���Ԃ������I��
		normal(delta_time);
	}

	// �v���C���[�̍��W���ړ�
	velocity_ = rotation_.Left() * 1.2f;
	position_ += velocity_ * delta_time;

	// ����I����A�ʏ��Ԃɖ߂�
	if (skip_timer_ <= 0.0f)
	{
		normal(delta_time);
	}

	// ����^�C�}�[�̍X�V
	skip_timer_ -= delta_time;
}

// �E�����Ԃł̍X�V
void Player::right_evasion(float delta_time)
{
	// �n�ʂɗ��ꂽ��A�ʏ��Ԃɖ߂�
	if (!is_ground_)
	{
		invincible_timer_ = 0.0f;	// ���G���Ԃ������I��
		normal(delta_time);
	}

	// �v���C���[�̍��W���ړ�
	velocity_ = rotation_.Right() * 1.2f;
	position_ += velocity_ * delta_time;

	// ����I����A�ʏ��Ԃɖ߂�
	if (skip_timer_ <= 0.0f)
	{
		normal(delta_time);
	}

	// ����^�C�}�[�̍X�V
	skip_timer_ -= delta_time;
}

// �U����̉���s���ڍs�i�Q�[���p�b�h�j
void Player::attack_to_evasion_pad()
{
	// �v���C���[�̕������͂��擾
	auto input = PlayerInput::L_stick_move();

	// �J�������擾
	auto camera = world_->camera()->pose();
	// �J�����̐��ʃx�N�g�����擾
	auto camera_forward = camera.Forward();
	// �J������y�������𖳎�����
	camera_forward.y = 0;
	// ���K��
	camera_forward.Normalize();

	// ���͂����������A�J��������̃x�N�g���ɕϊ�
	Vector3 direction = Vector3::Zero;
	direction += camera_forward * input.y;
	direction += camera.Left() * -input.x;
	direction.Normalize();

	// ���͂��������ƃv���C���[�̕����x�N�g���̍������Ȃ�������A����s���Ɉڂ�
	// �����
	if (Vector3::Angle(rotation_.Left(), direction) <= 45.0f)
	{
		ready_to_skip();
		change_state(PlayerState::LeftEvasion, PlayerMotion::MOTION_STRAFE_LEFT);
		return;
	}
	// �E���
	else if (Vector3::Angle(rotation_.Right(), direction) <= 45.0f)
	{
		ready_to_skip();
		change_state(PlayerState::RightEvasion, PlayerMotion::MOTION_STRAFE_RIGHT);
		return;
	}
	// �O���
	else
	{
		ready_to_skip();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// �U����̉���s���ڍs�i�L�[�{�[�h�j
void Player::attack_to_evasion_keyboard()
{
	// �����
	if (PlayerInput::move_left())
	{
		ready_to_skip();
		change_state(PlayerState::LeftEvasion, PlayerMotion::MOTION_STRAFE_LEFT);
		return;
	}
	// �E���
	else if (PlayerInput::move_right())
	{
		ready_to_skip();
		change_state(PlayerState::RightEvasion, PlayerMotion::MOTION_STRAFE_RIGHT);
		return;
	}
	// �O���
	else
	{
		ready_to_skip();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// �n�ʂƂ̐ڐG����
void Player::intersect_ground()
{
	// �t�B�[���h���擾
	auto& field = world_->field();
	// �n�ʂƂ̐ڐG�_
	Vector3 intersect;
	// �ڐG����p����
	Line line = Line(position_ + Vector3(0.0f, 10.0f, 0.0f), position_ - Vector3(0.0f, 1.0f, 0.0f));

	// �n�ʂƂ̐ڐG�_���擾
	if (field.collide_line(line.start, line.end, &intersect))
	{
		// �ڒn�����ꍇ�Ay�����W��␳����i�n�ʂɂ߂荞�܂Ȃ��j
		if (intersect.y >= position_.y)
		{
			velocity_.y = 0;			// y���ړ��ʂ�0�ɂ���
			position_.y = intersect.y;	// y���ʒu��␳
			is_ground_ = true;			// �ڒn�����true�ɂ���
		}
	}
	else
	{
		is_ground_ = false;				// �ڒn�����false�ɂ���i�ڒn���Ă��Ȃ��j
	}
}

// �ǂƂ̐ڐG����
void Player::intersect_wall()
{
	// �t�B�[���h���擾
	auto& field = world_->field();
	// �ǂƂ̐ڐG�_
	Vector3 intersect;
	// �ڐG����p����
	BoundingSphere sphere = BoundingSphere(position_ + Vector3(0.0f, 8.5f, 0.0f), 4.5f);

	// �ǂƂ̐ڐG�_���擾
	if (field.collide_sphere(sphere.position(), sphere.radius(), &intersect))
	{
		// �v���C���[�̍��W��␳
		position_.x = intersect.x;
		position_.z = intersect.z;
	}
}

// �������
void Player::ready_to_skip()
{
	// ���[�V�����̍Đ����x�������グ��
	mesh_.change_speed(1.5f);
	// ������ԂƖ��G���Ԃ�ݒ�
	skip_timer_ = 30.0f;			// ������ԁF0.5�b
	invincible_timer_ = 12.0f;		// ���G���ԁF0.2�b
}

// �K�[�h�͐������邩
bool Player::can_block(Vector3 atk_pos)
{
	// ���ςōU������̂�������𔻒�
	Vector3 to_attack = atk_pos - position_;									// �U����������̃x�N�g��
	float forward_dot_target = Vector3::Dot(rotation_.Forward(), to_attack);	// �O�����ƍU������̓���

	// �O�����ƃv���C���[�̓��ς�0�ȏ�ł���΁ATrue��Ԃ�
	return (forward_dot_target >= 0.0f);
}

// ���G���ԓ��ł��邩
bool Player::is_invincible()
{
	return (invincible_timer_ > 0.0f || state_ == PlayerState::Damage);
}

// �X�[�p�[�A�[�}�[��Ԃł��邩
bool Player::is_super_armor()
{
	return state_ == PlayerState::Slash3;
}