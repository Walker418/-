#include "SceneGamePlay.h"
#include "Scene.h"
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

	// ������
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
	world_.add_actor(ActorGroup::Effect, new_actor<GamePlayManager>(&world_));							// �Q�[���v���C�Ǘ���
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

	// �f�o�b�O���b�Z�[�W
	/*
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);

	auto player = world_.find_actor(ActorGroup::Player, "Player");
	if (!player) return;
	auto player_pos = player->position();
	DrawFormatString(0, 0, Cr, "�v���C���[�̍��W�F %f�A%f�A%f", player_pos.x, player_pos.y, player_pos.z);
	auto player_forward = player->pose().Forward();
	float player_angle = Vector3::Angle(player_forward, Vector3::Forward);
	DrawFormatString(0, 15, Cr, "�v���C���[�̊p�x�F %f", player_angle);
	*/
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
	return Scene::LoadGamePlay;
}

// �I��
void SceneGamePlay::end()
{
	// ���[���h���N���A
	world_.clear();

	// �f�ނ�j��
	SkeletalMesh::erase(MESH_PALADIN);
	SkeletalMesh::erase(MESH_GHOUL);
	SkeletalMesh::erase(MESH_DRAGONBOAR);

	CollisionMesh::erase(MESH_STAGE_CASTLE);
	Skybox::erase(MESH_SKYBOX);
	Billboard::erase(0);
}

// ���b�Z�[�W����
void SceneGamePlay::handle_message(EventMessage message, void* param)
{
	// �󂯎�������b�Z�[�W�̎�ނɂ���āA�������s��

	// �v���C���[�����S�����ꍇ
	if (message == EventMessage::PlayerDead)
	{
		// �V�[���I���i�Q�[���I�[�o�[�V�[���ֈڍs�j
		is_end_ = true;
		return;
	}

	// �{�X�G�����S�����ꍇ
	if (message == EventMessage::BossDead)
	{
		// �V�[���I���i�Q�[���N���A�V�[���ֈڍs�j

		return;
	}
}