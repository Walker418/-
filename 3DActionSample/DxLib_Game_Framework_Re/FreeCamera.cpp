#include "FreeCamera.h"
#include "IWorld.h"
#include "Graphics3D.h"

// コンストラクタ
FreeCamera::FreeCamera(IWorld* world, const Vector3& position, const Matrix& rotation) :
	Actor(world, "Camera", position)
{
	rotation_ = rotation;
}

// 更新
void FreeCamera::update(float delta_time)
{

}

// 描画
void FreeCamera::draw() const
{
	// 画面をクリア
	Graphics3D::clear();

	// カメラを設定
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), pose().Translation() + pose().Forward(), { 0.0f, 1.0f, 0.0f }));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));
}