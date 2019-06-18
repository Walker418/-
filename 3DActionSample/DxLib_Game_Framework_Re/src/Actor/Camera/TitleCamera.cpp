#include "TitleCamera.h"
#include "../../Math/MathHelper.h"
#include "../../Graphic/Graphics3D.h"

// �R���X�g���N�^
TitleCamera::TitleCamera(IWorld* world, const Vector3& position) :
	Actor(world, "Camera", position)
{
	rotation_ = Matrix::Identity;
}

// �X�V
void TitleCamera::update(float delta_time)
{
	// �J�����̈ʒu��ݒ�
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + target_;
	// �ړ��A��]����
	rotate(delta_time);
}

// �`��
void TitleCamera::draw() const
{
	// �J������ݒ�
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), pose().Translation() + rotation_.Forward(), rotation_.Up()));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));
}

// �J�����̈ړ��A��]
void TitleCamera::rotate(float delta_time)
{
	// �J�����̊�{���W�Ɖ�]�p�x��ݒ�
	auto position = Vector3{ 0.0f, 0.0f, CameraDistance } *(Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));
	
	// �J�����̍��W�ɍ��������Z
	position_ = position + target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// �J�����̒����_�i�v���C���[�̍��W�j�ɍ��������Z
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// �J�����̉�]�𔽉f
	rotation_ = Matrix::CreateWorld(Vector3::Zero, view_point - position_, Vector3::Up);

	// �J��������]������
	yaw_angle_ += RotateSpeed * delta_time;
}