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

// 更新
void TPCamera::update(float delta_time)
{
	// プレイヤーを検索し、その位置を取得する
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	if (!player) return;	// プレイヤーが存在しない場合、何もしない

	target_ = player->position();
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + player->position();

	// 移動、回転処理
	move(delta_time);
}

// 描画
void TPCamera::draw() const
{
	// 画面をクリア
	Graphics3D::clear();

	// カメラを設定
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), pose().Translation() + rotation_.Forward(), rotation_.Up()));
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, 640.0f / 480.0f, 0.3f, 1000.0f));
}

// カメラの移動、回転処理
void TPCamera::move(float delta_time)
{
	// カメラの基本座標と回転角度を設定
	auto position = Vector3{ 0.0f, 0.0f, CameraDistance } * (Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));

	auto player = world_->find_actor(ActorGroup::Player, "Player");
	if (!player) return;	// プレイヤーが存在しない場合、何もしない

	// カメラの座標に高さを加算
	position_ = position + target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// カメラの注視点（プレイヤーの座標）に高さを加算
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// カメラの回転を反映
	rotation_ = Matrix::CreateWorld(Vector3::Zero, view_point - position_, Vector3::Up);

	// ============================================================
	// 以下は回転操作

	// 左右回転
	if (CheckHitKey(KEY_INPUT_LEFT))		// 左
	{
		yaw_angle_ -= RotateSpeed * delta_time;
	}
	else if (CheckHitKey(KEY_INPUT_RIGHT))	// 右
	{
		yaw_angle_ += RotateSpeed * delta_time;
	}
	// 上下回転
	if (CheckHitKey(KEY_INPUT_UP))			// 前
	{
		pitch_angle_ += RotateSpeed * delta_time;
	}
	else if (CheckHitKey(KEY_INPUT_DOWN))	// 後
	{
		pitch_angle_ -= RotateSpeed * delta_time;
	}

	// カメラの仰角を制限
	pitch_angle_ = MathHelper::clamp(pitch_angle_, PitchMin, PitchMax);

	// 回転操作終了
	// ============================================================
}