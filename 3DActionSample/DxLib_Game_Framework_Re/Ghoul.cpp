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
	is_attack_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
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

	DrawLine3D(position_, position_ + pose().Forward() * 50.0f, Cr);
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
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	is_attack_ = false;
}

// �ҋ@��Ԃł̍X�V
void Ghoul::idle(float delta_time)
{

}

// �ړ���Ԃł̍X�V
void Ghoul::move(float delta_time)
{

}

// ���ݏ�Ԃł̍X�V
void Ghoul::wince(float delta_time)
{
	// ���[�V�����I����A�ړ���Ԃɖ߂�
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		current_wince_ = 0;
		change_state(GhoulState::Move, MOTION_IDLE);
	}
}

// �U����Ԃł̍X�V
void Ghoul::attack(float delta_time)
{

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