#ifndef ISCENE_PTR_H_
#define ISCENE_PTR_H_

#include <memory>

// ポインタ：シーン
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class IScene;
using IScenePtr = std::shared_ptr<IScene>;

// シーンの生成
template<class T, class...Args>
inline IScenePtr new_scene(Args&&... args)
{
	return std::make_shared<T>(args...);
}

#endif // !ISCENE_PTR_H_