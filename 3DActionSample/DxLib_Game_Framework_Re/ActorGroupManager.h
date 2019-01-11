#ifndef ACTOR_GROUP_MANAGER_H_
#define ACTOR_GROUP_MANAGER_H_

#include "ActorManager.h"
#include <map>
#include <functional>

// �N���X�F�A�N�^�[�O���[�v�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

enum class ActorGroup;

class ActorGroupManager
{
private:
	// �A�N�^�[�O���[�v�}�b�v
	using ActorGroupMap = std::map<ActorGroup, ActorManager>;

public:
	// �f�t�H���g�R���X�g���N�^
	ActorGroupManager() = default;
	// �O���[�v�̒ǉ�
	void add_group(ActorGroup group);
	// �A�N�^�[�̒ǉ�
	void add_actor(ActorGroup group, const ActorPtr& actor);
	// �X�V
	void update(float delta_time);
	// �`��
	void draw() const;
	// �`��
	void draw(ActorGroup group) const;
	// ����
	void clear();
	// �A�N�^�[�̎擾
	ActorPtr find(ActorGroup group, const std::string& name) const;
	// �A�N�^�[���̎擾
	unsigned int count(ActorGroup group) const;
	// �A�N�^�[���X�g�̏���
	void each(ActorGroup group, std::function<void(const ActorPtr&)> fn) const;
	// �Փ˔���
	void collide(ActorGroup group1, ActorGroup group2);
	// �폜
	void remove();
	// ���b�Z�[�W����
	void handle_message(EventMessage message, void* param);

	// �R�s�[�֎~
	ActorGroupManager(const ActorGroupManager& other) = delete;
	ActorGroupManager& operator = (const ActorGroupManager& other) = delete;

private:
	// �A�N�^�[�O���[�v�}�b�v
	ActorGroupMap actor_group_map_;
};

#endif // !ACTOR_GROUP_MANAGER_H_