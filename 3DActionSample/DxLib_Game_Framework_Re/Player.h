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
		Normal,		// �ʏ�
		Attack,		// �U��
		Guard,		// �K�[�h
		Damage,		// ��e
		Dead,		// ���S
	};

	// �񋓌^�F�v���C���[�̃��[�V����
	enum PlayerMotion
	{
		MOTION_IDLE = 28,	// �Î~�i�ʏ�ҋ@�j
	};

public:
	// �R���X�g���N�^
	Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3{ 0.0f, 9.2f, 0.0f }, Matrix::Identity, 2.2f, 14.5f));
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
	void normal_update(float delta_time);
	// �U����Ԃł̍X�V
	void attack_update(float delta_time);
	// �K�[�h��Ԃł̍X�V
	void guard_update(float delta_time);
	// ��e��Ԃł̍X�V
	void damage_update(float delta_time);
	// ���S��Ԃł̍X�V
	void dead_update(float delta_time);

private:
	// �A�j���[�V�������b�V��
	AnimatedMesh	mesh_{ MESH_PALADIN };
	// ���[�V�����ԍ�
	int				motion_{ PlayerMotion::MOTION_IDLE };
	// �v���[���[�̏��
	PlayerState		state_{ PlayerState::Normal };
	// ��ԃ^�C�}�[
	float			state_timer_{ 0.0f };
};

#endif // !PLAYER_H_