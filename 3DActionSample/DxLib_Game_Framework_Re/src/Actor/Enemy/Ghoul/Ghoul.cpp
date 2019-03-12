#include "Ghoul.h"
#include "../../../World/IWorld.h"
#include "../../../ID/EventMessage.h"
#include "../../../Field/Field.h"
#include "../../../Actor/Body/Line.h"
#include "../../ActorGroup.h"
#include "../EnemyAttack.h"
#include "../../Damage.h"

// �N���X�F�O�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Ghoul::Ghoul(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "Ghoul", position, body),
	mesh_{ MESH_GHOUL, GhoulMotion::MOTION_IDLE },
	motion_{ GhoulMotion::MOTION_IDLE },
	state_{ GhoulState::Idle },
	state_timer_{ 0.0f },
	attack_on_{ false },
	is_following_player_{ false },
	is_moving_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
	attack_count_ = 0;
	rand_.randomize();

	ready_to_next_state(1, 3);
}

// �X�V
void Ghoul::update(float delta_time)
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
	if (current_hp_ <= 0 && state_ != GhoulState::Death)
	{
		world_->send_message(EventMessage::EnemyDead);	// ���S���b�Z�[�W�𑗐M
		change_state(GhoulState::Death, MOTION_DEATH);

		return;
	}

	// ���ݗݐϒl�����ʂ��z������A���ݏ�ԂɈڍs
	if (current_wince_ >= ToWince && state_ != GhoulState::Wince)
	{
		current_wince_ = 0;
		change_state(GhoulState::Wince, MOTION_WINCE);
		return;
	}
}

// �`��
void Ghoul::draw() const
{
	mesh_.draw();	// ���b�V����`��

	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
	body_->transform(pose())->draw();
	// �����ŕ����������i�f�o�b�O���[�h�̂݁j
	unsigned int Cr;
	Cr = GetColor(255, 0, 0);

	DrawLine3D(position_, position_ + pose().Forward() * 25.0f, Cr);
}

// �Փ˃��A�N�V����
void Ghoul::react(Actor& other)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == GhoulState::Death) return;
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
	// �A���U���񐔃J�E���^�[���X�V
	if (state == GhoulState::Attack)
		++attack_count_;
	else
		attack_count_ = 0;

	previous_state_ = state_;

	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
}

// �ҋ@��Ԃł̍X�V
void Ghoul::idle(float delta_time)
{
	// ��莞�Ԍ�A���̍s���𒊑I�i�ҋ@��Ԃ̈ێ����Ԃ͗����Ō��߂�j
	if (state_timer_ >= state_time_)
		next_move();
}

// �ړ���Ԃł̍X�V
void Ghoul::move(float delta_time)
{
	// ============================================================
	// �ȉ��͈ړ����̏���

	float interval = 0.0f;	// ���̍s���ւ̈ڍs�^�C�~���O�̕ϐ���錾���Ă���

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
		if (Vector3::Distance(position_, next_destination_) <= 25.0f)
		{
			interval = state_timer_ + 60.0f;	// ���̍s����1�b��Ɏ��s
			is_moving_ = false;					// �ړ�����
		}

		// �s���̈ێ����Ԃ𒴂�����A���̍s���𒊑I
		if (move_timer_ >= state_time_)
		{
			next_move();
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
		if (state_timer_ >= interval)
		{
			// �v���C���[��Ǐ]���Ă����ꍇ�A�U���s���Ɉڍs�i�U���s���͍ő�2��܂ŘA���j
			if (is_following_player_ && attack_count_ < 2)
			{
				change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
			}
			else
			{
				next_move();	// ���̍s���𒊑I
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
		next_move();
	}
}

// �U����Ԃł̍X�V
void Ghoul::attack(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= mesh_.motion_end_time() && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 10.0f + Vector3(0.0f, 12.5f, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, 8));
	}

	// ���[�V�����I����A���̍s���𒊑I
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		next_move();
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

// �n�ʂƂ̐ڐG����
void Ghoul::intersect_ground()
{
	// �t�B�[���h���擾
	auto& field = world_->field();
	// �n�ʂƂ̐ڐG�_
	Vector3 intersect;
	// �ڐG����p����
	Line line = Line(position_ + Vector3(0.0f, 2.5f, 0.0f), position_ - Vector3(0.0f, 1.0f, 0.0f));

	// �n�ʂƂ̐ڐG�_���擾
	if (field.collide_line(line.start, line.end, &intersect))
	{
		// �ڒn�����ꍇ�Ay�����W��␳����i�n�ʂɂ߂荞�܂Ȃ��j
		if (intersect.y >= position_.y)
		{
			velocity_.y = 0;			// y���ړ��ʂ�0�ɂ���
			position_.y = intersect.y;	// y���ʒu��␳
		}
	}
}

// �ǂƂ̐ڐG����
void Ghoul::intersect_wall()
{
	// �t�B�[���h���擾
	auto& field = world_->field();
	// �ǂƂ̐ڐG�_
	Vector3 intersect;
	// �ڐG����p����
	BoundingSphere sphere = BoundingSphere(position_ + Vector3(0.0f, 2.5f, 0.0f), 2.5f);

	// �ǂƂ̐ڐG�_���擾
	if (field.collide_sphere(sphere.position(), sphere.radius(), &intersect))
	{
		// �v���C���[�̍��W��␳
		position_.x = intersect.x;
		position_.z = intersect.z;
	}
}

// ���̍s��������
void Ghoul::next_move()
{
	// �����Ŏ��̍s��������
	int i = rand_.rand_int(0, 6);

	// �ҋ@�A�ړ��A�U����Ԃւ̈ڍs�m����1/6�A2/3�A1/6�ɂȂ��Ă���
	// �ҋ@��Ԃւ̈ڍs
	if (i == 0)
	{
		// �ҋ@�s���͘A���ɔ������Ȃ�
		if (previous_state_ == GhoulState::Idle)
		{
			next_move();
			return;
		}
		ready_to_next_state(2, 4);
		change_state(GhoulState::Idle, GhoulMotion::MOTION_IDLE);

		return;
	}
	// �ړ���Ԃւ̈ڍs
	else if (i < 5)
	{
		next_destination();			// ���̖ړI�n������
		ready_to_next_state(4, 8);	// �ړ���Ԃ�4�`8�b�Ԉێ�
		move_timer_ = 0.0f;			// �ړ���ԃ^�C�}�[�����Z�b�g
		is_moving_ = true;
		change_state(GhoulState::Move, GhoulMotion::MOTION_WALK);

		return;
	}
	// �U����Ԃւ̈ڍs
	else
	{
		if (attack_count_ >= 2)
		{
			next_move();
			return;
		}
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

	// i = 0�A�܂��̓v���C���[�͑��݂��Ȃ��ꍇ�A�����_���ō��W�𐶐�����
	if (i == 0 || player == nullptr)
	{
		// �v���C���[�Ǐ]������
		is_following_player_ = false;

		// x���̍��W�𐶐�;
		float x = rand_.rand_float(-135, 136);
		// z���̍��W�𐶐�
		float z = rand_.rand_float(-135, 136);
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

// �v���C���[�̈ʒu���擾
Vector3 Ghoul::get_player_position() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�A���ݎ����̍��W��Ԃ�
	if (player == nullptr) return position_;

	// �v���C���[�����݂���ꍇ�A���̍��W��Ԃ��iy�����͖�������j
	auto pos = player->position();
	pos.y = 0.0f;

	return pos;
}

// �v���C���[�ւ̊p�x���擾
float Ghoul::get_angle_to_target(Vector3 target) const
{
	// �ړI�n�����ւ̃x�N�g��
	Vector3 to_target = target - position_;
	// �O�����ƃ^�[�Q�b�g�̊O��
	Vector3 forward_cross_target = Vector3::Cross(rotation_.Forward(), to_target);
	// �O�����ƃ^�[�Q�b�g�̓���
	float forward_dot_target = Vector3::Dot(rotation_.Forward(), to_target);

	if (forward_cross_target.y >= 0.0f)
	{
		return Vector3::Angle(pose().Forward(), to_target);
	}
	else
	{
		return -Vector3::Angle(pose().Forward(), to_target);
	}
}

// �v���C���[�ւ̊p�x���擾�i���������j
float Ghoul::get_unsigned_angle_to_target(Vector3 target) const
{
	return std::abs(get_angle_to_target(target));
}

// �v���C���[�͑��݂��邩
bool Ghoul::player_exists() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��邩�ǂ�����Ԃ�
	return (player != nullptr);
}

// �v���C���[�͑O�ɂ��邩
bool Ghoul::player_in_forward() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (player == nullptr) return false;

	return (get_unsigned_angle_to_target(get_player_position()) <= 90.0f);
}



// �v���C���[�͍U���������ɂ��邩
bool Ghoul::player_in_range_distance() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (player == nullptr) return false;

	// ���g����v���C���[�܂ł̋��������߁A�U���������ł����True��Ԃ�
	return (Vector3::Distance(position_, get_player_position()) <= 25.0f);
}

// �v���C���[�͍U���ł���p�x�ɂ��邩
bool Ghoul::player_in_range_angle() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (player == nullptr) return false;

	// ���g����v���C���[�܂ł̊p�x�����߁A�U���p�x���ł����True��Ԃ�
	return (get_unsigned_angle_to_target(get_player_position()) <= 10.0f);
}

// �v���C���[���U���ł��邩
bool Ghoul::can_attack_player() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (!player_exists()) return false;
	// �v���C���[���O�ɂ��Ȃ���΁A�AFalse��Ԃ�
	if (!player_in_forward()) return false;
	// �v���C���[���U���ł��鋗�����ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_distance()) return false;
	// �v���C���[���U���ł���p�x���ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_angle()) return false;

	// ������S�Ė������Ă���΁ATrue��Ԃ�
	return true;
}

// ���̏�Ԃւ̈ڍs����
void Ghoul::ready_to_next_state(int min, int max)
{
	// �����Ŏ��̏�Ԉێ����Ԃ����߂�
	int i = rand_.rand_int(min, max);
	state_time_ = 60 * (float)i;
}