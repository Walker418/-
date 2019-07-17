#ifndef ACTION_SAMPLE_H_
#define ACTION_SAMPLE_H_

#include "Game.h"
#include "../Scene/SceneManager.h"

// �N���X�F3D�A�N�V�����Q�[���T���v���i�A�v���P�[�V�����N���X�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class ActionSample : public Game
{
public:
	// �R���X�g���N�^
	ActionSample();

private:
	// �J�n
	void start() override;
	// �X�V
	void update(float delta_time) override;
	// �`��
	void draw() override;
	// �I��
	void end() override;

private:
	// �V�[���}�l�[�W���[
	SceneManager	scene_manager_;
	// �|�[�Y���Ȃ̂�
	bool			pause_{ false };
};

#endif // !ACTION_SAMPLE_H_