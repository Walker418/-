#ifndef PLAYER_H_
#define PLAYER_H_

#include "../Actor.h"
#include "../../Graphic/AnimatedMesh.h"
#include "../Body/IBodyPtr.h"
#include "../Body/BoundingCapsule.h"
#include "../../ID/SourceID.h"

// �N���X�F�v���C���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Player : public Actor
{
private:
	// �񋓌^�F�v���C���[�̏��
	enum class PlayerState
	{
		Normal,			// �ʏ�
		Slash1,			// �U���i1�i�ځj
		Slash2,			// �U���i2�i�ځj
		Slash3,			// �U���i3�i�ځj
		JumpAttack1,	// �W�����v�U���i1�i�ځj
		JumpAttack2,	// �W�����v�U���i2�i�ځj
		Damage,			// ����
		Guard,			// �K�[�h
		Blocking,		// �K�[�h�ɂ��m�b�N�o�b�N
		GuardAttack,	// �K�[�h�U��
		GuardEnd,		// �K�[�h�I��
		Death,			// ���S
		ForwardEvasion,	// �O���
		LeftEvasion,	// �����
		RightEvasion,	// �E���
	};

	// �񋓌^�F�v���C���[�̃��[�V����
	enum PlayerMotion
	{
		MOTION_IDLE = 16,				// �Î~�ҋ@
		MOTION_WALK = 25,				// �O���֕��s
		MOTION_DASH = 9,				// �O���փ_�b�V��
		MOTION_JUMP = 18,				// �W�����v
		MOTION_SLASH_1 = 20,			// �U���i1�i�ځj
		MOTION_SLASH_2 = 21,			// �U���i2�i�ځj
		MOTION_SLASH_3 = 22,			// �U���i3�i�ځj
		MOTION_JUMP_ATTACK = 19,		// �W�����v�U��
		MOTION_IMPACT = 17,				// ����
		MOTION_GUARD_BEGIN = 11,		// �K�[�h�J�n
		MOTION_GUARD_IDLE = 14,			// �K�[�h��
		MOTION_GUARD_BLOCK = 12,		// �K�[�h�ɂ��m�b�N�o�b�N
		MOTION_GUARD_SLASH = 15,		// �K�[�h�U��
		MOTION_GUARD_END = 13,			// �K�[�h�I��
		MOTION_CROUCH_BEGIN = 0,		// ���Ⴊ�݊J�n
		MOTION_CROUCH_IDLE = 6,			// ���Ⴊ�ݒ��ҋ@
		MOITON_CROUCH_IMPACT = 7,		// ���Ⴊ�ݒ���e
		MOTION_CROUCH_ATTACK = 8,		// ���Ⴊ�ݒ��U��
		MOTION_CROUCH_GUARD_BEGIN = 2,	// ���Ⴊ�݃K�[�h�J�n
		MOTION_CROUCH_GUARD_IDLE = 5,	// ���Ⴊ�݃K�[�h��
		MOTION_CROUCH_GUARD_BLOCK = 3,	// ���Ⴊ�݃K�[�h�ɂ��m�b�N�o�b�N
		MOTION_CROUCH_GUARD_ATTACK = 8,	// ���Ⴊ�݃K�[�h�U���i���Ⴊ�ݒ��U���Ɠ����j
		MOTION_CROUCH_GUARD_END = 4,	// ���Ⴊ�݃K�[�h�I��
		MOTION_CROUCH_END = 1,			// ���Ⴊ�ݏI��
		MOTION_DEATH = 10,				// ���S
		MOTION_STRAFE_LEFT = 23,		// ���_�b�V��
		MOTION_STRAFE_RIGHT = 24,		// �E�_�b�V��
	};

public:
	// �R���X�g���N�^
	Player(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3(0.0f, 8.5f, 0.0f), Matrix::Identity, 11.0f, 2.5f));
	// �X�V
	virtual void update(float delta_time) override;
	// �`��
	virtual void draw() const override;
	// �Փ˃��A�N�V����
	virtual void react(Actor& other) override;
	// ���b�Z�[�W����
	virtual void handle_message(EventMessage message, void* param = nullptr) override;
	// �̗͂̎擾
	virtual int get_HP() override;

private:
	// ��Ԃ̍X�V
	void update_state(float delta_time);
	// ��Ԃ̕ύX
	void change_state(PlayerState state, int motion);

	// �ʏ��Ԃł̍X�V
	void normal(float delta_time);
	// �U���i1�i�ځj�ł̍X�V
	void slash1(float delta_time);
	// �U���i2�i�ځj�ł̍X�V
	void slash2(float delta_time);
	// �U���i3�i�ځj�ł̍X�V
	void slash3(float delta_time);
	// �W�����v�U���i1�i�ځj�ł̍X�V
	void jump_attack1(float delta_time);
	// �W�����v�U���i2�i�ځj�ł̍X�V
	void jump_attack2(float delta_time);
	// ��e��Ԃł̍X�V
	void damage(float delta_time);
	// �K�[�h���̍X�V
	void guard(float delta_time);
	// �K�[�h�ɂ��m�b�N�o�b�N���̍X�V
	void blocking(float delta_time);
	// �K�[�h�U���ł̍X�V
	void guard_attack(float delta_time);
	// �K�[�h�I�����̍X�V
	void guard_end(float delta_time);
	// ���S��Ԃł̍X�V
	void death(float delta_time);
	// �O�����Ԃł̍X�V
	void forward_evasion(float delta_time);
	// �������Ԃł̍X�V
	void left_evasion(float delta_time);
	// �E�����Ԃł̍X�V
	void right_evasion(float delta_time);
	// �U����̉���s���ڍs
	void attack_to_evasion();

	// �n�ʂƂ̐ڐG����
	void intersect_ground();
	// �ǂƂ̐ڐG����
	void intersect_wall();
	// ���W����
	void clamp_position();
	// �������
	void ready_to_skip();

	// �K�[�h�͐������邩
	bool can_block(Vector3 atk_pos);
	// ���G���ԓ��ł��邩
	bool is_invincible();
	// �X�[�p�[�A�[�}�[��Ԃł��邩
	bool is_super_armor();

private:
	// �A�j���[�V�������b�V��
	AnimatedMesh	mesh_{ MESH_PALADIN };
	// ���[�V�����ԍ�
	int				motion_{ PlayerMotion::MOTION_IDLE };
	// �v���[���[�̏��
	PlayerState		state_{ PlayerState::Normal };
	// ��ԃ^�C�}�[
	float			state_timer_{ 0.0f };
	// �ڒn���Ă��邩
	bool			is_ground_{ false };
	// �K�[�h���Ă��邩
	bool			is_guard_{ false };
	// �U������𔭐������̂�
	bool			attack_on_{ false };
	// �W�����v�U���͊J�n������
	bool			jump_attack_started_{ false };
	// ���G���ԃ^�C�}�[
	float			invincible_timer_{ 0.0f };
	// ����^�C�}�[
	float			skip_timer_{ 0.0f };

	// ���݂̗̑�
	int				current_hp_;

	// ============================================================
	// �ȉ��̓v���C���[�̃p�����[�^�[�萔

	const int	HP{ 100 };				// �ő�̗�
	const float	WalkSpeed{ 0.3f };		// ���s���x
	const float	DashSpeed{ 0.8f };		// �_�b�V�����x
	const float	RotateSpeed{ 0.3f };	// ��]���x
	const float	Gravity{ 0.03f };		// �������x

	const int	Power_Atk1{ 3 };		// �U��1�i�ڂ̈З�
	const int	Power_Atk2{ 2 };		// �U��2�i�ڂ̈З�
	const int	Power_Atk3{ 5 };		// �U��3�i�ڂ̈З�
	const int	Power_JumpAtk1{ 5 };	// �W�����v�U��1�i�ڂ̈З�
	const int	Power_JumpAtk2{ 2 };	// �W�����v�U��2�i�ڂ̈З�
	const int	Power_GuardAtk{ 3 };	// �K�[�h�U���̈З�

	const int	Wince_Atk1{ 1 };		// �U��1�i�ڂ̋��ݒl
	const int	Wince_Atk2{ 1 };		// �U��2�i�ڂ̋��ݒl
	const int	Wince_Atk3{ 3 };		// �U��3�i�ڂ̋��ݒl
	const int	Wince_JumpAtk1{ 3 };	// �W�����v�U��1�i�ڂ̋��ݒl
	const int	Wince_JumpAtk2{ 1 };	// �W�����v�U��1�i�ڂ̋��ݒl
	const int	Wince_GuardAtk{ 1 };	// �K�[�h�U���̋��ݒl

	// ============================================================
};

#endif // !PLAYER_H_