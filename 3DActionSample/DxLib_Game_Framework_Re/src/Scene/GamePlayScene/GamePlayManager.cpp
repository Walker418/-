#include "GamePlayManager.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../../Actor/ActorGroup.h"
#include "../../Actor/Enemy/Ghoul/Ghoul.h"
#include "../../Actor/Enemy/DragonBoar/DragonBoar.h"
#include "../../ID/EventMessage.h"
#include "../../Graphic/Graphics2D.h"
#include "../../ID/SourceID.h"
#include "../../Math/MathHelper.h"

// �N���X�F�Q�[���v���C�V�[���Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
GamePlayManager::GamePlayManager(IWorld* world) :
	Actor(world, "GamePlayManager"),
	phase_{ GamePlayPhase::Phase1 },
	enemy_defeated_{ 0 },
	boss_defeated_{ false },
	phase2_end_{ false },
	player_dead_{ false },
	game_end_{ false },
	fade_counter_{ 0 }
{
	// �Q�[���J�n�������s��
	game_start();
}

// �X�V
void GamePlayManager::update(float delta_time)
{
	update_phase(delta_time);	// �v���C�󋵂��X�V

	// �t�F�C�h�C��/�t�F�C�h�A�E�g�p�J�E���^�[�̒l�𐧌�
	fade_counter_ = (int)MathHelper::clamp((float)fade_counter_, 0.0f, 255.0f);
}

// �`��
void GamePlayManager::draw() const
{
	// �`��P�x���Z�b�g���A�t�F�C�h�C��/�t�F�C�h�A�E�g���o������
	SetDrawBright(fade_counter_, fade_counter_, fade_counter_);

	// �v���C���[�L�����̗̑͂�\��
	draw_HP_gauge(Vector2(20.0f, 15.0f));

	// ���݂̖ړI��\��
	draw_message(Vector2(750.0f, 430.0f));
}

// ���b�Z�[�W����
void GamePlayManager::handle_message(EventMessage message, void* param)
{
	// �󂯎�������b�Z�[�W�̎�ނɂ���āA�������s��

	switch (message)
	{
	case EventMessage::EnemyDead:	// �G���G���|���ꂽ�ꍇ
		++enemy_defeated_;			// �|���ꂽ�G�̐������Z
		break;
	case EventMessage::BossDead:	// �{�X�G���|���ꂽ�ꍇ
		boss_defeated_ = true;
		break;
	case EventMessage::PlayerDead:	// �v���C���[���|�ꂽ�ꍇ
		player_dead_ = true;
		break;
	default:
		break;
	}
}

// �v���C�󋵂̍X�V
void GamePlayManager::update_phase(float delta_time)
{
	switch (phase_)
	{
	case (GamePlayPhase::Phase1):
		phase1(delta_time);
		break;
	case (GamePlayPhase::Phase2):
		phase2(delta_time);
		break;
	default:
		break;
	}

	// �v���C���[�����S����ƁA���΂炭���ăQ�[���I�[�o�[���b�Z�[�W�𑗂�i�Q�[���I�[�o�[�������s���j
	if (player_dead_)
	{
		gameover_scene_timer_.update(delta_time);

		if (gameover_scene_timer_.is_time_out())
		{
			world_->send_message(EventMessage::GameOver);
		}
	}
}

// �Q�[���J�n����
void GamePlayManager::game_start()
{
	// �e��^�C�}�[�����Z�b�g
	phase_change_timer_.reset();
	gameover_scene_timer_.reset();
	gameclear_scene_timer_.reset();

	// �G���G3�̂𐶐�����
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 60.0f, 0.0f, -35.0f }, 160.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ -60.0f, 0.0f, -35.0f }, 200.0f));
}

// �t�F�[�Y�ڍs
void GamePlayManager::change_phase()
{
	// �{�X�𐶐����A�{�X��Ɉڍs����
	world_->add_actor(ActorGroup::Enemy, new_actor<DragonBoar>(world_, Vector3{ 0.0f, 0.0f, -50.0f }, 180.0f));
	phase_ = GamePlayPhase::Phase2;
}

// ��1�i�K�̏���
void GamePlayManager::phase1(float delta_time)
{
	// �Q�[���J�n���̃t�F�[�h�C��
	if (fade_counter_ < 255) fade_counter_ += 4;

	// �G���G���S���|�ꂽ��3�b��A�{�X��Ɉڍs
	if (phase1_end())
	{
		if (phase_change_timer_.is_time_out())
		{
			change_phase();
			return;
		}

		phase_change_timer_.update(delta_time);
	}
}

// ��2�i�K�̏���
void GamePlayManager::phase2(float delta_time)
{
	if (boss_defeated_)
	{
		if (gameclear_scene_timer_.is_time_out())
		{
			// �Q�[���N���A���b�Z�[�W�𑗂�i�Q�[���N���A�������s���j
			world_->send_message(EventMessage::StageClear);
			return;
		}

		gameclear_scene_timer_.update(delta_time);
	}
}

// �v���C���[�̗̑̓Q�[�W��\��
void GamePlayManager::draw_HP_gauge(Vector2 position) const
{
	Graphics2D::draw(TEXTURE_HPGAUGE, position);	// �̗̓Q�[�W��`��

	// �v���C���[�̗̑͂��擾
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	int player_hp = (player != nullptr) ? player->get_HP() : 0;

	// �Q�[�W�̒��g��`��
	if (player_hp > 0)
	{
		Vector2 draw_pos = position + Vector2(70.0f, 24.0f);	// �ŏ��̕`��ʒu

		for (int i = 0; i < player_hp; ++i)
		{

			Graphics2D::draw(TEXTURE_HP, Vector2(draw_pos.x + 4 * i, draw_pos.y));
		}
	}
}

// ���݂̖ړI��\��
void GamePlayManager::draw_message(Vector2 position) const
{
	// �i�s�󋵂ɉ����āA���݂̖ړI��\��
	switch (phase_)
	{
	case (GamePlayPhase::Phase1):
		Graphics2D::draw(TEXTURE_P1MESSAGE, position);
		break;
	case (GamePlayPhase::Phase2):
		Graphics2D::draw(TEXTURE_P2MESSAGE, position);
		break;
	default:
		break;
	}
}

// ��1�i�K�͏I�����Ă��邩
bool GamePlayManager::phase1_end() const
{
	// �G���G���S���|���ꂽ��ATrue��Ԃ�
	return enemy_defeated_ >= EnemyPopNo;
}
