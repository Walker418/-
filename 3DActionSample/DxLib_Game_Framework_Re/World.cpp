#include "World.h"
#include "Field.h"
#include "ActorGroup.h"
#include "Actor.h"

// �N���X�F���[���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
World::World()
{
	// �������������s��
	initialize();
}

// ������
void World::initialize()
{
	// �S�Ẵf�[�^������
	clear();
	// �A�N�^�[�O���[�v���A�N�^�[�O���[�v�}�l�[�W���[�ɒǉ�
	actors_.add_group(ActorGroup::None);			// ��������
	actors_.add_group(ActorGroup::Player);			// �v���C���[
	actors_.add_group(ActorGroup::PlayerAttack);	// �v���C���[�̍U��
	actors_.add_group(ActorGroup::Enemy);			// �G
	actors_.add_group(ActorGroup::EnemyAttack);		// �G�̍U��
	actors_.add_group(ActorGroup::Effect);			// �G�t�F�N�g
}

// �X�V
void World::update(float delta_time)
{
	// �e�A�N�^�[�̏�Ԃ��X�V
	actors_.update(delta_time);
	// �ڐG������s��
	actors_.collide(ActorGroup::Player, ActorGroup::Enemy);
	actors_.collide(ActorGroup::Player, ActorGroup::EnemyAttack);
	actors_.collide(ActorGroup::PlayerAttack, ActorGroup::Enemy);
	// ���S�����A�N�^�[���폜
	actors_.remove();

	// �J�����̏�Ԃ��X�V
	camera_->update(delta_time);
	// ���C�g�̏�Ԃ��X�V
	light_->update(delta_time);
}

// �`��
void World::draw() const
{
	camera_->draw();
	light_->draw();
	field_->draw();
	actors_.draw();
}

// ���b�Z�[�W����
void World::handle_message(EventMessage message, void* param)
{
	// ���[���h�̃��b�Z�[�W����
	listener_(message, param);

	// �A�N�^�[�̃��b�Z�[�W����
	actors_.handle_message(message, param);
	camera_->handle_message(message, param);
	light_->handle_message(message, param);
}

// ���b�Z�[�W���X�i�[�̓o�^
void World::add_event_message_listener(EventMessageListener listener)
{
	listener_ = listener;
}

// �t�B�[���h�̒ǉ�
void World::add_field(const FieldPtr& field)
{
	field_ = field;
}

// �J�����̒ǉ�
void World::add_camera(const ActorPtr& camera)
{
	camera_ = camera;
}

// ���C�g�̒ǉ�
void World::add_light(const ActorPtr& light)
{
	light_ = light;
}

// �S�f�[�^�̏���
void World::clear()
{
	actors_.clear();
	field_ = nullptr;
	light_ = nullptr;
	camera_ = nullptr;
	listener_ = [](EventMessage, void*) {};
}

// �A�N�^�[��ǉ�
void World::add_actor(ActorGroup group, const ActorPtr& actor)
{
	actors_.add_actor(group, actor);
}

// �A�N�^�[�̎擾
ActorPtr World::find_actor(ActorGroup group, const std::string& name) const
{
	return actors_.find(group, name);
}

// �A�N�^�[���̎擾
unsigned int World::count_actor(ActorGroup group) const
{
	return actors_.count(group);
}

// �A�N�^�[���X�g�̏���
void World::each_actor(ActorGroup group, std::function<void(const ActorPtr&)> fn) const
{
	actors_.each(group, fn);
}

// ���b�Z�[�W�̑��M
void World::send_message(EventMessage message, void * param)
{
	handle_message(message, param);
}

// �t�B�[���h�̎擾
Field& World::field()
{
	return *field_;
}

// �J�����̎擾
ActorPtr World::camera()
{
	return camera_;
}
