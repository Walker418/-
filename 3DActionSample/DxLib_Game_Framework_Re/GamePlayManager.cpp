#include "GamePlayManager.h"
#include "IWorld.h"
#include "ActorGroup.h"
#include "Ghoul.h"

#include "EventMessage.h"

// �N���X�F�Q�[���v���C�V�[���Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
GamePlayManager::GamePlayManager(IWorld* world) :
	Actor(world, "GamePlayManager"),
	state_{ GamePlayState::Phase1 },
	state_change_timer_{ 0.0f },
	enemy_defeated_{ 0 }
{
	// �Q�[���J�n�������s��
	game_start();
}

// �X�V
void GamePlayManager::update(float delta_time)
{

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
}

// ���b�Z�[�W����
void GamePlayManager::handle_message(EventMessage message, void* param)
{
	if (message == EventMessage::EnemyDead)
	{
		// �|���ꂽ�G�̐������Z
		++enemy_defeated_;
	}
}

// �Q�[���J�n����
void GamePlayManager::game_start()
{
	// �G���G�𐶐�����
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 0.0f, 0.0f, -100.0f }, 180.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ 50.0f, 0.0f, -80.0f }, 160.0f));
	world_->add_actor(ActorGroup::Enemy, new_actor<Ghoul>(world_, Vector3{ -50.0f, 0.0f, -80.0f }, 200.0f));
}