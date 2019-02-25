#include "Ghoul.h"
#include "IWorld.h"
#include "EventMessage.h"
#include "Field.h"
#include "Line.h"
#include "ActorGroup.h"
#include "Random.h"
#include "EnemyAttack.h"
#include "Damage.h"

// �N���X�F�O�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Ghoul::Ghoul(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "Ghoul", position, body),
	mesh_{ MESH_GHOUL, GhoulMotion::MOTION_IDLE },
	motion_{ GhoulMotion::MOTION_IDLE },
	state_{ GhoulState::Idle },
	state_timer_{ 0.0f },
	is_attack_{ false },
	is_following_player_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
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
		change_state(GhoulState::Death, MOTION_DEATH);

		return;
	}

	// ���ݗݐϒl�����ʂ��z������A���ݏ�ԂɈڍs
	if (current_wince_ >= ToWince && state_ != GhoulState::Wince)
	{
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
		current_hp_ -= damage->power;
		current_wince_ += damage->impact;

		return;
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

	is_attack_ = false;
}

// �ҋ@��Ԃł̍X�V
void Ghoul::idle(float delta_time)
{
	// 3�b��A���̍s���𒊑I
	if (state_timer_ >= 180.0f)
		next_move();
}

// �ړ���Ԃł̍X�V
void Ghoul::move(float delta_time)
{
	// �v���C���[��Ǐ]���̏ꍇ�A�펞�ړI�n�̍��W���v���C���[�̍��W�ɂ���
	if (is_following_player_)
	{
		next_destination_ = get_player_position();
	}

	// �ړI�n�Ɍ������Ĉړ�
	// �ړI�n�Ɍ���
	Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, next_destination_.Normalize(), Vector3::Up);	// �V����������ݒ�
	rotation_ = Matrix::Lerp(rotation_, new_rotation, RotateSpeed);		// ��Ԃŕ�����]������
	rotation_.NormalizeRotationMatrix();								// ��]�s��𐳋K��

	velocity_ = Vector3::Zero;						// �ړ��ʂ����Z�b�g
	velocity_ += rotation_.Forward() * WalkSpeed;	// �ړ����x�����Z
	position_ += velocity_ * delta_time;			// ���̈ʒu���v�Z

	// �v���C���[���U���͈͓��ɂ���΁A�U������
	if (can_attack_player())
	{
		change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
		return;
	}

	// 3�b��A���̍s���𒊑I
	if (state_timer_ >= 180.0f)
		next_move();
}

// ���ݏ�Ԃł̍X�V
void Ghoul::wince(float delta_time)
{
	// ���[�V�����I����A���̍s���𒊑I
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		current_wince_ = 0;
		// change_state(GhoulState::Move, MOTION_IDLE);
		next_move();
	}
}

// �U����Ԃł̍X�V
void Ghoul::attack(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= mesh_.motion_end_time() && !is_attack_)
	{
		is_attack_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 15.0f + Vector3(0.0f, 12.5f, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, 8));
	}

	// ���[�V�����I����A���̍s���𒊑I
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		// move(delta_time);
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
	Random rand = Random();
	rand.randomize();

	// �����Ŏ��̍s��������
	int i = rand.rand_int(0, 4);

	switch (i)
	{
	case 0:
		// �ҋ@�s���͘A���ɔ������Ȃ�
		if (previous_state_ == GhoulState::Idle)
		{
			next_move();
			return;
		}
		change_state(GhoulState::Idle, GhoulMotion::MOTION_IDLE);
		break;
	case 1:
		next_destination();		// ���̖ړI�n������
		change_state(GhoulState::Move, GhoulMotion::MOTION_WALK);
		break;
	case 2:
		next_destination();		// ���̖ړI�n������
		change_state(GhoulState::Move, GhoulMotion::MOTION_WALK);
		break;
	case 3:
		change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
		break;
	default:
		next_move();
		break;
	}
}

// ���̖ړI�n������
void Ghoul::next_destination()
{
	Random rand = Random();
	rand.randomize();

	int i = rand.rand_int(0, 3);

	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// i = 1�A�܂��̓v���C���[�͑��݂��Ȃ��ꍇ�A�����_���ō��W�𐶐�����
	if (i == 1 || player == nullptr)
	{
		// �v���C���[�Ǐ]������
		is_following_player_ = false;

		// x���̍��W�𐶐�
		rand.randomize();
		float x = rand.rand_float(-100, 101);
		// z���̍��W�𐶐�
		rand.randomize();
		float z = rand.rand_float(-100, 101);
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

	// �v���C���[�����݂��Ȃ��ꍇ�A���݂̍��W��Ԃ�
	if (player == nullptr) return position_;

	// �v���C���[�����݂���ꍇ�A���̍��W��Ԃ�
	return player->position();
}

// �v���C���[�ւ̊p�x���擾
float Ghoul::get_angle_to_player() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�A0����Ԃ�
	if (player == nullptr) return 0.0f;

	return Vector3::Angle(position_ + pose().Forward(), player->position());
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

	// ���ςŃv���C���[����������𔻒�
	Vector3 to_target = player->position() - position_;								// �v���C���[�����̃x�N�g��
	float forward_dot_target = Vector3::Dot(rotation_.Forward(), to_target);		// �O�����ƃv���C���[�̓���

	// �O�����ƃv���C���[�̓��ς�0�ȏ�ł���΁ATrue��Ԃ�
	return (forward_dot_target >= 0.0f);
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
	return (get_angle_to_player() <= 10.0f);
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