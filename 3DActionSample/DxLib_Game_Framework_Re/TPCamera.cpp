#include "TPCamera.h"
#include "IWorld.h"
#include "ActorGroup.h"
#include "Graphics3D.h"

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

	target_ = player->position() + Vector3(0.0f, CameraHeight, 0.0f);
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + player->position();

	// ��]����
	rotate();
}

// �`��
void TPCamera::draw() const
{
	// ��ʂ��N���A
	Graphics3D::clear();

	// �J������ݒ�
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), target_, Vector3::Up));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));
}

// �J�����̉�]
void TPCamera::rotate()
{

}