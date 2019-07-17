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

#include "../Actor/Player/PlayerInput.h"
#include "../Game/WindowSetting.h"
#include "../Graphic/Graphics2D.h"
#include "../ID/SourceID.h"

// �N���X�F�Q�[���v���C�V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// ============================================================
// �ȉ��͉摜�X�v���C�g�̊֘A�萔
// ============================================================

const int PauseTextWidth = 400;		// PAUSE�����摜�̕�
const int PauseTextHeight = 160;	// PAUSE�����摜�̍���

// ============================================================

// �J�n
void SceneGamePlay::start()
{
	// �I���t���O��false�ɂ���
	is_end_ = false;
	// �|�[�Y��Ԃ�false�ɂ���
	is_pause_ = false;
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
	// �|�[�Y���Ă��Ȃ���΁A�Q�[���i�s���X�V
	if (!is_pause_)
		world_.update(delta_time);

	// �|�[�Y
	if (PlayerInput::pause())
		is_pause_ = (is_pause_) ? false : true;
}

// �`��
void SceneGamePlay::draw() const
{
	world_.draw();

	// �|�[�Y����PAUSE�摜��`��
	if (is_pause_)
	{
		Graphics2D::draw(TEXTURE_PAUSE_BG, Vector2::Zero);

		int win_width = WindowSetting::WindowWidth;
		int win_height = WindowSetting::WindowHeight;
		int text_posX = win_width / 2 - PauseTextWidth / 2;
		int text_posY = win_height / 2 - PauseTextHeight / 2;
		Vector2 pos{ (float)text_posX, (float)text_posY };
		Graphics2D::draw(TEXTURE_PAUSE_TEXT, pos);
	}
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