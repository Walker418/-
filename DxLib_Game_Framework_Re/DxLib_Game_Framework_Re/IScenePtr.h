#ifndef ISCENE_PTR_H_
#define ISCENE_PTR_H_

#include <memory>

// �|�C���^�F�V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class IScene;
using IScenePtr = std::shared_ptr<IScene>;

// �V�[���̐���
template<class T, class...Args>
inline IScenePtr new_scene(Args&&... args)
{
	return std::make_shared<T>(args...);
}

#endif // !ISCENE_PTR_H_