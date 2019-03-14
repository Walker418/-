#ifndef GHOUL_H_
#define GHOUL_H_

#include "../Enemy.h"
#include "../../../Graphic/AnimatedMesh.h"
#include "../../Body/IBodyPtr.h"
#include "../../Body/BoundingCapsule.h"
#include "../../../ID/SourceID.h"
#include "../../../Math/Random.h"

// �N���X�F�O�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class Ghoul : public Enemy
{
private:
	// �񋓌^�F�G�̏��
	enum class GhoulState
	{
		Idle,	// �Î~�ҋ@
		Move,	// �ړ�
		Attack,	// �U��
		Wince,	// ����
		Death,	// ���S
	};

	// �񋓌^�F�G�̃��[�V����
	enum GhoulMotion
	{
		MOTION_IDLE = 0,
		MOTION_WALK = 1,
		MOTION_TURN_LEFT = 2,
		MOTION_TURN_RIGHT = 3,
		MOTION_WINCE = 4,
		MOTION_ATTACK = 5,
		MOTION_DEATH = 6
	};

public:
	// �R���X�g���N�^
	Ghoul(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3(0.0f, 9.2f, 0.0f), Matrix::Identity, 13.5f, 2.5f));
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
	void change_state(GhoulState state, int motion);

	// �ҋ@��Ԃł̍X�V
	void idle(float delta_time);
	// �ړ���Ԃł̍X�V
	void move(float delta_time);
	// ���ݏ�Ԃł̍X�V
	void wince(float delta_time);
	// �U����Ԃł̍X�V
	void attack(float delta_time);
	// ���S��Ԃł̍X�V
	void death(float delta_time);

	// ���̍s��������
	void next_move();
	// ���̖ړI�n������
	void next_destination();

	// �v���C���[���U���ł��邩
	virtual bool can_attack_player() override;

	// ���̏�Ԃւ̈ڍs����
	void ready_to_next_state(int min, int max);

private:
	// �A�j���[�V�������b�V��
	AnimatedMesh	mesh_{ MESH_GHOUL };
	// ���[�V�����ԍ�
	int				motion_{ GhoulMotion::MOTION_IDLE };
	// �G�̏��
	GhoulState		state_{ GhoulState::Idle };
	// �O�̏��
	GhoulState		previous_state_;
	// ��ԃ^�C�}�[
	float			state_timer_{ 0.0f };
	// �ړ���ԃ^�C�}�[�i�ړ���Ԓ��A�v���C���[���U���͈͂ɂ��Ȃ���Ή��Z�j
	float			move_timer_{ 0.0f };
	// ����������
	Random			rand_;

	// �U������𔭐������̂�
	bool			attack_on_{ false };
	// ���̖ړI�n
	Vector3			next_destination_;
	// �v���C���[�ɒǏ]���Ȃ̂�
	bool			is_following_player_{ false };
	// �ړ����Ȃ̂�
	bool			is_moving_{ false };
	// ���̏�Ԉێ�����
	float			state_time_{ 60.0f };
	// �U���̊Ԋu����
	float			attack_interval_{ 0.0f };

	// �ő�̗�
	const int		HP{ 40 };
	// �ړ����x
	const float		WalkSpeed{ 0.28f };
	// ��]���x
	const float		RotateSpeed{ 1.0f };
	// �d��
	const float		Gravity{ 0.03f };

	// ���ݑϐ�
	const int		ToWince{ 10 };
	// ���G����
	const float		Range{ 12.0f };
	// ���G�p�x
	const float		Angle{ 10.0f };
};

#endif // !GHOUL_H_