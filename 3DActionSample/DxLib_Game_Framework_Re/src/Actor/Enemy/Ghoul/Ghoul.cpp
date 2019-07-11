#include "Ghoul.h"
#include "../../../World/IWorld.h"
#include "../../../ID/EventMessage.h"
#include "../../../Field/Field.h"
#include "../../../Actor/Body/Line.h"
#include "../../ActorGroup.h"
#include "../EnemyAttack.h"
#include "../../Damage.h"
#include "../../../Sound/Sound.h"
#include "../../../ID/SourceID.h"

// �N���X�F�O�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

//--------------------------------------------------

const int Power = 8;					// �U����
const float AttackInterval = 120.0f;	// �U���Ԋu

//--------------------------------------------------

// �R���X�g���N�^
Ghoul::Ghoul(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Enemy(world, "Ghoul", position, angle, body),
	mesh_{ MESH_GHOUL, GhoulMotion::MOTION_IDLE },
	motion_{ GhoulMotion::MOTION_IDLE },
	state_{ GhoulState::Idle },
	state_timer_{ 0.0f },
	attack_on_{ false },
	is_following_player_{ false },
	is_moving_{ false },
	attack_interval_{ 0.0f }
{
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
	rand_.randomize();

	const int min = 1, max = 3;	// ���̏�Ԏ������Ԃ̍ŏ��l�ƍő�l�i�b�j
	ready_to_next_state(min, max);
}

// �X�V
void Ghoul::update(float delta_time)
{
	// ��������
	velocity_ += Vector3::Down * Gravity;		// �d�͉����x���v�Z
	position_.y += velocity_.y * delta_time;	// y�����W���v�Z

	intersect_ground();		// �n�ʂƂ̐ڐG����
	intersect_wall();		// �ǂƂ̐ڐG����
	clamp_position();		// ���W����

	// �G�̏�Ԃ��X�V
	update_state(delta_time);
	// ���[�V������ύX
	mesh_.change_motion(motion_);
	// ���b�V�����X�V
	mesh_.update(delta_time);
	// �s����v�Z
	mesh_.transform(pose());

	// HP��0�ȉ��ɂȂ�ƁA���S��ԂɈڍs
	if (current_hp_ <= 0)
	{
		if (state_ == GhoulState::Death) return;
		change_state(GhoulState::Death, MOTION_DEATH);
		world_->send_message(EventMessage::EnemyDead);	// ���S���b�Z�[�W�𑗐M
		return;
	}

	// ���ݗݐϒl�����ʂ��z������A���ݏ�ԂɈڍs
	if (current_wince_ >= ToWince && state_ != GhoulState::Wince)
	{
		current_wince_ = 0;
		change_state(GhoulState::Wince, MOTION_WINCE);
		return;
	}

	// �U���Ԋu�J�E���^�[�����Z
	if (attack_interval_ > 0.0f)
		attack_interval_ -= delta_time;
}

// �`��
void Ghoul::draw() const
{
	mesh_.draw();	// ���b�V����`��

	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
	body_->transform(pose())->draw();
}

// �Փ˃��A�N�V����
void Ghoul::react(Actor& other)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == GhoulState::Death) return;

	Enemy::react(other);
}

// ���b�Z�[�W����
void Ghoul::handle_message(EventMessage message, void* param)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == GhoulState::Death) return;

	// �v���C���[����_���[�W���󂯂�
	if (message == EventMessage::EnemyDamage)
	{
		// ���b�Z�[�W����v���C���[�̍U���͂Ƌ��ݒl���擾���A�_���[�W�v�Z���s��
		Damage* damage = (Damage*)param;
		if (state_ != GhoulState::Death)
			current_hp_ -= damage->power;
		if (state_ != GhoulState::Wince)
			current_wince_ += damage->impact;
	}

	// �������b�Z�[�W���󂯂�
	if (message == EventMessage::EnemyDestroy)
	{
		if (state_ != GhoulState::Death)
			current_hp_ = 0;
	}
}

// ��Ԃ̍X�V
void Ghoul::update_state(float delta_time)
{
	// ���݂̏�Ԃɉ����āA�G���X�V
	switch (state_)
	{
	case(GhoulState::Idle):
		idle(delta_time);
		break;
	case(GhoulState::Move):
		move(delta_time);
		break;
	case(GhoulState::Attack):
		attack(delta_time);
		break;
	case(GhoulState::Wince):
		wince(delta_time);
		break;
	case(GhoulState::Death):
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// ��ԃ^�C�}�[�����Z
}

// ��Ԃ̕ύX
void Ghoul::change_state(GhoulState state, int motion)
{
	previous_state_ = state_;

	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
	interval_ = 0.0f;
}

// �ҋ@��Ԃł̍X�V
void Ghoul::idle(float delta_time)
{
	// ��莞�Ԍ�A���̍s���𒊑I�i�ҋ@��Ԃ̈ێ����Ԃ͗����Ō��߂�j
	if (state_timer_ >= state_time_)
		next_action();
}

// �ړ���Ԃł̍X�V
void Ghoul::move(float delta_time)
{
	// ============================================================
	// �ȉ��͈ړ����̏���

	// �v���C���[��Ǐ]���Ă���΁A�펞�ړI�n�̍��W���X�V
	if (is_following_player_)	next_destination_ = get_player_position();

	// �ړI�n�̕����Ɍ����Ă���A�ړI�n�ֈړ�
	// ��]�����i�v���C���[�ɒǏ]���Ă���ꍇ�A�ړ�������ł��v���C���[�Ɍ�����悤�ɂȂ��Ă���j
	motion_ = MOTION_IDLE;
	float angle_to_target = get_angle_to_target(next_destination_);

	if (angle_to_target >= 0.5f)
	{
		motion_ = MOTION_TURN_RIGHT;
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * delta_time);
	}
	else if (angle_to_target <= -0.5f)
	{
		motion_ = MOTION_TURN_LEFT;
		rotation_ *= Matrix::CreateRotationY(-RotateSpeed * delta_time);
	}

	// �v���C���[�Ƃ̊p�x�����傫���ꍇ�A��]���[�V�������Đ����A��]���Ă���ړ�����
	if (angle_to_target >= 135.0f)
	{
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * 2 * delta_time);
	}
	else if (angle_to_target <= -135.0f)
	{
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * 2 * delta_time);
	}

	rotation_ = Matrix::NormalizeRotationMatrix(rotation_);		// ��]�s���������

	if (is_moving_ && get_unsigned_angle_to_target(next_destination_) <= 18.0f)
	{
		motion_ = MOTION_WALK;

		// �ړ�����
		velocity_ = Vector3::Zero;						// �ړ��ʂ����Z�b�g
		velocity_ += rotation_.Forward() * WalkSpeed;	// �ړ����x�����Z
		position_ += velocity_ * delta_time;			// ���̈ʒu���v�Z
	}

	if (is_moving_)
	{
		// �ړI�n�ɒ����ƁA�ړ�����
		if (can_attack_player() || Vector3::Distance(position_, next_destination_) <= 12.0f)
		{
			interval_ = state_timer_ + 12.0f;	// ���̍s����0.2�b��Ɏ��s
			is_moving_ = false;					// �ړ�����
		}

		// �s���̈ێ����Ԃ𒴂�����A���̍s���𒊑I
		if (move_timer_ >= state_time_)
		{
			next_action();
		}

		move_timer_ += delta_time;
	}

	// �ړ����̏����I��
	// ============================================================

	// ============================================================
	// �ȉ��͈ړ�������̏���

	if (!is_moving_)
	{
		// ���̍s�������s
		if (state_timer_ >= interval_)
		{
			// �v���C���[�͋߂��ɂ���ꍇ�A�U���s���Ɉڍs
			if (can_attack_player())
			{
				attack_interval_ = AttackInterval;
				change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
			}
			else
			{
				next_action();	// ���̍s���𒊑I
			}
		}
	}

	// �ړ�������̏����I��
	// ============================================================
}

// ���ݏ�Ԃł̍X�V
void Ghoul::wince(float delta_time)
{
	// ���[�V�����I����A���̍s���𒊑I
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		next_action();
	}
}

// �U����Ԃł̍X�V
void Ghoul::attack(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= 9.0f && !attack_on_)
	{
		attack_on_ = true;

		float distance = 8.0f;					// �U������̔��������i�O������ǂꂮ�炢�j
		float height = 12.5f;					// �U������̍���
		Vector3 attack_position = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, Power));
		Sound::play_se(SE_ENEMY_ATK_LIGHT);		// SE���Đ�
		interval_ = state_timer_ + 40.0f;
	}

	// ���[�V�����I����A���̍s���𒊑I
	if (state_timer_ >= 26.0f)
	{
		motion_ = MOTION_IDLE;
		if (state_timer_ >= interval_)	next_action();
	}
}

// ���S��Ԃł̍X�V
void Ghoul::death(float delta_time)
{
	// ���[�V�������I������ƁA���S�����L����
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		die();
	}
}

// ���̍s��������
void Ghoul::next_action()
{
	// �����Ŏ��̍s��������i�ŏ��l�F0�A�ő�l�F6�j
	int i = rand_.rand_int(0, 6);

	// �ҋ@�A�ړ��A�U����Ԃւ̈ڍs�m����1/6�A2/3�A1/6�ɂȂ��Ă���
	// �ҋ@��Ԃւ̈ڍs
	if (i == 0)
	{
		// �ҋ@�s���͘A���ɔ������Ȃ�
		if (previous_state_ == GhoulState::Idle)
		{
			next_action();
			return;
		}

		const int min = 2, max = 4;		// ���̏�Ԏ������Ԃ̍ŏ��l�ƍő�l�i�b�j
		ready_to_next_state(min, max);
		change_state(GhoulState::Idle, GhoulMotion::MOTION_IDLE);
		return;
	}
	// �ړ���Ԃւ̈ڍs
	else if (i <= 5)
	{
		next_destination();			// ���̖ړI�n������
		// �ړ���Ԃ�4�`8�b�Ԉێ�
		const int min = 4, max = 8;		// ���̏�Ԏ������Ԃ̍ŏ��l�ƍő�l�i�b�j
		ready_to_next_state(min, max);

		move_timer_ = 0.0f;			// �ړ���ԃ^�C�}�[�����Z�b�g
		is_moving_ = true;
		change_state(GhoulState::Move, GhoulMotion::MOTION_WALK);
		return;
	}
	// �U����Ԃւ̈ڍs
	else
	{
		attack_interval_ = AttackInterval;
		change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
		return;
	}
}

// ���̖ړI�n������
void Ghoul::next_destination()
{
	int i = rand_.rand_int(0, 2);

	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	// �t�B�[���h�̎Q�Ƃ��擾
	auto& field = world_->field();
	// �t�B�[���h�̍ő�ƍŏ����W���擾
	auto max_pos = field.max_position();
	auto min_pos = field.min_position();

	// i = 0�A�܂��̓v���C���[�͑��݂��Ȃ��ꍇ�A�����_���ō��W�𐶐�����
	if (i == 0 || player == nullptr)
	{
		// �v���C���[�Ǐ]������
		is_following_player_ = false;

		// x���̍��W�𐶐�;
		float x = rand_.rand_float(min_pos.x, max_pos.x);
		// z���̍��W�𐶐�
		float z = rand_.rand_float(min_pos.z, max_pos.z);
		// �����������W��ړI�n�ɂ���
		Vector3 new_dest = Vector3(x, 0.0f, z);
		next_destination_ = new_dest;
	}
	else
	{
		// �v���C���[�Ǐ]�J�n
		is_following_player_ = true;

		// �v���C���[�̈ʒu���擾���A������ړI�n�ɂ���
		next_destination_ = get_player_position();
	}
}

// �v���C���[���U���ł��邩
bool Ghoul::can_attack_player()
{
	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (!player_exists()) return false;
	// �v���C���[���O�ɂ��Ȃ���΁A�AFalse��Ԃ�
	if (!player_in_forward()) return false;
	// �v���C���[���U���ł��鋗�����ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_distance(Range)) return false;
	// �v���C���[���U���ł���p�x���ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_angle(Angle)) return false;
	// �U���Ԋu��0���傫���ꍇ�AFalse��Ԃ�
	if (attack_interval_ > 0.0f) return false;

	// ������S�Ė������Ă���΁ATrue��Ԃ�
	return true;
}

// ���̏�Ԃւ̈ڍs����
void Ghoul::ready_to_next_state(int min, int max)
{
	// �����Ŏ��̏�Ԏ������Ԃ����߂�
	int i = rand_.rand_int(min, max);
	state_time_ = 60 * (float)i;
}