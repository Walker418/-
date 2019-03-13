#ifndef DRAGON_BOAR_H_
#define DRAGON_BOAR_H_

#include "../Enemy.h"
#include "../../../Graphic/AnimatedMesh.h"
#include "../../Body/IBodyPtr.h"
#include "../../Body/BoundingCapsule.h"
#include "../../../ID/SourceID.h"
#include "../../../Math/Random.h"

// �N���X�F�G�i�C�m�V�V�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class DragonBoar : public Enemy
{
private:
	// �񋓌^�F�G�̏��
	enum class DragonBoarState
	{
		Idle,		// �Î~�ҋ@
		Roar,		// ���K
		Normal,		// �ʏ�ړ�
		Bite,		// ���݂�
		Wince,		// ����
		Death,		// ���S
	};

	// �񋓌^�F�G�̃��[�V����
	enum DragonBoarMotion
	{
		MOTION_IDLE = 4,	// �Î~
		MOTION_ROAR = 2,	// ���K
		MOTION_WALK = 6,	// ���s
		MOTION_BITE = 3,	// ���݂�
		MOTION_DASH = 5,	// �_�b�V��
		MOTION_WINCE = 1,	// ����
		MOTION_DEATH = 0,	// ���S
	};

public:
	// �R���X�g���N�^
	DragonBoar(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3(-12.0f, 14.0f, 0.0f), Matrix::CreateRotationX(90.0f), 20.5f, 10.0f));
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
	// ���ݏ�Ԃł̍X�V
	void wince(float delta_time);
	// ���S��Ԃł̍X�V
	void death(float delta_time);

	// �v���C���[���U���ł��邩
	virtual bool can_attack_player() override;

private:
	// �A�j���[�V�������b�V��
	AnimatedMesh		mesh_{ MESH_DRAGONBOAR };
	// ���[�V�����ԍ�
	int					motion_{ DragonBoarMotion::MOTION_IDLE };
	// �G�̏��
	DragonBoarState		state_{ DragonBoarState::Idle };
	// �O�̏��
	DragonBoarState		previous_state_;
	// ��ԃ^�C�}�[
	float				state_timer_{ 0.0f };
	// ����������
	Random				rand_;

	// �U������𔭐������̂�
	bool				attack_on_{ false };
	// ���̖ړI�n
	Vector3				next_destination_;

	// �ő�̗�
	const int			HP{ 100 };
	// �ړ����x
	const float			WalkSpeed{ 0.28f };
	// ��]���x
	const float			RotateSpeed{ 1.0f };
	// �d��
	const float			Gravity{ 0.03f };

	// ���ݑϐ�
	const int			ToWince{ 5 };
	// ���G����
	const float			Range{ 12.0f };
	// ���G�p�x
	const float			Angle{ 10.0f };
};

#endif // !DRAGON_BOAR_H_