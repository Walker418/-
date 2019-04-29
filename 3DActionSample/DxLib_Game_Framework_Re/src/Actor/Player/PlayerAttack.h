#ifndef PLAYER_ATTACK_H_
#define PLAYER_ATTACK_H_

#include "../Actor.h"
#include "../Body/IBodyPtr.h"
#include "../Body/BoundingSphere.h"
#include "../AttackParameter.h"

// �N���X�F�v���C���[�̍U���̓����蔻��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class PlayerAttack : public Actor
{
public:
	// �R���X�g���N�^
	PlayerAttack(IWorld* world, const Vector3& position, int power = 0, int impact = 0, float hit_stop = 0.0f, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3::Zero, 9.0f));
	// �R���X�g���N�^�i�U���p�����[�^�\���̂��g�p�j
	PlayerAttack(IWorld* world, AttackParameter parameter, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3::Zero, 9.0f));
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// �Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	float destroy_counter_{ 0.0f };	// ���ŃJ�E���^�[
	int power_{ 0 };				// �З�
	int impact_{ 0 };				// ���ݒl
	float hit_stop_{ 0.0f };		// �q�b�g�X�g�b�v
};

#endif // !PLAYER_ATTACK_H_