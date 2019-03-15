#include "DragonBoar.h"
#include "../../../World/IWorld.h"
#include "../../../ID/EventMessage.h"
#include "../../Damage.h"
#include "../EnemyAttack.h"
#include "../../ActorGroup.h"

// �N���X�F�G�i�C�m�V�V�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Enemy(world, "DragonBoar", position, angle, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_ROAR },
	motion_{ DragonBoarMotion::MOTION_ROAR },
	state_{ DragonBoarState::Roar },
	state_timer_{ 0.0f },
	attack_on_{ false },
	is_moving_{ false },
	is_anger_{ false },
	anger_timer_{ 0.0f },
	dash_timer_{ 0.0f }
{
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
	rand_.randomize();
}

// �X�V
void DragonBoar::update(float delta_time)
{
	// ��������
	velocity_ += Vector3::Down * Gravity;		// �d�͉����x���v�Z
	position_.y += velocity_.y * delta_time;	// y�����W���v�Z
	// �n�ʂƂ̐ڐG����
	intersect_ground();
	// �ǂƂ̐ڐG����
	intersect_wall();

	// �G�̏�Ԃ��X�V
	update_state(delta_time);
	// ���[�V������ύX
	mesh_.change_motion(motion_);
	// ���b�V�����X�V
	mesh_.update(delta_time);
	// �s����v�Z
	mesh_.transform(pose());

	// HP��0�ȉ��ɂȂ�ƁA���S��ԂɈڍs
	if (current_hp_ <= 0 && state_ != DragonBoarState::Death)
	{
		world_->send_message(EventMessage::BossDead);	// ���S���b�Z�[�W�𑗐M
		change_state(DragonBoarState::Death, MOTION_DEATH);
		return;
	}

	/*
	// �̗͂������ȉ��ɂȂ�����A1����K���ē{���Ԃֈڍs
	if (current_hp_ <= HP / 2 && !is_anger_)
	{
		current_wince_ /= 2;
		is_anger_ = true;
		anger_timer_ = 120.0f;
		change_state(DragonBoarState::Roar, MOTION_ROAR);
		return;
	}
	*/

	// ���ݗݐϒl�����ʂ��z������A���ݏ�ԂɈڍs
	if (current_wince_ >= ToWince && state_ != DragonBoarState::Wince)
	{
		current_wince_ = 0;
		change_state(DragonBoarState::Wince, MOTION_WINCE);
		return;
	}
}

// �`��
void DragonBoar::draw() const
{
	mesh_.draw();	// ���b�V����`��

	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
	body_->transform(pose())->draw();
	// �����ŕ����������i�f�o�b�O���[�h�̂݁j
	unsigned int Cr;
	Cr = GetColor(255, 0, 0);
	DrawLine3D(position_, position_ + pose().Forward() * 50.0f, Cr);

	// �f�o�b�O���b�Z�[�W
	// Cr = GetColor(255, 255, 255);
	// DrawFormatString(0, 0, Cr, "�v���C���[�ւ̊p�x�F %f", get_angle_to_player());
	/*
	if (player_in_front())
		DrawString(0, 0, "�v���C���[�͑O�ɂ���", Cr);
	else
		DrawString(0, 0, "�v���C���[�͌�ɂ���", Cr);
		*/
}

// �Փ˃��A�N�V����
void DragonBoar::react(Actor& other)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == DragonBoarState::Death) return;

	// �{��ːi���Ƀv���C���[�𓖂�������A�_���[�W��^����
	if (state_ == DragonBoarState::Anger && attack_on_)
	{
		// �v���C���[�ւ̃_���[�W�\���̂𐶐�
		Damage damage = { position_, 25 };
		// �v���C���[�փ_���[�W���b�Z�[�W�𑗂�
		other.handle_message(EventMessage::PlayerDamage, &damage);
		// �U������𖳌���
		attack_on_ = false;
		return;
	}
}

// ���b�Z�[�W����
void DragonBoar::handle_message(EventMessage message, void* param)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == DragonBoarState::Death) return;

	// �v���C���[����_���[�W���󂯂�
	if (message == EventMessage::EnemyDamage)
	{
		// ���b�Z�[�W����v���C���[�̍U���͂Ƌ��ݒl���擾���A�_���[�W�v�Z���s��
		Damage* damage = (Damage*)param;
		if (state_ != DragonBoarState::Death)
			current_hp_ -= damage->power;
		if (state_ != DragonBoarState::Wince)
			current_wince_ += damage->impact;
	}
}

// ��Ԃ̍X�V
void DragonBoar::update_state(float delta_time)
{
	// ���݂̏�Ԃɉ����āA�G���X�V
	switch (state_)
	{
	case(DragonBoarState::Idle):
		idle(delta_time);
		break;
	case(DragonBoarState::Roar):
		roar(delta_time);
		break;
	case(DragonBoarState::Normal):
		normal(delta_time);
		break;
	case(DragonBoarState::Bite):
		bite(delta_time);
		break;
	case(DragonBoarState::Anger):
		anger(delta_time);
		break;
	case(DragonBoarState::Wince):
		wince(delta_time);
		break;
	case(DragonBoarState::Death):
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// ��ԃ^�C�}�[�����Z
}

// ��Ԃ̕ύX
void DragonBoar::change_state(DragonBoarState state, int motion)
{
	previous_state_ = state_;

	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
	interval_ = 0.0f;

	if (state == DragonBoarState::Normal || state == DragonBoarState::Anger)
		is_moving_ = true;

	if (state == DragonBoarState::Anger)
	{
		dash_timer_ = 60 * rand_.rand_int(2, 4);
	}
}

// �ҋ@��Ԃł̍X�V
void DragonBoar::idle(float delta_time)
{
	// 5�b��A�ړ��J�n
	if (state_timer_ >= 300.0f)
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// �{��ړ��ֈڍs
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// �ʏ�ړ��ֈڍs
	}

	/*
	if (state_timer_ >= 120.0f)
	{
		change_state(DragonBoarState::Bite, DragonBoarMotion::MOTION_BITE);
	}
	*/
}

// ���K���̍X�V
void DragonBoar::roar(float delta_time)
{
	// ���[�V�����I����A�ړ��J�n
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// �{��ړ��ֈڍs
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// �ʏ�ړ��ֈڍs
	}
}

// �ʏ��Ԃł̈ړ�
void DragonBoar::normal(float delta_time)
{
	// ============================================================
	// �ȉ��͈ړ����̏���

	// �ړI�n�i�v���C���[�̍��W�j���X�V
	next_destination_ = get_player_position();

	// �ړI�n�̕����Ɍ����Ă���A�ړI�n�ֈړ�
	// ��]�����i�����ƃv���C���[�Ɍ�����悤�Ɂj
	motion_ = MOTION_IDLE;
	float angle_to_target = get_angle_to_target(next_destination_);

	if (angle_to_target >= 0.5f)
	{
		motion_ = MOTION_WALK;
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * delta_time);
	}
	else if (angle_to_target <= -0.5f)
	{
		motion_ = MOTION_WALK;
		rotation_ *= Matrix::CreateRotationY(-RotateSpeed * delta_time);
	}
	rotation_ = Matrix::NormalizeRotationMatrix(rotation_);		// ��]�s���������

	// �v���C���[�Ɍ����Ă�ꍇ�A�ړ�����
	if (is_moving_ && get_unsigned_angle_to_target(next_destination_) <= 18.0f)
	{
		motion_ = MOTION_WALK;

		// �ړ�����
		velocity_ = Vector3::Zero;						// �ړ��ʂ����Z�b�g
		velocity_ += rotation_.Forward() * WalkSpeed;	// �ړ����x�����Z
		position_ += velocity_ * delta_time;			// ���̈ʒu���v�Z
	}

	// �v���C���[�ɐڋ߂���ƁA���݂�
	if (is_moving_)
	{
		if (can_attack_player())
		{
			interval_ = state_timer_ + 12.0f;	// ���̍s����0.2�b��Ɏ��s
			is_moving_ = false;					// �ړ�����
		}
	}

	// �ړ����̏����I��
	// ============================================================

	// ============================================================
	// �ȉ��͈ړ�������̏���

	// �U����Ԃֈڍs
	if (!is_moving_ && state_timer_ >= interval_)
	{
		change_state(DragonBoarState::Bite, DragonBoarMotion::MOTION_BITE);
	}

	// �ړ�������̏����I��
	// ============================================================
}

// ���݂����̍X�V
void DragonBoar::bite(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= 40.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 40.0f + Vector3(0.0f, 12.5f, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, 15, 10));
		interval_ = state_timer_ + 40.0f;
	}

	// ���̏�Ԃֈڍs
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		next_action();
		// change_state(DragonBoarState::Idle, DragonBoarMotion::MOTION_IDLE);
	}
}

// �{���Ԃł̈ړ�
void DragonBoar::anger(float delta_time)
{
	motion_ = MOTION_IDLE;

	// 2�b�ԁA�v���C���[�Ǝ����킹
	if (state_timer_ <= 120.0f)
	{
		next_destination_ = get_player_position();						// �ړI�n�i�v���C���[�̍��W�j���X�V
		float angle_to_target = get_angle_to_target(next_destination_);	// �v���C���[�����̊p�x�����߂�
		// �v���C���[�Ɍ����ĉ�]
		if (angle_to_target >= 0.5f)
		{
			motion_ = MOTION_WALK;
			rotation_ *= Matrix::CreateRotationY(RotateSpeed * 2 * delta_time);
		}
		else if (angle_to_target <= -0.5f)
		{
			motion_ = MOTION_WALK;
			rotation_ *= Matrix::CreateRotationY(-RotateSpeed * 2 * delta_time);
		}
		rotation_ = Matrix::NormalizeRotationMatrix(rotation_);			// ��]�s���������
		return;
	}

	// �ړI�n�Ɍ������ēːi
	
}

// ���ݏ�Ԃł̍X�V
void DragonBoar::wince(float delta_time)
{
	// ���[�V�����I����A���̍s���𒊑I
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		next_action();
	}
}

// ���S��Ԃł̍X�V
void DragonBoar::death(float delta_time)
{
	// ���[�V�������I������ƁA���S�����L����
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		die();
	}
}

// ���̍s���𒊑I
void DragonBoar::next_action()
{
	// �����Ŏ��̍s��������
	int i = rand_.rand_int(0, 3);

	// �ҋ@�A�ړ��ւ̈ڍs�m����1/4�A3/4�ɂȂ��Ă���
	// �ҋ@��Ԃւ̈ڍs
	if (i == 0)
	{
		// �ҋ@�s���͘A���ɔ������Ȃ�
		if (previous_state_ == DragonBoarState::Idle)
		{
			next_action();
			return;
		}
		change_state(DragonBoarState::Idle, DragonBoarMotion::MOTION_IDLE);
		return;
	}
	// �ړ���Ԃւ̈ڍs
	else
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// �{��ړ��ֈڍs
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// �ʏ�ړ��ֈڍs
	}
}

// �v���C���[���U���ł��邩
bool DragonBoar::can_attack_player()
{
	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (!player_exists()) return false;
	// �v���C���[���O�ɂ��Ȃ���΁A�AFalse��Ԃ�
	if (!player_in_forward()) return false;
	// �v���C���[���U���ł��鋗�����ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_distance(Range)) return false;
	// �v���C���[���U���ł���p�x���ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_angle(Angle)) return false;

	// ������S�Ė������Ă���΁ATrue��Ԃ�
	return true;
}