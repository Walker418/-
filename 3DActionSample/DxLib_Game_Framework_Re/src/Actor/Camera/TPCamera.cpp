#include "TPCamera.h"
#include "../../World/IWorld.h"
#include "../ActorGroup.h"
#include "../../Graphic/Graphics3D.h"
#include "../../Math/MathHelper.h"
#include "../Player/PlayerInput.h"
#include "../Body/Line.h"
#include "../../Field/Field.h"

TPCamera::TPCamera(IWorld* world) :
	Actor(world, "Camera", Vector3::Zero)
{
	velocity_ = Vector3::Zero;
	yaw_angle_ = 0.0f;
	pitch_angle_ = 0.0f;
}

// �X�V
void TPCamera::update(float delta_time)
{
	// �v���C���[���������A���̈ʒu���擾����
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	if (!player) return;	// �v���C���[�����݂��Ȃ��ꍇ�A�������Ȃ�

	target_ = player->position();
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + target_;

	move(delta_time);	// �ړ��A��]����
	intersect_wall();	// �ǂƂ̐ڐG����
}

// �`��
void TPCamera::draw() const
{
	// ��ʂ��N���A
	Graphics3D::clear();

	// �J������ݒ�
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), pose().Translation() + rotation_.Forward(), rotation_.Up()));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));
}

// �J�����̈ړ��A��]����
void TPCamera::move(float delta_time)
{
	// �J�����̊�{���W�Ɖ�]�p�x��ݒ�
	auto position = Vector3{ 0.0f, 0.0f, CameraDistance } *(Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));

	auto player = world_->find_actor(ActorGroup::Player, "Player");
	if (!player) return;	// �v���C���[�����݂��Ȃ��ꍇ�A�������Ȃ�

	// �J�����̍��W�ɍ��������Z
	position_ = position + target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// �J�����̒����_�i�v���C���[�̍��W�j�ɍ��������Z
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// �J�����̉�]�𔽉f
	rotation_ = Matrix::CreateWorld(Vector3::Zero, view_point - position_, Vector3::Up);

	// ============================================================
	// �ȉ��͉�]����

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
	// �����_�i�v���C���[�j�ƃJ�����̊Ԃɕǂ����邩�ǂ��������m
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