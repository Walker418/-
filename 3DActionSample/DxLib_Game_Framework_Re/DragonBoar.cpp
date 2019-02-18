#include "DragonBoar.h"
#include "IWorld.h"
#include "EventMessage.h"
#include "Field.h"
#include "Line.h"
#include "ActorGroup.h"
#include "Random.h"
#include "EnemyAttack.h"
#include "Damage.h"

// �N���X�F�G�i�C�m�V�V�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "DragonBoar", position, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_ROAR },
	motion_{ DragonBoarMotion::MOTION_ROAR },
	state_{ DragonBoarState::Roar },
	state_timer_{ 0.0f },
	is_attack_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
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
		change_state(DragonBoarState::Death, MOTION_DEATH);

		return;
	}

	// ���ݗݐϒl�����ʂ��z������A���ݏ�ԂɈڍs
	if (current_wince_ >= ToWince && state_ != DragonBoarState::Wince)
	{
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
		current_hp_ -= damage->power;
		current_wince_ += damage->impact;

		return;
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
	case(DragonBoarState::Move):
		move(delta_time);
		break;
	case(DragonBoarState::Attack):
		attack(delta_time);
		break;
	case(DragonBoarState::Dash):
		dash(delta_time);
		break;
	case(DragonBoarState::Roar):
		roar(delta_time);
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
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	is_attack_ = false;
}

// �ҋ@��Ԃł̍X�V
void DragonBoar::idle(float delta_time)
{
	// �ҋ@3�b��A���̏�Ԃֈڍs
	if (state_timer_ >= 180.0f)
	{
		change_state(DragonBoarState::Roar, MOTION_ROAR);
	}
}

// �ړ���Ԃł̍X�V
void DragonBoar::move(float delta_time)
{
	// �v���C���[���U���͈͓��ɋ���΁A���݂��U�����s��
	if (can_attack_player())
	{
		change_state(DragonBoarState::Attack, MOTION_BITE);

		return;
	}

	// ============================================================
	// �ȉ��͈ړ�����
	
	// �������Ă��Ȃ��ꍇ�A�ҋ@���[�V�����ɕύX
	int motion{ MOTION_IDLE };

	Vector3 next_position = get_player_position();	// �v���C���[�̈ʒu���擾
	next_position.y = 0.0f;							// �v���C���[���W�̍����𖳎�

	// �v���C���[�Ɍ�����
	Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, next_position.Normalize(), Vector3::Up);	// �V����������ݒ�
	rotation_ = Matrix::Lerp(rotation_, new_rotation, RotateSpeed);		// ��Ԃŕ�����]������
	/*
	// �����Ă�������ֈړ�
	velocity_ = Vector3::Zero;						// �ړ��ʂ����Z�b�g
	velocity_ += rotation_.Forward() * WalkSpeed;	// �ړ����x�����Z
	position_ += velocity_ * delta_time;			// ���̈ʒu���v�Z

	// �ړ����Ă���΁A�ړ����[�V�����ɕύX
	if (velocity_.x != 0.0f || velocity_.z != 0.0f)
		motion = MOTION_WALK;
		*/
	// ��Ԃ��X�V
	change_state(DragonBoarState::Move, motion);

	// �ړ������I��
	// ============================================================
}

// �U����Ԃł̍X�V
void DragonBoar::attack(float delta_time)
{
	// �U������𔭐�
	if (state_timer_ >= mesh_.motion_end_time() && !is_attack_)
	{
		is_attack_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 43.0f + Vector3(0.0f, 12.5f, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, 20));
	}

	// ���[�V�����I����A�ړ���Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		move(delta_time);
	}
}

// �ːi��Ԃł̍X�V
void DragonBoar::dash(float delta_time)
{

}

// ���K��Ԃł̍X�V
void DragonBoar::roar(float delta_time)
{
	// ���[�V�����I����A�ړ���Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		move(delta_time);
	}
}

// ���ݏ�Ԃł̍X�V
void DragonBoar::wince(float delta_time)
{
	// ���[�V�����I����A�ړ���Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		current_wince_ = 0;
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		move(delta_time);
	}
}

// ���S��Ԃł̍X�V
void DragonBoar::death(float delta_time)
{
	// ���[�V�������I������ƁA���S�����L����
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		world_->send_message(EventMessage::EnemyDead);
		die();
	}
}

// �v���C���[�͑O�ɂ��邩
bool DragonBoar::player_in_front() const
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
bool DragonBoar::player_in_range_distance() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (player == nullptr) return false;

	// ���g����v���C���[�܂ł̋��������߁A�U���������ł����True��Ԃ�
	return (Vector3::Distance(position_, get_player_position()) <= 50.0f);
}

// �v���C���[���U���ł���p�x�ɂ��邩
bool DragonBoar::player_in_range_angle() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (player == nullptr) return false;

	// ���g����v���C���[�܂ł̊p�x�����߁A�U���p�x���ł����True��Ԃ�
	return (get_angle_to_player() <= 10.0f);
}

// �v���C���[���U���ł��邩
bool DragonBoar::can_attack_player() const
{
	// �v���C���[���O�ɂ��Ȃ���΁A�AFalse��Ԃ�
	if (!player_in_front()) return false;
	// �v���C���[���U���ł��鋗�����ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_distance()) return false;
	// �v���C���[���U���ł���p�x���ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_angle()) return false;

	// ������S�Ė������Ă���΁ATrue��Ԃ�
	return true;
}

// ���̍s��������
void DragonBoar::next_move()
{
	// ����������ŗ����𐶐����A���̍s�������߂�
	Random rand = Random();
	rand.randomize();

	int i = rand.rand(0, 10);


}

// �v���C���[�̈ʒu���擾
Vector3 DragonBoar::get_player_position() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�A���݂̍��W��Ԃ�
	if (player == nullptr) return position_;

	// �v���C���[�����݂���ꍇ�A���̍��W��Ԃ�
	return player->position();
}

// �v���C���[�ւ̊p�x���擾
float DragonBoar::get_angle_to_player() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�A0����Ԃ�
	if (player == nullptr) return 0.0f;

	// return Vector3::Angle(rotation_.Forward(), player->position());
	return Vector3::Angle(position_ + pose().Forward(), player->position());
}

// �n�ʂƂ̐ڐG����
void DragonBoar::intersect_ground()
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
void DragonBoar::intersect_wall()
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