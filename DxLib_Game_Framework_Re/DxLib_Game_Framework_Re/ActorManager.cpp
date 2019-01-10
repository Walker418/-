#include "ActorManager.h"

// �N���X�F�A�N�^�[�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �A�N�^�[�̒ǉ�
void ActorManager::add(const ActorPtr& actor)
{
	actors_.push_front(actor);
}

// �X�V
void ActorManager::update(float delta_time)
{
	for (const auto& actor : actors_)
	{
		actor->update(delta_time);
	}
}

// �`��
void ActorManager::draw() const
{
	for (const auto& actor : actors_)
	{
		actor->draw();
	}
}

// �Փ˔���
void ActorManager::collide()
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i)
	{
		for (auto j = std::next(i); j != actors_.end(); ++j)
		{
			(*i)->collide(**j);
		}
	}
}

// �Փ˔���
void ActorManager::collide(Actor& other)
{
	for (const auto& actor : actors_)
	{
		other.collide(*actor);
	}
}

// �Փ˔���
void ActorManager::collide(ActorManager& other)
{
	for (const auto& actor : actors_)
	{
		other.collide(*actor);
	}
}

// �폜
void ActorManager::remove()
{
	// ���S�����A�N�^�[���폜
	actors_.remove_if([](const ActorPtr& actor) {return actor->is_dead(); });
}

// �A�N�^�[�̎擾
ActorPtr ActorManager::find(const std::string& name) const
{
	for (const auto& actor : actors_)
	{
		if (actor->name() == name)
		{
			return actor;
		}
	}
}

// �A�N�^�[���̎擾
unsigned int ActorManager::count() const
{
	return actors_.size();
}

// �A�N�^�[���X�g�̏���
void ActorManager::each(std::function<void(const ActorPtr&)> fn) const
{
	for (const auto& actor : actors_)
	{
		fn(actor);
	}
}

// ���b�Z�[�W����
void ActorManager::handle_message(EventMessage message, void* param)
{
	for (const auto& actor : actors_)
	{
		actor->handle_message(message, param);
	}
}

// ����
void ActorManager::clear()
{
	// �A�N�^�[���X�g�̓��e��S�ď���
	actors_.clear();
}