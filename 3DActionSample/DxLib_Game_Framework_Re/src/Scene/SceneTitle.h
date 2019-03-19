#ifndef SCENE_TITLE_H_
#define SCENE_TITLE_H_

#include "IScene.h"
#include "../World/World.h"

// �N���X�F�^�C�g���V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class SceneTitle : public IScene
{
public:
	// �f�t�H���g�R���X�g���N�^
	SceneTitle() = default;
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
	// ���[���h�N���X
	World	world_;
	// �I���t���O
	bool	is_end_{ false };
};

#endif // !SCENE_TITLE_H_