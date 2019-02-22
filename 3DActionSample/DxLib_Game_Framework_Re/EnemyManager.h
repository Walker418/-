#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include "Actor.h"

// �N���X�F�G�����A�Ǘ�
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class EnemyManager : public Actor
{
public:
	// �R���X�g���N�^
	explicit EnemyManager(IWorld* world);
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// ���b�Z�[�W����
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// �G�����^�C�}�[
	float	generate_timer_{ 0.0f };
};

#endif // !ENEMY_MANAGER_H_