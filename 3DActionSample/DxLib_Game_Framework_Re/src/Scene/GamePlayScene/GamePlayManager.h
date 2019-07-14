#ifndef GAME_PLAY_MANAGER_H_
#define GAME_PLAY_MANAGER_H_

#include "../../Actor/Actor.h"
#include "../../Math/Vector2.h"
#include "../../Math/CountdownTimer.h"

// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �v���C��
enum GamePlayPhase
{
	Phase1,		// ��1�i�K�i�G����j
	Phase2,		// ��2�i�K�i�{�X��j
};

// �N���X�F�Q�[���v���C�V�[���Ǘ�
class GamePlayManager : public Actor
{
private:
	const float PhaseChangeTime{ 180.0f };				// ��1�i�K�I����A��2�i�K�Ɉڍs����܂ł̃t���[����
	const float SceneChangeTime_GameOver{ 120.0f };		// �Q�[���I�[�o�[��A���̃V�[���Ɉڍs����܂ł̃t���[����
	const float SceneChangeTime_GameClear{ 180.0f };	// �Q�[���N���A��A���̃V�[���Ɉڍs����܂ł̃t���[����

	const int	EnemyPopNo{ 3 };						// ��������G���G�̐�

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
	void draw_HP_gauge() const;
	// ���݂̖ړI��\��
	void draw_message() const;

	// ��1�i�K�͏I�����Ă��邩
	bool phase1_end() const;
	// ��1�i�K�̃X�L�b�v
	void skip_phase1();

private:
	// �v���C��
	GamePlayPhase	phase_{ GamePlayPhase::Phase1 };
	// �t�F�[�Y�ڍs�^�C�}�[
	CountdownTimer	phase_change_timer_{ PhaseChangeTime };
	// �Q�[���I�[�o�[��V�[���ڍs�^�C�}�[
	CountdownTimer	gameover_scene_timer_{ SceneChangeTime_GameOver };
	// �Q�[���N���A��V�[���ڍs�^�C�}�[
	CountdownTimer	gameclear_scene_timer_{ SceneChangeTime_GameClear };
	// �|���ꂽ�G�̐��i�󋵈ڍs����p�j
	int				enemy_defeated_{ 0 };
	// �{�X���|���ꂽ��
	bool			boss_defeated_{ false };
	// ��2�i�K�͏I��������
	bool			phase2_end_{ false };
	// �v���C���[���|�ꂽ��
	bool			player_dead_{ false };
	// �Q�[���I���t���O
	bool			game_end_{ false };

	int fade_counter_{ 0 };		// �t�F�[�h�C��/�t�F�[�h�A�E�g�p�J�E���^�[
};

#endif // !GAME_PLAY_MANAGER_H_