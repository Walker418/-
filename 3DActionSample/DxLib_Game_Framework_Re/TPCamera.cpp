#include "TPCamera.h"

TPCamera::TPCamera(IWorld* world) :
	Actor(world, "Camera", Vector3::Zero)
{ }

// �X�V
void TPCamera::update(float delta_time)
{

}

// �`��
void TPCamera::draw() const
{

}

// �ړ�
void TPCamera::move(const Vector3& rest_position, float stiffness, float friction, float mass)
{

}