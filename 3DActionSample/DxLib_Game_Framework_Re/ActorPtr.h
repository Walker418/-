#ifndef ACTOR_PTR_H_
#define ACTOR_PTR_H_

#include <memory>

// �|�C���^�F�A�N�^�[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Actor;
using ActorPtr = std::shared_ptr<Actor>;

// �A�N�^�[�̐���
template<class T, class... Args>
inline ActorPtr new_actor(Args&&... args)
{
	return std::make_shared<T>(args...);
}

#endif // !ACTOR_PTR_H_