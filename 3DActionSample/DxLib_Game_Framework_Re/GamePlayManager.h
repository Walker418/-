#ifndef GAME_PLAY_MANAGER_H_
#define GAME_PLAY_MANAGER_H_

#include "Actor.h"

// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �v���C��
enum GamePlayState
{
	Phase1,		// ��1�i�K�i�G����j
	Phase2,		// ��2�i�K�i�{�X��j
};

// �N���X�F�Q�[���v���C�V�[���Ǘ�
class GamePlayManager : public Actor
{
public:
	// �R���X�g���N�^
	explicit GamePlayManager(IWorld* world);
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// ���b�Z�[�W����
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// �Q�[���J�n����
	void game_start();

private:
	// �v���C��
	GamePlayState	state_{ GamePlayState::Phase1 };
	// ��Ԉڍs�^�C�}�[
	float			state_change_timer_{ 0.0f };
	// �|���ꂽ�G�̐��i�󋵈ڍs����p�j
	int				enemy_defeated_{ 0 };

	// ��������G���G�̐�
	const unsigned int EnemyQuantity{ 3 };
};

#endif // !GAME_PLAY_MANAGER_H_