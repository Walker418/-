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
#include "../AttackParameter.h"
#include "../Damage.h"
#include "PlayerInput.h"
#include "../../Sound/Sound.h"

// �N���X�F�v���C���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// ============================================================
// �ȉ��͊e���[�V���������֘A�̃t���[����

const float FrameInOneSecond = 60.0f;		// 1�b = 60�t���[��

const float Atk1_Active = 25.0f;			// �U��1�i�ڂ̔��蔭���t���[����
const float Atk1_InputValid = 35.0f;		// �U��1�i�ڂ̎��̍s�����͂̊J�n�t���[����
const float Atk1_InputInvalid = 45.0f;		// �U��1�i�ڂ̎��̍s�����͂̏I���t���[����
const float Atk1_End = 60.0f;				// �U��1�i�ڂ̃��[�V�����I���t���[����

const float Atk2_Active = 10.0f;			// �U��2�i�ڂ̔��蔭���t���[����
const float Atk2_InputValid = 31.0f;		// �U��2�i�ڂ̎��̍s�����͂̊J�n�t���[����
const float Atk2_InputInvalid = 45.0f;		// �U��2�i�ڂ̎��̍s�����͂̏I���t���[����
const float Atk2_End = 45.0f;				// �U��2�i�ڂ̃��[�V�����I���t���[����

const float Atk3_MoveTime = 69.0f;			// �U��3�i�ڂ̈ړ������t���[����
const float Atk3_MoveSpeed = 0.22f;			// �U��3�i�ڂ̈ړ����x
const float Atk3_Active = 43.0f;			// �U��3�i�ڂ̔��蔭���t���[����
const float Atk3_InputValid = 50.0f;		// �U��3�i�ڂ̎��̍s�����͂̊J�n�t���[����
const float Atk3_InputInvalid = 72.0f;		// �U��3�i�ڂ̎��̍s�����͂̏I���t���[����
const float Atk3_End = 72.0f;				// �U��3�i�ڂ̃��[�V�����I���t���[����

const float JumpAtk1_MoveTime = 45.0f;		// �W�����v�U��1�i�ڂ̈ړ������t���[����
const float JumpAtk1_MoveSpeed = 0.7f;		// �W�����v�U��1�i�ڂ̈ړ����x
const float JumpAtk1_Active = 45.0f;		// �W�����v�U��1�i�ڂ̔��蔭���t���[����
const float JumpAtk1_InputValid = 50.0f;	// �W�����v�U��1�i�ڂ̎��̍s�����͂̊J�n�t���[����
const float JumpAtk1_InputInvalid = 65.0f;	// �W�����v�U��1�i�ڂ̎��̍s�����͂̏I���t���[����
const float JumpAtk1_End = 70.0f;			// �W�����v�U��1�i�ڂ̃��[�V�����I���t���[����

const float JumpAtk2_Active = 10.0f;		// �W�����v�U��2�i�ڂ̔��蔭���t���[����
const float JumpAtk2_InputValid = 25.0f;	// �W�����v�U��2�i�ڂ̎��̍s�����͂̊J�n�t���[����
const float JumpAtk2_InputInvalid = 43.0f;	// �W�����v�U��2�i�ڂ̎��̍s�����͂̏I���t���[����
const float JumpAtk2_End = 45.0f;			// �W�����v�U��2�i�ڂ̃��[�V�����I���t���[����

const float Damage_End = 31.0f;				// ��e���[�V�����̏I���t���[����
const float Block_End = 34.0f;				// �K�[�h�ɂ��m�b�N�o�b�N���[�V�����̏I���t���[����

const float GuardAtk_Active = 35.0f;		// �K�[�h�U���̔��蔭���t���[����
const float GuardAtk_InputValid = 45.0f;	// �K�[�h�U���̎��̍s�����͂̊J�n�t���[����
const float GuardAtk_InputInvalid = 60.0f;	// �K�[�h�U���̎��̍s�����͂̏I���t���[����
const float GuardAtk_End = 60.0f;			// �K�[�h�U���̃��[�V�����I���t���[����

// ============================================================

// �R���X�g���N�^
Player::Player(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	is_ground_{ false },
	is_guard_{ false },
	attack_on_{ false },
	jump_attack_started_{ false },
	is_defeated_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = PlayerParameter::HP;

	state_timer_.reset();
	invincible_timer_.shut();
	evasion_timer_.shut();
	hit_stop_timer_.shut();
}

// �X�V
void Player::update(float delta_time)
{
	// HP��0�ȉ��ɂȂ�ƁA���S��ԂɈڍs
	if (current_hp_ <= 0 && state_ != PlayerState::Death)
	{
		change_state(PlayerState::Death, MOTION_DEATH);
		return;
	}

	// ���G���ԃ^�C�}�[���X�V
	invincible_timer_.update(delta_time);

	// �q�b�g�X�g�b�v�^�C�}�[���X�V
	hit_stop_timer_.update(delta_time);
	// �q�b�g�X�g�b�v���Ă���΁A�ȍ~�̍X�V�͂��Ȃ�
	if (is_hit_stop()) return;

	// ��������
	velocity_ += Vector3::Down * PlayerParameter::Gravity;	// �d�͉����x���v�Z
	position_.y += velocity_.y * delta_time;				// y�����W���v�Z

	intersect_ground();		// �n�ʂƂ̐ڐG����
	intersect_wall();		// �ǂƂ̐ڐG����
	clamp_position();		// ���W����

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
}

// �`��
void Player::draw() const
{
	mesh_.draw();	// ���b�V����`��

	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
	body_->transform(pose())->draw();
}

// �Փ˃��A�N�V����
void Player::react(Actor& other)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == PlayerState::Death) return;
	// ���G��Ԃł͔������Ȃ�
	if (is_invincible()) return;
}

// ���b�Z�[�W����
void Player::handle_message(EventMessage message, void* param)
{
	// ���G���Ԓ��A���݂⎀�S��Ԃł͔������Ȃ�
	if (is_invincible() || state_ == PlayerState::Death) return;

	// �G����_���[�W���󂯂�
	if (message == EventMessage::PlayerDamage)
	{
		// �J������U��������
		world_->send_message(EventMessage::Camera_Vibration);

		// ���b�Z�[�W����G�̍U�����擾
		Damage* damage = (Damage*)param;
		Vector3 atk_pos = damage->position;

		// �K�[�h����
		if (is_guard_ && can_block(atk_pos))
		{
			// �U������̈ʒu���v���C���[���O�̏ꍇ�A�K�[�h����������
			Sound::play_se(SE_BLOCK);	// SE��炷
			change_state(PlayerState::Blocking, MOTION_GUARD_BLOCK);
			return;
		}

		// �K�[�h���������Ȃ��ꍇ�A�_���[�W���v�Z����
		current_hp_ -= damage->power;	// �_���[�W�v�Z
		// �X�[�p�[�A�[�}�[��ԂłȂ���΁A���ݏ�Ԃֈڍs
		if (!is_super_armor())
		{
			change_state(PlayerState::Damage, MOTION_IMPACT);
		}
	}

	// �q�b�g�X�g�b�v����
	if (message == EventMessage::HitStop)
	{
		float* hit_stop_time = (float*)param;
		reset_hit_stop(*hit_stop_time);
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
		mesh_.reset_speed();
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

	state_timer_.update(delta_time);		// ��ԃ^�C�}�[�����Z
}

// ��Ԃ̕ύX
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_.reset();

	attack_on_ = false;
	jump_attack_started_ = false;
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
		// �ړ����͂��擾
		forward_speed = PlayerParameter::DashSpeed * PlayerInput::move_input().y;	// �O��
		left_speed = -PlayerParameter::DashSpeed * PlayerInput::move_input().x;		// ���E
	}
	// �ړ����Ă���΁A���s���[�V�����ɕύX
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_DASH;

	// ��Ԃ��X�V
	change_state(PlayerState::Normal, motion);

	// �ړ��ʂ��v�Z���A�v���C���[���ړ�������
	velocity_ += camera_forward * forward_speed;	// �O�㑬�x�����Z
	velocity_ += camera.Left() * left_speed;		// ���E���x�����Z
	velocity_.Normalize();							// �ړ����x�𐳋K��
	position_ += velocity_ * delta_time;			// ���̈ʒu���v�Z

	// �v���C���[����]������
	if (velocity_.x != 0.0f || velocity_.z != 0.0f)		// �ړ����Ă����
	{
		Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, Vector3(velocity_.x, 0.0f, velocity_.z).Normalize(), Vector3::Up);	// �V����������ݒ�
		rotation_ = Matrix::Lerp(rotation_, new_rotation, PlayerParameter::RotateSpeed);	// ��Ԃŕ�����]������
	}

	// �ړ������I��
	// ============================================================

	// ���
	if (PlayerInput::evasion() && is_ground_)
	{
		ready_for_evasion();
		change_state(PlayerState::ForwardEvasion, PlayerMotion::MOTION_DASH);
		return;
	}
}

// �U���i1�i�ځj�ł̍X�V
void Player::slash1(float delta_time)
{
	// �U������𔭐�
	if (state_timer_.get_time() >= Atk1_Active && !attack_on_)
	{
		attack_on_ = true;	// �U�������1�񂵂��������Ȃ�

		// �U���p�����[�^�\���̂𐶐�
		const float distance = 12.0f;						// �U������̔��������i�O������ǂꂮ�炢�j
		const float height = 9.5f;							// �U������̍���
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// �U������̈ʒu
		int		power = PlayerParameter::Power_Atk1;		// �З�
		int		wince = PlayerParameter::Wince_Atk1;		// ���ݒl
		float	hit_stop = PlayerParameter::HitStop_Short;	// �q�b�g�X�g�b�v
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// �U������𐶐�
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SE���Đ�

		mesh_.change_speed(1.4f);	// �ȍ~�̃��[�V�������x�������x���ɂ���
	}

	// ���[�V�����I���̑O�ɁA���̍U�������ւ̈ڍs
	if (state_timer_.get_time() > Atk1_InputValid && state_timer_.get_time() <= Atk1_InputInvalid && is_ground_)
	{
		// �U����2�i�K�ڂւ̈ڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash2, MOTION_SLASH_2);
			return;
		}

		// ����ւ̈ڍs
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_.get_time() >= Atk1_End)
	{
		normal(delta_time);
	}
}

// �U���i2�i�ځj�ł̍X�V
void Player::slash2(float delta_time)
{
	// �U������𔭐�
	if (state_timer_.get_time() >= Atk2_Active && !attack_on_)
	{
		attack_on_ = true;	// �U�������1�񂵂��������Ȃ�

		// �U���p�����[�^�\���̂𐶐�
		const float distance = 12.0f;						// �U������̔��������i�O������ǂꂮ�炢�j
		const float height = 9.5f;							// �U������̍���
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// �U������̈ʒu
		int		power = PlayerParameter::Power_Atk2;		// �З�
		int		wince = PlayerParameter::Wince_Atk2;		// ���ݒl
		float	hit_stop = PlayerParameter::HitStop_Short;	// �q�b�g�X�g�b�v
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// �U������𐶐�
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SE���Đ�

		mesh_.reset_speed();		// �ȍ~�̃��[�V�������x�������x���ɂ���
	}

	// ���[�V�����I���̑O�ɁA���̍U�������ւ̈ڍs
	if (state_timer_.get_time() > Atk2_InputValid && state_timer_.get_time() < Atk2_InputInvalid && is_ground_)
	{
		// �U����3�i�K�ڂւ̈ڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::Slash3, MOTION_SLASH_3);
			return;
		}

		// ����ւ̈ڍs
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_.get_time() >= Atk2_End)
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
	if (state_timer_.get_time() <= Atk3_MoveTime)
	{
		velocity_ = rotation_.Forward() * Atk3_MoveSpeed;
		position_ += velocity_ * delta_time;
	}

	// �U������𔭐�
	if (state_timer_.get_time() >= Atk3_Active && !attack_on_)
	{
		attack_on_ = true;	// �U�������1�񂵂��������Ȃ�

		// �U���p�����[�^�\���̂𐶐�
		const float distance = 15.0f;						// �U������̔��������i�O������ǂꂮ�炢�j
		const float height = 9.5f;							// �U������̍���
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// �U������̈ʒu
		int		power = PlayerParameter::Power_Atk3;		// �З�
		int		wince = PlayerParameter::Wince_Atk3;		// ���ݒl
		float	hit_stop = PlayerParameter::HitStop_Long;	// �q�b�g�X�g�b�v
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// �U������𐶐�
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SE���Đ�
	}

	// ���[�V�����I���̑O�ɁA����ւ̈ڍs
	if (state_timer_.get_time() > Atk3_InputValid && state_timer_.get_time() < Atk3_InputInvalid && is_ground_)
	{
		// ����+�����͂����ƁA�����ԂɈڍs
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_.get_time() >= Atk3_End)
	{
		normal(delta_time);
	}
}

// �W�����v�U���i1�i�ځj�ł̍X�V
void Player::jump_attack1(float delta_time)
{
	// ���[�V�����Đ��̊ԁA�L�����N�^�[��O�i������
	if (state_timer_.get_time() < JumpAtk1_MoveTime)
	{
		velocity_ = rotation_.Forward() * JumpAtk1_MoveSpeed;
		position_ += velocity_ * delta_time;
	}

	// �U������𔭐�
	if (state_timer_.get_time() >= JumpAtk1_Active && !attack_on_)
	{
		attack_on_ = true;	// �U�������1�񂵂��������Ȃ�

		// �U���p�����[�^�\���̂𐶐�
		const float distance = 13.0f;						// �U������̔��������i�O������ǂꂮ�炢�j
		const float height = 9.5f;							// �U������̍���
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// �U������̈ʒu
		int		power = PlayerParameter::Power_JumpAtk1;	// �З�
		int		wince = PlayerParameter::Wince_JumpAtk1;	// ���ݒl
		float	hit_stop = PlayerParameter::HitStop_Long;	// �q�b�g�X�g�b�v
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// �U������𐶐�
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SE���Đ�
	}

	// ���[�V�����I���̑O�ɁA���̍U�������ւ̈ڍs
	if (state_timer_.get_time() > JumpAtk1_InputValid && state_timer_.get_time() < JumpAtk1_InputInvalid && is_ground_)
	{
		// �W�����v�U����2�i�K�ւ̈ڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.2f);
			change_state(PlayerState::JumpAttack2, MOTION_SLASH_2);
			return;
		}

		// ����ւ̈ڍs
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_.get_time() >= JumpAtk1_End)
	{
		normal(delta_time);
	}
}

// �W�����v�U���i2�i�ځj�ł̍X�V
void Player::jump_attack2(float delta_time)
{
	// �U������𔭐�
	if (state_timer_.get_time() >= JumpAtk2_Active && !attack_on_)
	{
		attack_on_ = true;	// �U�������1�񂵂��������Ȃ�

		// �U���p�����[�^�\���̂𐶐�
		const float distance = 12.0f;						// �U������̔��������i�O������ǂꂮ�炢�j
		const float height = 9.5f;							// �U������̍���
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// �U������̈ʒu
		int		power = PlayerParameter::Power_JumpAtk2;	// �З�
		int		wince = PlayerParameter::Wince_JumpAtk2;	// ���ݒl
		float	hit_stop = PlayerParameter::HitStop_Short;	// �q�b�g�X�g�b�v
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// �U������𐶐�
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SE���Đ�

		mesh_.reset_speed();		// �ȍ~�̃��[�V�������x�������x���ɂ���
	}

	// ���[�V�����I���̑O�ɁA���̍U�������ւ̈ڍs
	if (state_timer_.get_time() > JumpAtk2_InputValid && state_timer_.get_time() < JumpAtk2_InputInvalid && is_ground_)
	{
		// �ʏ�U����1�i�K�ڂւ̈ڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// ���̃��[�V�������x��ݒ�
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}

		// ����ւ̈ڍs
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_.get_time() >= JumpAtk2_End)
	{
		normal(delta_time);
	}
}

// ��e��Ԃł̍X�V
void Player::damage(float delta_time)
{
	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_.get_time() >= Damage_End)
	{
		normal(delta_time);
	}
}

// �K�[�h���̍X�V
void Player::guard(float delta_time)
{
	// �K�[�h����𔭐�
	is_guard_ = true;

	// �K�[�h�U���ւ̈ڍs
	if (PlayerInput::attack())
	{
		mesh_.change_speed(1.5f);
		change_state(PlayerState::GuardAttack, MOTION_GUARD_SLASH);
		return;
	}

	// �K�[�h�J�n�̃��[�V�����I����A�K�[�h�ҋ@���[�V�����Ɉڍs
	if (state_timer_.get_time() >= mesh_.motion_end_time())
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
	if (state_timer_.get_time() >= Block_End)
	{

		state_ = PlayerState::Guard;
		guard(delta_time);
	}
}

// �K�[�h�U���ł̍X�V
void Player::guard_attack(float delta_time)
{
	// �U������𔭐�
	if (state_timer_.get_time() >= GuardAtk_Active && !attack_on_)
	{
		attack_on_ = true;	// �U�������1�񂵂��������Ȃ�

		// �U���p�����[�^�\���̂𐶐�
		const float distance = 15.0f;						// �U������̔��������i�O������ǂꂮ�炢�j
		const float height = 9.5f;							// �U������̍���
		Vector3 atk_pos = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);	// �U������̈ʒu
		int		power = PlayerParameter::Power_GuardAtk;	// �З�
		int		wince = PlayerParameter::Wince_GuardAtk;	// ���ݒl
		float	hit_stop = PlayerParameter::HitStop_Short;	// �q�b�g�X�g�b�v
		AttackParameter atk_para{ atk_pos, power, wince, hit_stop };
		// �U������𐶐�
		world_->add_actor(ActorGroup::PlayerAttack, new_actor<PlayerAttack>(world_, atk_para));
		Sound::play_se(SE_SLASH);	// SE���Đ�
	}

	// ���[�V�����I���̑O�ɁA���̍U�������ւ̈ڍs
	if (state_timer_.get_time() > GuardAtk_InputValid && state_timer_.get_time() < GuardAtk_InputInvalid)
	{
		// �ʏ�U����1�i�K�ڂւ̈ڍs
		if (PlayerInput::attack())
		{
			mesh_.change_speed(1.6f);	// ���̃��[�V�������x��ݒ�
			change_state(PlayerState::Slash1, MOTION_SLASH_1);
			return;
		}

		// ����ւ̈ڍs
		if (PlayerInput::evasion())
		{
			attack_to_evasion();
		}
	}

	// �ʏ��Ԃւ̈ڍs
	if (state_timer_.get_time() >= GuardAtk_End)
	{
		normal(delta_time);
	}
}

// �K�[�h�I�����̍X�V
void Player::guard_end(float delta_time)
{
	// ���[�V�����I����A�ʏ��Ԃɖ߂�
	if (state_timer_.get_time() >= mesh_.motion_end_time() + 4.0f)
	{
		normal(delta_time);
	}
}

// ���S��Ԃł̍X�V
void Player::death(float delta_time)
{
	// �v���C���[���S���b�Z�[�W�𑗂�i1�񂾂��j
	if (!is_defeated_)
	{
		world_->send_message(EventMessage::PlayerDead);
		is_defeated_ = true;
	}

	// ���[�V�������I������ƁA���S�����L����
	if (state_timer_.get_time() >= mesh_.motion_end_time() * 2.0f)
	{
		die();
	}
}

// �O�����Ԃł̍X�V
void Player::forward_evasion(float delta_time)
{
	// �n�ʂɗ��ꂽ��A�ʏ��Ԃɖ߂�
	if (!is_ground_)
	{
		invincible_timer_.shut();	// ���G���Ԃ������I��
		normal(delta_time);
	}

	// �v���C���[�̍��W���ړ�
	velocity_ = rotation_.Forward() * PlayerParameter::EvasionSpeed;
	position_ += velocity_ * delta_time;

	// ����I����A�ʏ��Ԃɖ߂�
	if (evasion_timer_.is_time_out())
	{
		normal(delta_time);
		return;
	}

	// ����^�C�}�[���X�V
	evasion_timer_.update(delta_time);
}

// �������Ԃł̍X�V
void Player::left_evasion(float delta_time)
{
	// �n�ʂɗ��ꂽ��A�ʏ��Ԃɖ߂�
	if (!is_ground_)
	{
		invincible_timer_.shut();	// ���G���Ԃ������I��
		normal(delta_time);
	}

	// �v���C���[�̍��W���ړ�
	velocity_ = rotation_.Left() * PlayerParameter::EvasionSpeed;
	position_ += velocity_ * delta_time;

	// ����I����A�ʏ��Ԃɖ߂�
	if (evasion_timer_.is_time_out())
	{
		normal(delta_time);
		return;
	}

	// ����^�C�}�[���X�V
	evasion_timer_.update(delta_time);
}

// �E�����Ԃł̍X�V
void Player::right_evasion(float delta_time)
{
	// �n�ʂɗ��ꂽ��A�ʏ��Ԃɖ߂�
	if (!is_ground_)
	{
		invincible_timer_.shut();	// ���G���Ԃ������I��
		normal(delta_time);
	}

	// �v���C���[�̍��W���ړ�
	velocity_ = rotation_.Right() * PlayerParameter::EvasionSpeed;
	position_ += velocity_ * delta_time;

	// ����I����A�ʏ��Ԃɖ߂�
	if (evasion_timer_.is_time_out())
	{
		normal(delta_time);
		return;
	}

	// ����^�C�}�[���X�V
	evasion_timer_.update(delta_time);
}

// �U����̉���s���ڍs
void Player::attack_to_evasion()
{
	// �v���C���[�̕������͂��擾
	auto input = PlayerInput::move_input();

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

	// ���͂��������ƃv���C���[�̕����x�N�g���̍���45���ȉ��̏ꍇ�A����s���Ɉڂ�
	float angle = 45.0f;

	// �����
	if (Vector3::Angle(rotation_.Left(), direction) <= angle)
	{
		ready_for_evasion();
		change_state(PlayerState::LeftEvasion, PlayerMotion::MOTION_STRAFE_LEFT);
		return;
	}
	// �E���
	else if (Vector3::Angle(rotation_.Right(), direction) <= angle)
	{
		ready_for_evasion();
		change_state(PlayerState::RightEvasion, PlayerMotion::MOTION_STRAFE_RIGHT);
		return;
	}
	// �O���
	else
	{
		ready_for_evasion();
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

// ���W����
void Player::clamp_position()
{
	// �t�B�[���h���擾
	auto& field = world_->field();
	// �t�B�[���h�̍ő�ƍŏ����W���擾
	auto max_pos = field.max_position();
	auto min_pos = field.min_position();

	position_.x = MathHelper::clamp(position_.x, min_pos.x, max_pos.x);
	position_.z = MathHelper::clamp(position_.z, min_pos.z, max_pos.z);
}

// �������
void Player::ready_for_evasion()
{
	// ���[�V�����̍Đ����x�������グ��
	mesh_.change_speed(1.5f);
	// ������ԂƖ��G���Ԃ����Z�b�g
	evasion_timer_.reset();
	invincible_timer_.reset();
}

// �q�b�g�X�g�b�v�^�C�}�[�̃��Z�b�g
void Player::reset_hit_stop(float time)
{
	hit_stop_timer_.reset_time(time);
}

// �K�[�h�͐������邩
bool Player::can_block(Vector3 atk_pos) const
{
	// ���ςōU������̂�������𔻒�
	Vector3 to_attack = atk_pos - position_;									// �U����������̃x�N�g��
	float forward_dot_target = Vector3::Dot(rotation_.Forward(), to_attack);	// �O�����ƍU������̓���

	// �O�����ƃv���C���[�̓��ς�0�ȏ�ł���΁ATrue��Ԃ�
	return (forward_dot_target >= 0.0f);
}

// ���G���ԓ��ł��邩
bool Player::is_invincible() const
{
	return (!invincible_timer_.is_time_out() || state_ == PlayerState::Damage);
}

// �X�[�p�[�A�[�}�[��Ԃł��邩
bool Player::is_super_armor() const
{
	return state_ == PlayerState::Slash3;
}

// �q�b�g�X�g�b�v���Ȃ̂�
bool Player::is_hit_stop() const
{
	return !hit_stop_timer_.is_time_out();
}