#include "TPCamera.h"
#include "../../World/IWorld.h"
#include "../ActorGroup.h"
#include "../../Graphic/Graphics3D.h"
#include "../../Math/MathHelper.h"
#include "../Player/PlayerInput.h"
#include "../Body/Line.h"
#include "../../Field/Field.h"
#include "../../ID/EventMessage.h"
#include "../../Game/WindowSetting.h"

TPCamera::TPCamera(IWorld* world) :
	Actor(world, "Camera", Vector3::Zero)
{
	velocity_ = Vector3::Zero;
	yaw_angle_ = 0.0f;
	pitch_angle_ = 0.0f;

	state_ = TPCameraState::Normal;
	vibration_timer_.shut();
	min_pos_y_ = 0.0f;
	max_pos_y_ = 0.0f;
	rand_.randomize();

	target_backward_ = Vector3::Backward;
}

// ���b�Z�[�W����
void TPCamera::handle_message(EventMessage message, void * param)
{
	// �J������U��������
	if (message == EventMessage::Camera_Vibration)
	{
		start_vibration();
	}
}

// �X�V
void TPCamera::update(float delta_time)
{
	// �v���C���[�����݂��Ȃ��ꍇ�A�������Ȃ�
	if (get_player() == nullptr) return;

	// �J�������v���C���[�ɒǏ]����
	target_ = get_player()->position();

	update_state(delta_time);		// ��Ԃɉ����čX�V
	camera_vibration_V(delta_time);	// �U��
	intersect_wall();				// �ǂƂ̐ڐG����

	// ���K��
	rotation_ = Matrix::NormalizeRotationMatrix(rotation_);
}

// �`��
void TPCamera::draw() const
{
	// �J������ݒ�
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), pose().Translation() + rotation_.Forward(), rotation_.Up()));
	float width = WindowSetting::WindowWidth;
	float height = WindowSetting::WindowHeight;
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, width / height, 0.3f, 1000.0f));

	// ���؃��b�Z�[�W
	auto Cr = GetColor(255, 255, 255);
	/*float Tx = target_backward_.x;
	float Ty = target_backward_.y;
	float Tz = target_backward_.z;
	DrawFormatString(0, 200, Cr, "�J�������Z�b�g�̗\����W�o%f�A%f�A%f�p", Tx, Ty, Tz);*/
	// DrawSphere3D(position_, 0.4f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
	// DrawSphere3D(target_backward_, 0.5f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
}

// ��Ԃ̍X�V
void TPCamera::update_state(float delta_time)
{
	switch (state_)
	{
	case TPCameraState::Normal:
		normal(delta_time);
		break;
	case TPCameraState::Reset:
		reset(delta_time);
		break;
	default:
		break;
	}
}

// �ʏ��Ԃł̍X�V
void TPCamera::normal(float delta_time)
{
	move();					// �ړ�
	rotate(delta_time);		// ��]

	// �J�������Z�b�g
	if (PlayerInput::camera_reset())
	{
		start_reset();
	}
}

// �J�������Z�b�g���̍X�V
void TPCamera::reset(float delta_time)
{
	target_backward_ += get_player()->get_velocity() * delta_time;
}

// �v���C���[��Ǐ]���Ĉړ�
void TPCamera::move()
{
	// �J�����Ɋ�{���W�Ɖ�]�p�x��ݒ�
	auto pos = Vector3{ 0.0f, 0.0f, CameraDistance } *(Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));

	// �J�����̍��W�ɍ��������Z
	position_ = pos + target_ + Vector3(0.0f, CameraHeight, 0.0f);
}

// ��]����
void TPCamera::rotate(float delta_time)
{
	// �J�����̒����_�i�v���C���[�̍��W�j�ɍ��������Z
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// �J�����̉�]�𔽉f
	rotation_ = Matrix::CreateWorld(Vector3::Zero, view_point - position_, Vector3::Up);

	// ============================================================
	// ��]����
	// ============================================================

	// ���E��]
	if (PlayerInput::camera_turn_left())		// ��
	{
		yaw_angle_ -= YawSpeed * delta_time;
	}
	else if (PlayerInput::camera_turn_right())	// �E
	{
		yaw_angle_ += YawSpeed * delta_time;
	}
	// �㉺��]
	if (PlayerInput::camera_turn_up())			// ��
	{
		pitch_angle_ += PitchSpeed * delta_time;
	}
	else if (PlayerInput::camera_turn_down())	// ��
	{
		pitch_angle_ -= PitchSpeed * delta_time;
	}

	// �J�����̋p�𐧌�
	pitch_angle_ = MathHelper::clamp(pitch_angle_, PitchMin, PitchMax);

	// ��]����I��
	// ============================================================
}

// �ǂƂ̐ڐG����
void TPCamera::intersect_wall()
{
	// �����Œ����_�i�v���C���[�j�ƃJ�����̊Ԃɕǂ����邩�ǂ��������m
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);						// �����_
	auto pos = Vector3{ 0.0f, 0.0f, CameraDistance }
	*(Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));	// �J�����̊�{�ʒu
	auto pos_height = pos + target_ + Vector3(0.0f, CameraHeight, 0.0f);				// �������݂̃J�����ʒu

	auto& field = world_->field();				// �t�B�[���h���擾
	Vector3 intersect;							// �ǂƂ̐ڐG�_
	Line line = Line(view_point, pos_height);	// �ڐG����p����

	// �ǂ��������ꍇ�A�J�����̍��W��␳
	if (field.collide_line(line.start, line.end, &intersect))
		position_ = intersect;
}

// �J�����̏㉺�U��
void TPCamera::camera_vibration_V(float delta_time)
{
	// �U���^�C�}�[��0�ȉ��̏ꍇ�͐U�����Ȃ�
	if (vibration_timer_.is_time_out()) return;

	// ============================================================
	// �J�����̐U������
	// ============================================================

	auto original_pos = position_;	// �J�����̖{���̈ʒu

	// �J�����̃u���x�������擾���A�U��������
	float val_y = rand_.rand_float(min_pos_y_, max_pos_y_);
	Vector3 vib_pos = Vector3{ 0.0f, val_y, 0.0f };
	position_ += vib_pos;

	// �U���͎͂��Ԃɂꌸ������
	min_pos_y_ *= 0.9f;
	max_pos_y_ *= 0.9f;

	// ============================================================

	// �U���^�C�}�[���X�V
	vibration_timer_.update(delta_time);
}

// �U���J�n
void TPCamera::start_vibration()
{
	// �U���͂���уJ�����̍��W�u�������ݒ�
	current_power_ = VibrationPower;
	min_pos_y_ = -current_power_;
	max_pos_y_ = current_power_;

	// �U���^�C�}�[�����Z�b�g
	vibration_timer_.reset();
}

// �J�������Z�b�g�J�n
void TPCamera::start_reset()
{
	// ���Z�b�g�{�^���������ꂽ���_�ł̃v���C���[�w��������擾
	target_backward_ = target_ + get_player()->pose().Backward() * CameraDistance;

	// �J��������]������
	state_ = TPCameraState::Reset;
}

// �v���C���[�̎Q�Ƃ̎擾
ActorPtr TPCamera::get_player()
{
	return world_->find_actor(ActorGroup::Player, "Player");
}

// �v���C���[�����̊p�x�̎擾�i�����t���j
float TPCamera::get_angle_to_target(Vector3 target) const
{
	return 0.0f;
}

// �v���C���[�����̊p�x�̎擾�i�����t���j
float TPCamera::get_unsigned_angle_to_target(Vector3 target) const
{
	return 0.0f;
}