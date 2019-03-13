#include "ActionSample.h"

#include "../Graphic/Graphics3D.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math/Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneLoadGamePlay.h"
#include "../Scene/SceneGamePlay.h"

// �N���X�F3D�A�N�V�����Q�[���T���v���i�A�v���P�[�V�����N���X�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
ActionSample::ActionSample() :
	Game{ 1024, 768, false }
{ }

// �J�n
void ActionSample::start()
{
	// �V�[���N���X�̏�����
	scene_manager_.initialize();
	// �V�[����ǂݍ���
	scene_manager_.add(Scene::LoadGamePlay, new_scene<SceneLoadGamePlay>());	// �Q�[���v���C�V�[���̓ǂݍ���
	scene_manager_.add(Scene::GamePlay, new_scene<SceneGamePlay>());			// �Q�[���v���C�V�[��

	// �ŏ��̃V�[���ֈڍs
	scene_manager_.change(Scene::LoadGamePlay);
}

// �X�V
void ActionSample::update(float delta_time)
{
	scene_manager_.update(delta_time);
}

// �`��
void ActionSample::draw()
{
	scene_manager_.draw();
}

// �I��
void ActionSample::end()
{
	// �V�[���}�l�[�W���[���I��
	scene_manager_.end();
}