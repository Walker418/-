#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "BoundingCapsule.h"
#include "SourceID.h"

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
		Damage,			// ��e
		Guard,			// �K�[�h
		Blocking,		// �K�[�h�ɂ��m�b�N�o�b�N
		GuardAttack,	// �K�[�h�U��
		GuardEnd,		// �K�[�h�I��
		Death,			// ���S
	};

	// �񋓌^�F�v���C���[�̃��[�V����
	enum PlayerMotion
	{
		MOTION_IDLE = 16,				// �Î~�ҋ@
		MOTION_WALK = 23,				// �O���֕��s
		MOTION_DASH = 9,				// �O���փ_�b�V��
		MOTION_JUMP = 18,				// �W�����v
		MOTION_SLASH_1 = 20,			// �U���i1�i�ځj
		MOTION_SLASH_2 = 21,			// �U���i2�i�ځj
		MOTION_SLASH_3 = 22,			// �U���i3�i�ځj
		MOTION_JUMP_ATTACK = 19,		// �W�����v�U��
		MOTION_IMPACT = 17,				// ��e
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
	};

public:
	// �R���X�g���N�^
	Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3(0.0f, 8.5f, 0.0f), Matrix::Identity, 12.0f, 2.5f));
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
	void change_state(PlayerState state, int motion);

	// �ʏ��Ԃł̍X�V
	void normal(float delta_time);
	// �U���i1�i�ځj�ł̍X�V
	void slash1(float delta_time);
	// �U���i2�i�ځj�ł̍X�V
	void slash2(float delta_time);
	// �U���i3�i�ځj�ł̍X�V
	void slash3(float delta_time);
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

private:
	// �A�j���[�V�������b�V��
	AnimatedMesh	mesh_{ MESH_PALADIN };
	// ���[�V�����ԍ�
	int				motion_{ PlayerMotion::MOTION_IDLE };
	// �v���[���[�̏��
	PlayerState		state_{ PlayerState::Normal };
	// ��ԃ^�C�}�[
	float			state_timer_{ 0.0f };

	// ���݂̗̑�
	int				current_hp_;

	// �ő�̗�
	const int		HP{ 100 };
	// �ړ����x
	const float		WalkSpeed{ 0.28f };
	// �d��
	const float		Gravity{ 9.8f };
};

#endif // !PLAYER_H_