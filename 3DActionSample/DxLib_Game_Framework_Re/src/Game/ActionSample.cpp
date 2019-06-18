#include "ActionSample.h"

#include "../Graphic/Graphics3D.h"
#include "../Graphic/SkeletalMesh.h"
#include "../Math/Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"

#include "../Scene/Scene.h"
#include "../Scene/SceneLoading.h"
#include "../Scene/SceneTitle.h"
#include "../Scene/SceneGamePlay.h"

#include "WindowSetting.h"

// �N���X�F3D�A�N�V�����Q�[���T���v���i�A�v���P�[�V�����N���X�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
ActionSample::ActionSample() :
	Game{ WindowSetting::WindowWidth, WindowSetting::WindowHeight, false }
{ }

// �J�n
void ActionSample::start()
{
	// �V�[���N���X�̏�����
	scene_manager_.initialize();
	// �V�[����ǂݍ���
	scene_manager_.add(Scene::Loading, new_scene<SceneLoading>());		// �ǂݍ���
	scene_manager_.add(Scene::Title, new_scene<SceneTitle>());			// �^�C�g��
	scene_manager_.add(Scene::GamePlay, new_scene<SceneGamePlay>());	// �Q�[���v���C�V�[��

	// �ŏ��̃V�[���ֈڍs
	scene_manager_.change(Scene::Loading);
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