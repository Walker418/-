#include "TitleCamera.h"
#include "../../Math/MathHelper.h"

// �R���X�g���N�^
TitleCamera::TitleCamera(IWorld* world, const Vector3& position) :
	Actor(world, "TitleCamera", position)
{
	rotation_ = Matrix::Identity;
}

// �X�V
void TitleCamera::update(float delta_time)
{
	
}

// �`��
void TitleCamera::draw() const
{

}