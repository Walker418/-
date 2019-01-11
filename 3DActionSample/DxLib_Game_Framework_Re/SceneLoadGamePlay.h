#ifndef SCENE_LOAD_GAME_PLAY_H_
#define SCENE_LOAD_GAME_PLAY_H_

#include "IScene.h"

// �N���X�F�Q�[���v���C�V�[�����\�[�X�̓ǂݍ���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class SceneLoadGamePlay : public IScene
{
public:
	// �R���X�g���N�^
	SceneLoadGamePlay() = default;
	// �J�n
	virtual void start() override;
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// �I�����Ă��邩
	virtual bool is_end() const override;
	// ���̃V�[����Ԃ�
	virtual Scene next() const override;
	// �I��
	virtual void end() override;

private:
	bool	is_loaded_{ false };	// �ǂݍ��ޔ���
	float	end_timer_{ 0.0f };		// �I���^�C�}�[
	bool	is_end_{ false };		// �I���t���O
};

#endif // !SCENE_LOAD_GAME_PLAY_H_