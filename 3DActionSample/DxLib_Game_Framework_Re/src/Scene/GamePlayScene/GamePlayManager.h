#ifndef GAME_PLAY_MANAGER_H_
#define GAME_PLAY_MANAGER_H_

#include "../../Actor/Actor.h"
#include "../../Math/Vector2.h"

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
	// �v���C�󋵂̍X�V
	void update_phase(float delta_time);
	// �Q�[���J�n����
	void game_start();
	// �t�F�[�Y�ڍs
	void change_phase();
	// ��1�i�K�̏���
	void phase1(float delta_time);
	// ��2�i�K�̏���
	void phase2(float delta_time);
	// �v���C���[�̗̑̓Q�[�W��\��
	void draw_HP_gauge(Vector2 position) const;
	// ���݂̖ړI��\��
	void draw_message(Vector2 position) const;

private:
	// �v���C��
	GamePlayState	state_{ GamePlayState::Phase1 };
	// �t�F�[�Y�^�C�}�[
	float			state_timer_{ 0.0f };
	// �t�F�[�Y�ڍs�^�C�}�[
	float			state_change_time_{ 0.0f };
	// �|���ꂽ�G�̐��i�󋵈ڍs����p�j
	int				enemy_defeated_{ 0 };
	// ��1�i�K�͏I��������
	bool			phase1_end_{ false };
	// �{�X���|���ꂽ��
	bool			boss_defeated_{ false };
	// ��2�i�K�͏I��������
	bool			phase2_end_{ false };
	// �v���C���[���|�ꂽ��
	bool			player_dead_{ false };
	// �Q�[���I���t���O
	bool			game_end_{ false };
};

#endif // !GAME_PLAY_MANAGER_H_