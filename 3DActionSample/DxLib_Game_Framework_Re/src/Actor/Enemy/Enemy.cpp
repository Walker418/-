#include "Enemy.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../Body/Line.h"
#include "../Body/BoundingSphere.h"
#include "../ActorGroup.h"
#include "../../Math/MathHelper.h"

// �R���X�g���N�^
Enemy::Enemy(IWorld* world, const std::string& name, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, name, position, body),
	interval_{ 0.0f }
{
	rotation_ = Matrix::CreateRotationY(angle);
}

// �n�ʂƂ̐ڐG����
void Enemy::intersect_ground()
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
void Enemy::intersect_wall()
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

// ���W�̐���
void Enemy::clamp_position()
{
	// �t�B�[���h���擾
	auto& field = world_->field();
	// �t�B�[���h�̍ő�ƍŏ����W���擾
	auto max_pos = field.max_position();
	auto min_pos = field.min_position();

	position_.x = MathHelper::clamp(position_.x, min_pos.x, max_pos.x);
	position_.z = MathHelper::clamp(position_.z, min_pos.z, max_pos.z);
}

// �v���C���[���擾
ActorPtr Enemy::get_player()
{
	return world_->find_actor(ActorGroup::Player, "Player");
}

// �v���C���[�̈ʒu���擾
Vector3 Enemy::get_player_position()
{
	// �v���C���[�����݂��Ȃ��ꍇ�A���ݎ����̍��W��Ԃ�
	if (get_player() == nullptr) return position_;

	// �v���C���[�����݂���ꍇ�A���̍��W��Ԃ��iy�����͖�������j
	auto pos = get_player()->position();
	pos.y = 0.0f;

	return pos;
}

// �ڕW�ւ̊p�x���擾�i�����t���j
float Enemy::get_angle_to_target(Vector3 target) const
{
	// �ړI�n�����ւ̃x�N�g��
	Vector3 to_target = target - position_;
	// �O�����ƃ^�[�Q�b�g�̊O��
	Vector3 forward_cross_target = Vector3::Cross(rotation_.Forward(), to_target);

	// �O�ςŖڕW�ւ̊p�x���v�Z���A�p�x�̒l��Ԃ�
	float angle = (forward_cross_target.y >= 0.0f) ? Vector3::Angle(pose().Forward(), to_target) : -Vector3::Angle(pose().Forward(), to_target);

	return angle;
}

// �ڕW�ւ̊p�x���擾�i���������j
float Enemy::get_unsigned_angle_to_target(Vector3 target) const
{
	return std::abs(get_angle_to_target(target));
}

// �v���C���[�͑��݂��邩
bool Enemy::player_exists()
{
	return get_player() != nullptr;
}

// �v���C���[�͑O�ɂ��邩
bool Enemy::player_in_forward()
{
	// �v���C���[�����݂��Ȃ��ꍇ�AFalse��Ԃ�
	if (get_player() == nullptr) return false;

	// �v���C���[�ւ̊p�x��90���ȓ����ǂ�����Ԃ�
	return (get_unsigned_angle_to_target(get_player_position()) <= 90.0f);
}

// �v���C���[�͍U���������ɂ��邩
bool Enemy::player_in_range_distance(float distance)
{
	// �v���C���[�����݂��Ȃ��ꍇ�AFalse��Ԃ�
	if (get_player() == nullptr) return false;

	// ���g����v���C���[�܂ł̋��������߁A�U���������ł����True��Ԃ�
	return (Vector3::Distance(position_, get_player_position()) <= distance);
}

// �v���C���[�͍U���ł���p�x�ɂ��邩
bool Enemy::player_in_range_angle(float angle)
{
	// �v���C���[�����݂��Ȃ��ꍇ�AFalse��Ԃ�
	if (get_player() == nullptr) return false;

	// ���g����v���C���[�܂ł̊p�x�����߁A�U���p�x���ł����True��Ԃ�
	return (get_unsigned_angle_to_target(get_player_position()) <= angle);
}