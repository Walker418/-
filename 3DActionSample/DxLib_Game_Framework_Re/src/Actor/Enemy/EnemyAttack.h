#ifndef ENEMY_ATTACK_H_
#define ENEMY_ATTACK_H_

#include "../Actor.h"


// �N���X�F�G�̍U���̓����蔻��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class EnemyAttack : public Actor
{
public:
	// �R���X�g���N�^
	EnemyAttack(IWorld* world, const Vector3& position, int power = 0, float radius = 6.0f);
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// �Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	float destroy_counter_{ 0.0f };	// ���ŃJ�E���^�[
	int power_{ 0 };				// �З�
};

#endif // !ENEMY_ATTACK_H_