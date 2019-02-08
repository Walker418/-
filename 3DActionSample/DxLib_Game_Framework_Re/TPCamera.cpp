#include "TPCamera.h"
#include "IWorld.h"
#include "ActorGroup.h"
#include "Graphics3D.h"
#include "MathHelper.h"

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
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + player->position();

	// �ړ��A��]����
	move(delta_time);
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
	auto position = Vector3{ 0.0f, 0.0f, CameraDistance } * (Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));

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
	if (CheckHitKey(KEY_INPUT_LEFT))		// ��
	{
		yaw_angle_ -= RotateSpeed * delta_time;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT))	// �E
	{
		yaw_angle_ += RotateSpeed * delta_time;
	}
	// �㉺��]
	if (CheckHitKey(KEY_INPUT_UP))			// �O
	{
		pitch_angle_ += RotateSpeed * delta_time;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN))	// ��
	{
		pitch_angle_ -= RotateSpeed * delta_time;
	}

	// �J�����̋p�𐧌�
	pitch_angle_ = MathHelper::clamp(pitch_angle_, PitchMin, PitchMax);

	// ��]����I��
	// ============================================================
}