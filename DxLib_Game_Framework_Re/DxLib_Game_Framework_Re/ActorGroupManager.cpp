#include "ActorGroupManager.h"

// �N���X�F�A�N�^�[�O���[�v�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �O���[�v�̒ǉ�
void ActorGroupManager::add_group(ActorGroup group)
{
	actor_group_map_[group].clear();
}

// �A�N�^�[�̒ǉ�
void ActorGroupManager::add_actor(ActorGroup group, const ActorPtr& actor)
{
	actor_group_map_[group].add(actor);
}

// �X�V
void ActorGroupManager::update(float delta_time)
{
	for (auto& pair : actor_group_map_)
	{
		pair.second.update(delta_time);
	}
}

// �`��
void ActorGroupManager::draw() const
{
	for (auto& pair : actor_group_map_)
	{
		draw(pair.first);
	}
}

// �`��
void ActorGroupManager::draw(ActorGroup group) const
{
	actor_group_map_.at(group).draw();
}

// ����
void ActorGroupManager::clear()
{
	actor_group_map_.clear();
}

// �A�N�^�[�̎擾
ActorPtr ActorGroupManager::find(ActorGroup group, const std::string& name) const
{
	return actor_group_map_.at(group).find(name);
}

// �A�N�^�[���̎擾
unsigned int ActorGroupManager::count(ActorGroup group) const
{
	return actor_group_map_.at(group).count();
}

// �A�N�^�[���X�g�̏���
void ActorGroupManager::each(ActorGroup group, std::function<void(const ActorPtr&)> fn) const
{
	actor_group_map_.at(group).each(fn);
}

// �Փ˔���
void ActorGroupManager::collide(ActorGroup group1, ActorGroup group2)
{
	actor_group_map_[group1].collide(actor_group_map_[group2]);
}

// �폜
void ActorGroupManager::remove()
{
	for (auto& pair : actor_group_map_)
	{
		pair.second.remove();
	}
}

// ���b�Z�[�W����
void ActorGroupManager::handle_message(EventMessage message, void* param)
{
	for (auto& pair : actor_group_map_)
	{
		pair.second.handle_message(message, param);
	}
}