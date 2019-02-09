#ifndef DRAGON_BOAR_H_
#define DRAGON_BOAR_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "IBodyPtr.h"
#include "BoundingSphere.h"
#include "SourceID.h"

// �N���X�F�G�i�C�m�V�V�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class DragonBoar : public Actor
{
private:
	// �񋓌^�F�G�̏��
	enum class DragonBoarState
	{
		Idle,		// �Î~�ҋ@
		Move,		// �ړ�
		Attack,		// �U��
		Roar,		// ���K
		Damage,		// ����
		Death,		// ���S
	};

	// �񋓌^�F�G�̃��[�V����
	enum DragonBoarMotion
	{
		MOTION_IDLE = 4,	// �Î~�ҋ@
		MOTION_WALK = 6,	// ���s
		MOTION_DASH = 5,	// �_�b�V��
		MOTION_BITE = 3,	// ���݂�
		MOTION_ROAR = 2,	// ���K
		MOTION_DAMAGE = 1,	// ����
		MOTION_DEATH = 0,	// ���S
	};

public:
	// �R���X�g���N�^
	DragonBoar(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3(0.0f, 12.5f, 0.0f), 11.0f));
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// �Փ˃��A�N�V����
	virtual void react(Actor& other) override;
	// ���b�Z�[�W����
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// ��Ԃ̍X�V
	void update_state(float delta_time);
	// ��Ԃ̕ύX
	void change_state(DragonBoarState state, int motion);

	// �ҋ@��Ԃł̍X�V
	void idle(float delta_time);
	// �ړ���Ԃł̍X�V
	void move(float delta_time);
	// �U����Ԃł̍X�V
	void attack(float delta_time);
	// ���K��Ԃł̍X�V
	void roar(float delta_time);
	// ���ݏ�Ԃł̍X�V
	void damage(float delta_time);
	// ���S��Ԃł̍X�V
	void death(float delta_time);

	// �v���C���[�͍U���������ɂ��邩
	bool player_in_range_distance() const;
	// �v���C���[���U���ł���p�x�ɂ��邩
	bool player_in_range_angle() const;
	// �v���C���[���U���ł��邩
	bool can_attack_player() const;

	// ���̍s��������
	void next_move();
	// ���̖ړI�n���擾
	Vector3 next_destination() const;

	// �n�ʂƂ̐ڐG����
	void intersect_ground();
	// �ǂƂ̐ڐG����
	void intersect_wall();

private:
	// �A�j���[�V�������b�V��
	AnimatedMesh	mesh_{ MESH_DRAGONBOAR };
	// ���[�V�����ԍ�
	int				motion_{ DragonBoarMotion::MOTION_IDLE };
	// �G�̏��
	DragonBoarState	state_{ DragonBoarState::Idle };
	// ��ԃ^�C�}�[
	float			state_timer_{ 0.0f };

	// ���݂̗̑�
	int				current_hp_;

	// �ő�̗�
	const int		HP{ 100 };
	// �ړ����x
	const float		WalkSpeed{ 0.28f };
	// �d��
	const float		Gravity{ 0.03f };
};

#endif // !DRAGON_BOAR_H_