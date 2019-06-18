#include "TitleCamera.h"
#include "../../Math/MathHelper.h"
#include "../../Graphic/Graphics3D.h"

// コンストラクタ
TitleCamera::TitleCamera(IWorld* world, const Vector3& position) :
	Actor(world, "Camera", position)
{
	rotation_ = Matrix::Identity;
}

// 更新
void TitleCamera::update(float delta_time)
{
	// カメラの位置を設定
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + target_;
	// 移動、回転処理
	rotate(delta_time);
}

// 描画
void TitleCamera::draw() const
{
	// カメラを設定
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), pose().Translation() + rotation_.Forward(), rotation_.Up()));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));
}

// カメラの移動、回転
void TitleCamera::rotate(float delta_time)
{
	// カメラの基本座標と回転角度を設定
	auto position = Vector3{ 0.0f, 0.0f, CameraDistance } *(Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));
	
	// カメラの座標に高さを加算
	position_ = position + target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// カメラの注視点（プレイヤーの座標）に高さを加算
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// カメラの回転を反映
	rotation_ = Matrix::CreateWorld(Vector3::Zero, view_point - position_, Vector3::Up);

	// カメラを回転させる
	yaw_angle_ += RotateSpeed * delta_time;
}