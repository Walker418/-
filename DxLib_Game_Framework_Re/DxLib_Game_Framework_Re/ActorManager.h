#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include "Actor.h"
#include "ActorPtr.h"
#include <list>
#include <functional>

// �N���X�F�A�N�^�[�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

enum class EventMessage;

class ActorManager
{
private:
	// �A�N�^�[���X�g
	using ActorList = std::list<ActorPtr>;

public:
	// �f�t�H���g�R���X�g���N�^
	ActorManager() = default;
	// �A�N�^�[�̒ǉ�
	void add(const ActorPtr& actor);
	// �X�V
	void update(float delta_time);
	// �`��
	void draw() const;
	// �Փ˔���
	void collide();
	// �Փ˔���
	void collide(Actor& other);
	// �Փ˔���
	void collide(ActorManager& other);
	// �폜
	void remove();
	// �A�N�^�[�̎擾
	ActorPtr find(const std::string& name) const;
	// �A�N�^�[���̎擾
	unsigned int count() const;
	// �A�N�^�[���X�g�̏���
	void each(std::function<void(const ActorPtr&)>fn) const;
	// ���b�Z�[�W����
	void handle_message(EventMessage message, void* param);
	// ����
	void clear();

	// �R�s�[�֎~
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

private:
	// �A�N�^�[���X�g
	ActorList actors_;
};

#endif // !ACTOR_MANAGER_H_