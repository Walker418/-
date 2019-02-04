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

// 更新
void TPCamera::update(float delta_time)
{
	// プレイヤーを検索し、その位置を取得する
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	if (!player) return;	// プレイヤーが存在しない場合、何もしない

	target_ = player->position() + Vector3(0.0f, CameraHeight, 0.0f);
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + player->position();

	// 回転処理
	rotate();
}

// 描画
void TPCamera::draw() const
{
	// 画面をクリア
	Graphics3D::clear();

	// カメラを設定
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), target_, Vector3::Up));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));
}

// カメラの回転
void TPCamera::rotate()
{

}