#include "DragonBoar.h"
#include "../../../World/IWorld.h"
#include "../../../ID/EventMessage.h"
#include "../../Damage.h"

// �N���X�F�G�i�C�m�V�V�j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Enemy(world, "DragonBoar", position, angle, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_IDLE },
	motion_{ DragonBoarMotion::MOTION_IDLE },
	state_{ DragonBoarState::Idle },
	state_timer_{ 0.0f },
	attack_on_{ false },
	anger_timer_{ 0.0f }
{
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
	rand_.randomize();
}

// �X�V
void DragonBoar::update(float delta_time)
{
	// ��������
	velocity_ += Vector3::Down * Gravity;		// �d�͉����x���v�Z
	position_.y += velocity_.y * delta_time;	// y�����W���v�Z
	// �n�ʂƂ̐ڐG����
	intersect_ground();
	// �ǂƂ̐ڐG����
	intersect_wall();

	// �G�̏�Ԃ��X�V
	update_state(delta_time);
	// ���[�V������ύX
	mesh_.change_motion(motion_);
	// ���b�V�����X�V
	mesh_.update(delta_time);
	// �s����v�Z
	mesh_.transform(pose());

	// HP��0�ȉ��ɂȂ�ƁA���S��ԂɈڍs
	if (current_hp_ <= 0 && state_ != DragonBoarState::Death)
	{
		world_->send_message(EventMessage::BossDead);	// ���S���b�Z�[�W�𑗐M
		change_state(DragonBoarState::Death, MOTION_DEATH);
		return;
	}

	// ���ݗݐϒl�����ʂ��z������A���ݏ�ԂɈڍs
	if (current_wince_ >= ToWince && state_ != DragonBoarState::Wince)
	{
		current_wince_ = 0;
		change_state(DragonBoarState::Wince, MOTION_WINCE);
		return;
	}
}

// �`��
void DragonBoar::draw() const
{
	mesh_.draw();	// ���b�V����`��

	// �R���C�_�[��`��i�f�o�b�O���[�h�̂݁A�����p�j
	body_->transform(pose())->draw();
	// �����ŕ����������i�f�o�b�O���[�h�̂݁j
	unsigned int Cr;
	Cr = GetColor(255, 0, 0);
	DrawLine3D(position_, position_ + pose().Forward() * 50.0f, Cr);

	// �f�o�b�O���b�Z�[�W
	// Cr = GetColor(255, 255, 255);
	// DrawFormatString(0, 0, Cr, "�v���C���[�ւ̊p�x�F %f", get_angle_to_player());
	/*
	if (player_in_front())
		DrawString(0, 0, "�v���C���[�͑O�ɂ���", Cr);
	else
		DrawString(0, 0, "�v���C���[�͌�ɂ���", Cr);
		*/
}

// �Փ˃��A�N�V����
void DragonBoar::react(Actor& other)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == DragonBoarState::Death) return;
}

// ���b�Z�[�W����
void DragonBoar::handle_message(EventMessage message, void* param)
{
	// ���S��Ԃł͔������Ȃ�
	if (state_ == DragonBoarState::Death) return;

	// �v���C���[����_���[�W���󂯂�
	if (message == EventMessage::EnemyDamage)
	{
		// ���b�Z�[�W����v���C���[�̍U���͂Ƌ��ݒl���擾���A�_���[�W�v�Z���s��
		Damage* damage = (Damage*)param;
		if (state_ != DragonBoarState::Death)
			current_hp_ -= damage->power;
		if (state_ != DragonBoarState::Wince)
			current_wince_ += damage->impact;
	}
}

// ��Ԃ̍X�V
void DragonBoar::update_state(float delta_time)
{
	// ���݂̏�Ԃɉ����āA�G���X�V
	switch (state_)
	{
	case(DragonBoarState::Idle):
		idle(delta_time);
		break;
	case(DragonBoarState::Roar):
		roar(delta_time);
		break;
	case(DragonBoarState::Normal):
		normal(delta_time);
		break;
	case(DragonBoarState::Bite):
		bite(delta_time);
		break;
	case(DragonBoarState::Anger):
		anger(delta_time);
		break;
	case(DragonBoarState::Wince):
		wince(delta_time);
		break;
	case(DragonBoarState::Death):
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// ��ԃ^�C�}�[�����Z
}

// ��Ԃ̕ύX
void DragonBoar::change_state(DragonBoarState state, int motion)
{
	previous_state_ = state_;

	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
}

// �ҋ@��Ԃł̍X�V
void DragonBoar::idle(float delta_time)
{
	// 5�b��A�ړ��J�n
	if (state_timer_ >= 300.0f)
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// �{��ړ��ֈڍs
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// �ʏ�ړ��ֈڍs
	}
}

// ���K���̍X�V
void DragonBoar::roar(float delta_time)
{
	// ���[�V�����I����A�ړ��J�n
	if (state_timer_ >= 80.0f)
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// �{��ړ��ֈڍs
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// �ʏ�ړ��ֈڍs
	}
}

// �ʏ��Ԃł̈ړ�
void DragonBoar::normal(float delta_time)
{
	// ============================================================
	// �ȉ��͈ړ����̏���

	float interval = 0.0f;						// ���̍s���ւ̈ڍs�^�C�~���O�̕ϐ���錾���Ă���
	next_destination_ = get_player_position();	
}

// ���݂����̍X�V
void DragonBoar::bite(float delta_time)
{

}

// �{���Ԃł̈ړ�
void DragonBoar::anger(float delta_time)
{

}

// ���ݏ�Ԃł̍X�V
void DragonBoar::wince(float delta_time)
{

}

// ���S��Ԃł̍X�V
void DragonBoar::death(float delta_time)
{
	// ���[�V�������I������ƁA���S�����L����
	if (state_timer_ >= 40.0f)
	{
		die();
	}
}

// �v���C���[���U���ł��邩
bool DragonBoar::can_attack_player()
{
	// �v���C���[�����݂��Ȃ��ꍇ�Afalse��Ԃ�
	if (!player_exists()) return false;
	// �v���C���[���O�ɂ��Ȃ���΁A�AFalse��Ԃ�
	if (!player_in_forward()) return false;
	// �v���C���[���U���ł��鋗�����ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_distance(Range)) return false;
	// �v���C���[���U���ł���p�x���ɂ��Ȃ���΁AFalse��Ԃ�
	if (!player_in_range_angle(Angle)) return false;

	// ������S�Ė������Ă���΁ATrue��Ԃ�
	return true;
}