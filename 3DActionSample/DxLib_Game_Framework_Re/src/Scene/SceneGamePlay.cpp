#include "SceneGamePlay.h"
#include "../ID/SourceID.h"

#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../ID/EventMessage.h"
#include "../Actor/Camera/TPCamera.h"
#include "../Graphic/Light.h"
#include "../Actor/Player/Player.h"
#include "GamePlayScene/GamePlayManager.h"

#include "../Graphic/SkeletalMesh.h"
#include "../Math/Collision/CollisionMesh.h"
#include "../Field/Skybox.h"
#include "../Graphic/Billboard.h"

// �N���X�F�Q�[���v���C�V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �J�n
void SceneGamePlay::start()
{
	// �I���t���O��False�ɂ���
	is_end_ = false;
	// ���[���h��������
	world_.initialize();

	// �C�x���g���b�Z�[�W���X�i�[��o�^
	world_.add_event_message_listener([=](EventMessage message, void* param) {handle_message(message, param); });

	// �t�B�[���h��ǉ�
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// �J������ǉ�
	world_.add_camera(new_actor<TPCamera>(&world_));
	// ���C�g��ǉ�
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));

	// �A�N�^�[�͂����ɒǉ�
	world_.add_actor(ActorGroup::Player, new_actor<Player>(&world_, Vector3{ 0.0f, 0.0f, 100.0f }));	// �v���C���[
	world_.add_actor(ActorGroup::UI, new_actor<GamePlayManager>(&world_));								// �Q�[���v���C�Ǘ���
}

// �X�V
void SceneGamePlay::update(float delta_time)
{
	world_.update(delta_time);
}

// �`��
void SceneGamePlay::draw() const
{
	world_.draw();
}

// �I�����Ă��邩
bool SceneGamePlay::is_end() const
{
	// �I���t���O��Ԃ�
	return is_end_;
}

// ���̃V�[���̎擾
Scene SceneGamePlay::next() const
{
	return next_scene_;
}

// �I��
void SceneGamePlay::end()
{
	world_.clear();		// ���[���h���N���A
}

// ���b�Z�[�W����
void SceneGamePlay::handle_message(EventMessage message, void* param)
{
	// �󂯎�������b�Z�[�W�̎�ނɂ���āA�������s��

	// �Q�[���I�[�o�[�̏ꍇ
	if (message == EventMessage::GameOver)
	{
		is_end_ = true;
		return;
	}

	// �Q�[���N���A�̏ꍇ
	if (message == EventMessage::StageClear)
	{
		is_end_ = true;
		return;
	}
}