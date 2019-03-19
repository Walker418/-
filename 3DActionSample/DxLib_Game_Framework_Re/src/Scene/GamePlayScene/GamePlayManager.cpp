#include "GamePlayManager.h"
#include "../../World/IWorld.h"
#include "../../Actor/ActorGroup.h"
#include "../../Actor/Enemy/Ghoul/Ghoul.h"
#include "../../Actor/Enemy/DragonBoar/DragonBoar.h"
#include "../../ID/EventMessage.h"

// �N���X�F�Q�[���v���C�V�[���Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
GamePlayManager::GamePlayManager(IWorld* world) :
	Actor(world, "GamePlayManager"),
	state_{ GamePlayState::Phase1 },
	state_timer_{ 0.0f },
	state_change_time_{ 0.0f },
	enemy_defeated_{ 0 },
	phase1_end_{ false },
	boss_defeated_{ false },
	phase2_end_{ false },
	player_dead_{ false },
	game_end_{ false }
{
	// �Q�[���J�n�������s��
	game_start();
}

// �X�V
void GamePlayManager::update(float delta_time)
{
	update_phase(delta_time);	// �v���C�󋵂��X�V
}

// �`��
void GamePlayManager::draw() const
{
	/*
	// �f�o�b�O���b�Z�[�W
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DrawFormatString(0, 15, Cr, "�|�����G�̐��F %i", enemy_defeated_);
	*/

	unsigned int Cr;
	Cr = GetColor(230, 230, 230);

	// �v���C���[�L�����̗̑͂�\��
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	int player_hp = (player != nullptr) ? player->get_HP() : 0;
	DrawFormatString(0, 0, Cr, "�v���C���[�̗̑́F %i", player_hp);

	// �i�s�󋵂ɉ����āA���݂̖ړI��\��
	switch (state_)
	{
	case (GamePlayState::Phase1):
		DrawFormatString(750, 430, Cr, "�O�[��3�̂�|���I�I�F %i / 3", enemy_defeated_);
		break;
	case (GamePlayState::Phase2):
		DrawString(750, 430, "���僂���X�^�[��|���I�I", Cr);
		break;
	default:
		break;
	}
}

// ���b�Z�[�W����
void GamePlayManager::handle_message(EventMessage message, void* param)
{
	/*
	if (message == EventMessage::EnemyDead)
	{
		// �|���ꂽ�G�̐������Z
		++enemy_defeated_;
	}
	*/

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
	switch (state_)
	{
	case (GamePlayState::Phase1):
		phase1(delta_time);
		break;
	case (GamePlayState::Phase2):
		phase2(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;

	// �v���C���[�����S����ƁA�Q�[���I���t���O�𗧂��āA���΂炭���ăQ�[���I�[�o�[���b�Z�[�W�𑗂�
	if (player_dead_)
	{
		if (!game_end_)
		{
			state_change_time_ = state_timer_ + 60.0f;
			game_end_ = true;
		}

		if (state_timer_ >= state_change_time_)
		{
			world_->send_message(EventMessage::GameOver);
		}
	}
}

// �Q�[���J�n����
void GamePlayManager::game_start()
{
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
	state_ = GamePlayState::Phase2;
	state_timer_ = 0.0f;
}

// ��1�i�K�̏���
void GamePlayManager::phase1(float delta_time)
{
	// 3�b��A�{�X��Ɉڍs
	if (enemy_defeated_ >= 3)
	{
		if (!phase1_end_)
		{
			state_change_time_ = state_timer_ + 180.0f;
			phase1_end_ = true;
		}

		if (state_timer_ >= state_change_time_)
		{
			change_phase();
		}
	}
}

// ��2�i�K�̏���
void GamePlayManager::phase2(float delta_time)
{
	if (boss_defeated_)
	{
		if (!phase2_end_)
		{
			state_change_time_ = state_timer_ + 180.0f;
			phase2_end_ = true;
		}

		if (state_timer_ >= state_change_time_)
		{
			// �Q�[���N���A���b�Z�[�W�𑗂�
			world_->send_message(EventMessage::StageClear);
		}
	}
}