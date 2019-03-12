#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include "IScene.h"

// �N���X�F�󔒂̃V�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class SceneNull : public IScene
{
public:
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
};

#endif // !SCENE_NULL_H_