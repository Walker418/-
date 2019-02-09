#include "DragonBoar.h"
#include "IWorld.h"
#include "EventMessage.h"
#include "Field.h"
#include "Line.h"
#include "ActorGroup.h"
#include "Random.h"

// �N���X�F�G�i�C�m�V�V�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
// �R���X�g���N�^
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "DragonBoar", position, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_ROAR },
	motion_{ DragonBoarMotion::MOTION_ROAR },
	state_{ DragonBoarState::Roar },
	state_timer_{ 0.0f }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
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
	}
}

// �`��
void DragonBoar::draw() const
{
	mesh_.draw();	// ���b�V����`��

	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
	body_->transform(pose())->draw();

	/*
	// �f�o�b�O���b�Z�[�W
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);

	if (can_attack_player())
	{
		DrawString(0, 0, "�U���ł���", Cr);
	}
	else
	{
		DrawString(0, 0, "�U���ł��Ȃ�", Cr);
	}
	*/
}

// �Փ˃��A�N�V����
void DragonBoar::react(Actor& other)
{
	
}

// ���b�Z�[�W����
void DragonBoar::handle_message(EventMessage message, void* param)
{

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
	case(DragonBoarState::Roar):
		roar(delta_time);
		break;
	case(DragonBoarState::Damage):
		damage(delta_time);
		break;
	case(DragonBoarState::Death):
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// ��ԃ^�C�}�[�̉��Z
}

// ��Ԃ̕ύX
void DragonBoar::change_state(DragonBoarState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;
}

// �ҋ@��Ԃł̍X�V
void DragonBoar::idle(float delta_time)
{
	// 3�b��A���̏�Ԃֈڍs
	if (state_timer_ >= 180.0f)
	{
		change_state(DragonBoarState::Roar, MOTION_ROAR);
	}
}

// �ړ���Ԃł̍X�V
void DragonBoar::move(float delta_time)
{
	
}

// �U����Ԃł̍X�V
void DragonBoar::attack(float delta_time)
{

}

// ���K��Ԃł̍X�V
void DragonBoar::roar(float delta_time)
{
	// ���[�V�����I����A�ҋ@��Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		change_state(DragonBoarState::Idle, MOTION_IDLE);
	}
}

// ���ݏ�Ԃł̍X�V
void DragonBoar::damage(float delta_time)
{
	// ���[�V�����I����A�ړ���Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		change_state(DragonBoarState::Move, MOTION_IDLE);
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

// �v���C���[�͍U���������ɂ��邩
bool DragonBoar::player_in_range_distance() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (player == nullptr) return false;

	// ���g����v���C���[�܂ł̋��������߁A�U���������ł����True��Ԃ�
	return (Vector3::Distance(position_, player->position()) <= 50.0f);
}

// �v���C���[���U���ł���p�x�ɂ��邩
bool DragonBoar::player_in_range_angle() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (player == nullptr) return false;

	// ���g����v���C���[�܂ł̊p�x�����߁A�U���p�x���ł����True��Ԃ�
	return (Vector3::Angle(rotation_.Forward(), player->position()) <= 25.0f);
}

// �v���C���[���U���ł��邩
bool DragonBoar::can_attack_player() const
{
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

	int i = rand.rand(0, 6);
	switch (i)
	{

	default:
		break;
	}
}

// ���̖ړI�n���擾
Vector3 DragonBoar::next_destination() const
{
	// �v���C���[�̎Q�Ƃ��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// �v���C���[�����݂��Ȃ��ꍇ�A���݂̍��W��Ԃ�
	if (player == nullptr) return position_;

	// �v���C���[�����݂���ꍇ�A���̍��W��Ԃ�
	return player->position();
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