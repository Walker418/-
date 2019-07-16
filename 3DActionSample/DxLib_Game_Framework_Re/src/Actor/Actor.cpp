#include "Actor.h"

// �N���X�F�A�N�^�[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Actor::Actor() :
	world_{ nullptr }, name_{ "null" }, position_{ Vector3::Zero }, body_{ std::shared_ptr<DummyBody>() }
{ }

// �R���X�g���N�^
Actor::Actor(IWorld* world, const std::string& name, const Vector3& position, const IBodyPtr& body) :
	world_{ world }, name_{ name }, position_{ position }, body_{ body }
{ }

// ���z�f�X�g���N�^
Actor::~Actor()
{ }

// �X�V
void Actor::update(float delta_time)
{ }

// �`��
void Actor::draw() const
{ }

// �Փ˃��A�N�V����
void Actor::react(Actor& other)
{ }

// ���b�Z�[�W����
void Actor::handle_message(EventMessage message, void* param)
{ }

// �Փ˔���
void Actor::collide(Actor& other)
{
	if (is_collided(other))
	{
		react(other);
		other.react(*this);
	}
}

// ���S
void Actor::die()
{
	is_dead_ = true;
}

// �Փ˂��Ă��邩
bool Actor::is_collided(const Actor& other) const
{
	return body()->is_collided(*other.body());
}

// ���S���Ă��邩
bool Actor::is_dead() const
{
	return is_dead_;
}

// �O������̈ړ��w��
void Actor::move_order(Vector3 vector)
{
	position_ += vector;
}

// ���O�̎擾
const std::string& Actor::name() const
{
	return name_;
}

// ���W�̎擾
Vector3 Actor::position() const
{
	return position_;
}

// ��]�s��̎擾
Matrix Actor::rotation() const
{
	return rotation_;
}

// �ϊ��s��̎擾
Matrix Actor::pose() const
{
	return Matrix(rotation_).Translation(position_);
}

// �Փ˔���f�[�^�̎擾
IBodyPtr Actor::body() const
{
	return body_->transform(pose());
}

// �̗͂̎擾
int Actor::get_HP()
{
	return 0;
}

Vector3 Actor::get_velocity()
{
	// �ړ��ʂ̎擾
	return Vector3();
}
