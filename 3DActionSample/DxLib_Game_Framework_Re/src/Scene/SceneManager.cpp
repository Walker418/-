#include "SceneManager.h"
#include "SceneNull.h"

// �N���X�F�V�[���Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
// �ŏ��͋�̃V�[����ǉ�
SceneManager::SceneManager() :
	current_scene_{ new_scene<SceneNull>() }
{ }

// ������
void SceneManager::initialize()
{
	end();				// �I�����Ɠ�������
	scenes_.clear();	// �V�[�����X�g���N���A
}

// �X�V
void SceneManager::update(float delta_time)
{
	// ���݂̃V�[�����X�V
	current_scene_->update(delta_time);

	// ���݂̃V�[�����I��������A���̃V�[���ֈڍs
	if (current_scene_->is_end())
	{
		change(current_scene_->next());
	}
}

// �`��
void SceneManager::draw() const
{
	current_scene_->draw();
}

// �I��
void SceneManager::end()
{
	// ���݂̃V�[�����I�����A��̃V�[����u��������
	current_scene_->end();
	current_scene_ = new_scene<SceneNull>();
}

// �V�[���̒ǉ�
void SceneManager::add(Scene name, const IScenePtr& scene)
{
	scenes_[name] = scene;
}

// �V�[���̕ύX
void SceneManager::change(Scene name)
{
	end();								// ���݂̃V�[�����I��
	current_scene_ = scenes_[name];		// ���݂̃V�[���Ɉڍs����V�[����o�^
	current_scene_->start();			// ���̃V�[�����J�n
}