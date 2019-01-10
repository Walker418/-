#ifndef ACTOR_PTR_H_
#define ACTOR_PTR_H_

#include <memory>

// ポインタ：アクター
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Actor;
using ActorPtr = std::shared_ptr<Actor>;

// アクターの生成
template<class T, class... Args>
inline ActorPtr new_actor(Args&&... args)
{
	return std::make_shared<T>(args...);
}

#endif // !ACTOR_PTR_H_