#ifndef PLAYER_ATTACK_H_
#define PLAYER_ATTACK_H_

#include "Actor.h"
#include "IBodyPtr.h"
#include "BoundingSphere.h"

// �N���X�F�v���C���[�̍U������
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class PlayerAttack : public Actor
{
public:
	// �R���X�g���N�^
	PlayerAttack(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3::Zero, 1.5f));
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// �Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	float destroy_counter_{ 0.0f };	// ���ŃJ�E���^�[
};

#endif // !PLAYER_ATTACK_H_