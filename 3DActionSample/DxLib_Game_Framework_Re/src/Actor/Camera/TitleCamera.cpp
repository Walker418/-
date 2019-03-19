#include "TitleCamera.h"
#include "../../Math/MathHelper.h"

// コンストラクタ
TitleCamera::TitleCamera(IWorld* world, const Vector3& position) :
	Actor(world, "TitleCamera", position)
{
	rotation_ = Matrix::Identity;
}

// 更新
void TitleCamera::update(float delta_time)
{
	
}

// 描画
void TitleCamera::draw() const
{

}