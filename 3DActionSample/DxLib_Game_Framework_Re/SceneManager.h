#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include "IScenePtr.h"
#include <unordered_map>

// �N���X�F�V�[���Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

enum class Scene;

class SceneManager
{
public:
	// �R���X�g���N�^
	SceneManager();
	// ������
	void initialize();
	// �X�V
	void update(float delta_time);
	// �`��
	void draw() const;
	// �I��
	void end();
	// �V�[���̒ǉ�
	void add(Scene name, const IScenePtr& scene);
	// �V�[���̕ύX
	void change(Scene name);

	// �R�s�[�֎~
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;

private:
	// �V�[�����X�g
	std::unordered_map<Scene, IScenePtr> scenes_;
	// ���݂̃V�[��
	IScenePtr current_scene_;
};

#endif // !SCENE_MANAGER_H_