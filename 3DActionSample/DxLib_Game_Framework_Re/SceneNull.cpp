#include "SceneNull.h"
#include "Scene.h"

// �N���X�F�󔒂̃V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �J�n
void SceneNull::start()
{
	// �������Ȃ�
}

// �X�V
void SceneNull::update(float delta_time)
{
	// �������Ȃ�
}

// �`��
void SceneNull::draw() const
{
	// �������Ȃ�
}

// �I�����Ă��邩
bool SceneNull::is_end() const
{
	// False��Ԃ�
	return false;
}

// ���̃V�[���̎擾
Scene SceneNull::next() const
{
	// �󔒂̃V�[����Ԃ��i�V�[���J�ڂ͂Ȃ��j
	return Scene::None;
}

// �I��
void SceneNull::end()
{
	// �������Ȃ�
}