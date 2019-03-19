#include "SceneTitle.h"
#include "Scene.h"
#include "../ID/SourceID.h"

#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../Actor/Camera/TitleCamera.h"
#include "../Graphic/Light.h"

// �J�n
void SceneTitle::start()
{
	// �I���t���O��False�ɂ���
	is_end_ = false;
	// ���[���h��������
	world_.initialize();

	// �t�B�[���h��ǉ�
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// �J������ǉ�
	world_.add_camera(new_actor<TitleCamera>(&world_, Vector3::Zero));
	// ���C�g��ǉ�
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));
}

// �X�V
void SceneTitle::update(float delta_time)
{
	world_.update(delta_time);
}

// �`��
void SceneTitle::draw() const
{
	world_.draw();
}

// �I�����Ă��邩
bool SceneTitle::is_end() const
{
	// �I���t���O��Ԃ�
	return is_end_;
}

// ���̃V�[���̎擾
Scene SceneTitle::next() const
{
	return Scene::GamePlay;
}

// �I��
void SceneTitle::end()
{
	
}

// ���b�Z�[�W����
void SceneTitle::handle_message(EventMessage message, void* param)
{

}