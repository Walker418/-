#ifndef DRAGON_BOAR_H_
#define DRAGON_BOAR_H_

#include "../Enemy.h"
#include "../../../Graphic/AnimatedMesh.h"
#include "../../Body/IBodyPtr.h"
#include "../../Body/BoundingSphere.h"
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
		Normal,		// �ʏ�
		Bite,		// ���݂�
		Anger,		// �{��
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
		MOTION_DASH = 5,	// �ːi
		MOTION_WINCE = 1,	// ����
		MOTION_DEATH = 0,	// ���S
	};

public:
	// �R���X�g���N�^
	DragonBoar(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3(0.0f, 14.0f, 0.0f), 10.5f));
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
	// ���K���̍X�V
	void roar(float delta_time);
	// �ʏ��Ԃł̈ړ�
	void normal(float delta_time);
	// ���݂����̍X�V
	void bite(float delta_time);
	// �{���Ԃł̈ړ�
	void anger(float delta_time);
	// ���ݏ�Ԃł̍X�V
	void wince(float delta_time);
	// ���S��Ԃł̍X�V
	void death(float delta_time);

	// ���̍s���𒊑I
	void next_action();

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
	// �ːi�U���̔���𔭐������̂�
	bool				dash_attack_on_{ false };
	// ���̖ړI�n
	Vector3				next_destination_;
	// ���K��SE���Đ�������
	bool				roar_started_{ false };
	// �ړ����Ȃ̂�
	bool				is_moving_{ false };
	// �{���ԂȂ̂�
	bool				is_anger_{ false };
	// �{���ԃ^�C�}�[
	float				anger_timer_{ 0.0f };
	// �{��ːi�̎�������
	float				dash_timer_{ 0.0f };

	// �ő�̗�
	const int			HP{ 100 };
	// ���s���x
	const float			WalkSpeed{ 0.28f };
	// �ːi���x
	const float			DashSpeed{ 0.5f };
	// ��]���x
	const float			RotateSpeed{ 0.8f };
	// �d��
	const float			Gravity{ 0.03f };

	// ���ݑϐ�
	const int			ToWince{ 15 };
	// ���G����
	const float			Range{ 45.0f };
	// ���G�p�x
	const float			Angle{ 10.0f };
};

#endif // !DRAGON_BOAR_H_