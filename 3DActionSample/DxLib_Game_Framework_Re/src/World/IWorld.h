#ifndef IWORLD_H_
#define IWORLD_H_

#include "../Actor/ActorPtr.h"
#include <string>
#include <functional>

// �C���^�[�t�F�[�X�F���[���h
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

enum class ActorGroup;
enum class EventMessage;
class Field;

class IWorld
{
public:
	// ���z�f�X�g���N�^
	virtual ~IWorld() { }
	// �A�N�^�[�̒ǉ�
	virtual void add_actor(ActorGroup group, const ActorPtr& actor) = 0;
	// �A�N�^�[�̎擾
	virtual ActorPtr find_actor(ActorGroup group, const std::string& name) const = 0;
	// �A�N�^�[���̎擾
	virtual unsigned int count_actor(ActorGroup group) const = 0;
	// �A�N�^�[���X�g�̏���
	virtual void each_actor(ActorGroup group, std::function<void(const ActorPtr&)> fn) const = 0;
	// ���b�Z�[�W�̑��M
	virtual void send_message(EventMessage message, void* param = nullptr) = 0;
	// �t�B�[���h�̎擾
	virtual Field& field() = 0;
	// �J�����̎擾
	virtual ActorPtr camera() = 0;
};

#endif // !IWORLD_H_