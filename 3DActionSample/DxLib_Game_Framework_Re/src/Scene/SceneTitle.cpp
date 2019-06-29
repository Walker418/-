#include "SceneTitle.h"
#include "Scene.h"
#include "../ID/SourceID.h"

#include "../Field/Field.h"
#include "../Actor/ActorGroup.h"
#include "../Actor/Camera/TitleCamera.h"
#include "../Graphic/Light.h"
#include "../Actor/Player/PlayerInput.h"
#include "../Graphic/Graphics2D.h"
#include "../Math/MathHelper.h"
#include "../Sound/Sound.h"

// �J�n
void SceneTitle::start()
{
	is_end_ = false;			// �I���t���O��False�ɂ���
	world_.initialize();		// ���[���h��������

	is_started_ = false;		// �X�^�[�g�{�^���̏󋵂�������
	show_start_ = false;
	show_start_timer_ = 0.0f;
	end_timer_ = 0.0f;
	fade_counter_ = 0;

	// �t�B�[���h��ǉ�
	world_.add_field(new_field<Field>(MESH_STAGE_CASTLE, MESH_SKYBOX));
	// �J������ǉ�
	world_.add_camera(new_actor<TitleCamera>(&world_, Vector3::Zero));
	// ���C�g��ǉ�
	world_.add_light(new_actor<Light>(&world_, Vector3{ 0.0f, 30.0f, -20.0f }));

	// BGM�Đ��J�n
	Sound::play_bgm(BGM_TITLE);
}

// �X�V
void SceneTitle::update(float delta_time)
{
	world_.update(delta_time);

	// �t�F�C�h�C��/�t�F�C�h�A�E�g�p�J�E���^�[�̒l�𐧌�
	fade_counter_ = (int)MathHelper::clamp((float)fade_counter_, 0.0f, 255.0f);

	// �t�F�[�h�C�������܂ŁA������󂯕t���Ȃ�
	if (fade_counter_ < 255 && !is_started_)
	{
		fade_counter_ += 2;
		return;
	}

	// �Q�[���J�n
	if (PlayerInput::game_start())
	{
		is_started_ = true;
	}

	// �X�^�[�g�{�^���������ꂽ��A��ʂ��Ó]���A�v���C�V�[���Ɉڍs����
	if (is_started_)
	{
		// �t�F�[�h�A�E�g
		if (fade_counter_ > 0) fade_counter_ -= 2;

		if (end_timer_ >= 180.0f && fade_counter_ <= 0)
		{
			is_end_ = true;
		}

		end_timer_ += delta_time;
	}

	// �X�^�[�g���b�Z�[�W�̓_�Ő���
	// �X�y�[�X�L�[�������ꂽ��A�������_�ł���
	if (is_started_)
	{
		if (show_start_timer_ >= 5.0f)
		{
			(show_start_) ? show_start_ = false : show_start_ = true;

			show_start_timer_ = 0.0f;
		}
	}
	// ���i��1�b���Ɠ_��
	else
	{
		if (show_start_timer_ >= 60.0f)
		{
			(show_start_) ? show_start_ = false : show_start_ = true;

			show_start_timer_ = 0.0f;
		}
	}

	show_start_timer_ += delta_time;
}

// �`��
void SceneTitle::draw() const
{
	// �`��P�x���Z�b�g���A�t�F�[�h�C��/�t�F�[�h�A�E�g���o������
	SetDrawBright(fade_counter_, fade_counter_, fade_counter_);

	world_.draw();	// �w�i��`��

	draw_logo();	// �^�C�g�����S��`��
	draw_start();	// �X�^�[�g���b�Z�[�W��`��i�_�ŕt���j
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
	// BGM�Đ��I��
	Sound::stop_bgm();
}

// ���b�Z�[�W����
void SceneTitle::handle_message(EventMessage message, void* param)
{

}

// �^�C�g�����S�̕`��
void SceneTitle::draw_logo() const
{
	Graphics2D::draw(TEXTURE_TITLELOGO, Vector2(115.0f, 80.0f));
}

// �X�^�[�g���b�Z�[�W�̕`��
void SceneTitle::draw_start() const
{
	if (!show_start_) return;

	Graphics2D::draw(TEXTURE_START, Vector2(290.0f, 490.0f));
}