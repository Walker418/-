#ifndef SCENE_GAME_PLAY_H_
#define SCENE_GAME_PLAY_H_

#include "IScene.h"
#include "../World/World.h"
#include "Scene.h"

// �N���X�F�Q�[���v���C�V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class SceneGamePlay : public IScene
{
public:
	// �f�t�H���g�R���X�g���N�^
	SceneGamePlay() = default;
	// �J�n
	virtual void start() override;
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// �I�����Ă��邩
	virtual bool is_end() const override;
	// ���̃V�[���̎擾
	virtual Scene next() const override;
	// �I��
	virtual void end() override;
	// ���b�Z�[�W����
	void handle_message(EventMessage message, void* param);

private:
	// ���[���h
	World	world_;
	// �I���t���O
	bool	is_end_{ false };
	// ���̃V�[���i�f�t�H���g�̓Q�[���I�[�o�[�V�[���j
	// Scene	next_scene_{ Scene::GameOver };
	// Scene	next_scene_{ Scene::Title };
	Scene	next_scene_{ Scene::Loading };
};

#endif // !SCENE_GAME_PLAY_H_