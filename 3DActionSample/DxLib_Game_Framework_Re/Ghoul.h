#ifndef GHOUL_H_
#define GHOUL_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "IBodyPtr.h"
#include "BoundingCapsule.h"
#include "SourceID.h"

// �N���X�F�O�[��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class Ghoul : public Actor
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

	// �n�ʂƂ̐ڐG����
	void intersect_ground();
	// �ǂƂ̐ڐG����
	void intersect_wall();

private:
	// �A�j���[�V�������b�V��
	AnimatedMesh	mesh_{ MESH_GHOUL };
	// ���[�V�����ԍ�
	int				motion_{ GhoulMotion::MOTION_IDLE };
	// �G�̏��
	GhoulState		state_{ GhoulState::Idle };
	// ��ԃ^�C�}�[
	float			state_timer_{ 0.0f };

	// ���݂̗̑�
	int				current_hp_;
	// ���ݗݐϒl
	int				current_wince_{ 0 };
	// �U������𔭐������̂�
	bool			is_attack_{ false };

	// �ő�̗�
	const int		HP{ 20 };
	// �ړ����x
	const float		WalkSpeed{ 0.15f };
	// ��]���x
	const float		RotateSpeed{ 0.015f };
	// �d��
	const float		Gravity{ 0.03f };

	// ���ݑϐ�
	const int		ToWince{ 5 };
};

#endif // !GHOUL_H_