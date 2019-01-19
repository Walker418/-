#include "Player.h"
#include "GamePad.h"

#define STR(var) #var

// �N���X�F�v���C���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Player::Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	state_timer_{ 0.0f },
	current_hp_{ HP }
{
	rotation_ = rotation;
	velocity_ = Vector3::Zero;
}

// �X�V
void Player::update(float delta_time)
{
	// �v���[���[�̏�Ԃ��X�V
	update_state(delta_time);
	// ���[�V������ύX
	mesh_.change_motion(motion_);
	// ���b�V�����X�V
	mesh_.update(delta_time);
	// �s����v�Z
	mesh_.transform(pose());

	// HP��0�ȉ��ɂȂ�ƁA���S��ԂɈڍs
	
}

// �`��
void Player::draw() const
{
	mesh_.draw();							// ���b�V����`��
	body_->translate(position_)->draw();	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j

	// �ȉ��̓f�o�b�O�\��
	// �v���C���[�̏�Ԃ�\��
	/*
	switch (state_)
	{
	case (PlayerState::Normal):
		DrawString(0, 0, "�v���C���[�̏�ԁFNormal", GetColor(255, 255, 255));
		break;
	case (PlayerState::Attack):
		DrawString(0, 0, "�v���C���[�̏�ԁFAttack", GetColor(255, 255, 255));
		break;
	default:
		break;
	}
	// ��ԃ^�C�}�[��\��
	DrawFormatString(0, 15, GetColor(255, 255, 255), "��ԃ^�C�}�[�F %f ", state_timer_);

	if (GamePad::state(GamePad::A))
	{
		DrawString(0, 30, "A�L�[�������ꂽ", GetColor(255, 255, 255));
	}
	*/
	/*
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DrawFormatString(0, 0, Cr, "�v���C���[�̍��W�F �i %f �A %f�j", position_.x, position_.y);
	*/
}

// �Փ˃��A�N�V����
void Player::react(Actor& other)
{

}

// ���b�Z�[�W����
void Player::handle_message(EventMessage message, void* param)
{

}

// ��Ԃ̍X�V
void Player::update_state(float delta_time)
{
	// ��ԑJ��
	switch (state_)
	{
	case PlayerState::Normal:
		normal_update(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// ��ԃ^�C�}�[�̉��Z
}

// ��Ԃ̕ύX
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;
}

// �ʏ��Ԃł̍X�V
void Player::normal_update(float delta_time)
{
	// X�L�[���������ƁA�U������
	if (GamePad::state(GamePad::X))
	{
		// �U����ԂɈڍs

	}

	// ============================================================
	// �ȉ��͈ړ�����
	int motion{ MOTION_IDLE };		// �������Ȃ���΁A�ҋ@���[�V�����ɕύX
	velocity_ = Vector3::Zero;		// �ړ��ʂ����Z�b�g
	float forward_speed{ 0.0f };	// �O�������x
	float left_speed{ 0.0f };		// ���������x

	// �O��ړ�
	if (GamePad::state(GamePad::Up))
	{
		forward_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Down))
	{
		forward_speed = -WalkSpeed;
	}
	// ���E�ړ�
	if (GamePad::state(GamePad::Left))
	{
		left_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Right))
	{
		left_speed = -WalkSpeed;
	}
	// �ړ����Ă���΁A���s���[�V�����ɕύX
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_WALK;

	// ��Ԃ��X�V
	change_state(PlayerState::Normal, motion);
	// �ړ��ʂ��v�Z���A�v���C���[���ړ�������
	velocity_ += rotation_.Forward() * forward_speed;
	velocity_ += rotation_.Left() * left_speed;
	position_ += velocity_ * delta_time;

	// �ړ������I��
	// ============================================================

}