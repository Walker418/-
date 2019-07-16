#include "TPCamera.h"
#include "../../World/IWorld.h"
#include "../ActorGroup.h"
#include "../../Graphic/Graphics3D.h"
#include "../../Math/MathHelper.h"
#include "../Player/PlayerInput.h"
#include "../Body/Line.h"
#include "../../Field/Field.h"
#include "../../ID/EventMessage.h"
#include "../../Game/WindowSetting.h"

TPCamera::TPCamera(IWorld* world) :
	Actor(world, "Camera", Vector3::Zero)
{
	velocity_ = Vector3::Zero;
	yaw_angle_ = 0.0f;
	pitch_angle_ = 0.0f;

	state_ = TPCameraState::Normal;
	vibration_timer_.shut();
	min_pos_y_ = 0.0f;
	max_pos_y_ = 0.0f;
	rand_.randomize();

	target_backward_ = Vector3::Backward;
}

// メッセージ処理
void TPCamera::handle_message(EventMessage message, void * param)
{
	// カメラを振動させる
	if (message == EventMessage::Camera_Vibration)
	{
		start_vibration();
	}
}

// 更新
void TPCamera::update(float delta_time)
{
	// プレイヤーが存在しない場合、何もしない
	if (get_player() == nullptr) return;

	// カメラがプレイヤーに追従する
	target_ = get_player()->position();

	update_state(delta_time);		// 状態に応じて更新
	camera_vibration_V(delta_time);	// 振動
	intersect_wall();				// 壁との接触処理

	// 正規化
	rotation_ = Matrix::NormalizeRotationMatrix(rotation_);
}

// 描画
void TPCamera::draw() const
{
	// カメラを設定
	Graphics3D::set_view_matrix(Matrix::CreateLookAt(pose().Translation(), pose().Translation() + rotation_.Forward(), rotation_.Up()));
	float width = WindowSetting::WindowWidth;
	float height = WindowSetting::WindowHeight;
	Graphics3D::set_projection_matrix(Matrix::CreatePerspectiveFieldOfView(45.0f, width / height, 0.3f, 1000.0f));

	// 検証メッセージ
	auto Cr = GetColor(255, 255, 255);
	/*float Tx = target_backward_.x;
	float Ty = target_backward_.y;
	float Tz = target_backward_.z;
	DrawFormatString(0, 200, Cr, "カメラリセットの予定座標｛%f、%f、%f｝", Tx, Ty, Tz);*/
	// DrawSphere3D(position_, 0.4f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
	// DrawSphere3D(target_backward_, 0.5f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
}

// 状態の更新
void TPCamera::update_state(float delta_time)
{
	switch (state_)
	{
	case TPCameraState::Normal:
		normal(delta_time);
		break;
	case TPCameraState::Reset:
		reset(delta_time);
		break;
	default:
		break;
	}
}

// 通常状態での更新
void TPCamera::normal(float delta_time)
{
	move();					// 移動
	rotate(delta_time);		// 回転

	// カメラリセット
	if (PlayerInput::camera_reset())
	{
		start_reset();
	}
}

// カメラリセット中の更新
void TPCamera::reset(float delta_time)
{
	target_backward_ += get_player()->get_velocity() * delta_time;
}

// プレイヤーを追従して移動
void TPCamera::move()
{
	// カメラに基本座標と回転角度を設定
	auto pos = Vector3{ 0.0f, 0.0f, CameraDistance } *(Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));

	// カメラの座標に高さを加算
	position_ = pos + target_ + Vector3(0.0f, CameraHeight, 0.0f);
}

// 回転処理
void TPCamera::rotate(float delta_time)
{
	// カメラの注視点（プレイヤーの座標）に高さを加算
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);
	// カメラの回転を反映
	rotation_ = Matrix::CreateWorld(Vector3::Zero, view_point - position_, Vector3::Up);

	// ============================================================
	// 回転操作
	// ============================================================

	// 左右回転
	if (PlayerInput::camera_turn_left())		// 左
	{
		yaw_angle_ -= YawSpeed * delta_time;
	}
	else if (PlayerInput::camera_turn_right())	// 右
	{
		yaw_angle_ += YawSpeed * delta_time;
	}
	// 上下回転
	if (PlayerInput::camera_turn_up())			// 上
	{
		pitch_angle_ += PitchSpeed * delta_time;
	}
	else if (PlayerInput::camera_turn_down())	// 下
	{
		pitch_angle_ -= PitchSpeed * delta_time;
	}

	// カメラの仰角を制限
	pitch_angle_ = MathHelper::clamp(pitch_angle_, PitchMin, PitchMax);

	// 回転操作終了
	// ============================================================
}

// 壁との接触処理
void TPCamera::intersect_wall()
{
	// 線分で注視点（プレイヤー）とカメラの間に壁があるかどうかを検知
	auto view_point = target_ + Vector3(0.0f, CameraHeight, 0.0f);						// 注視点
	auto pos = Vector3{ 0.0f, 0.0f, CameraDistance }
	*(Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));	// カメラの基本位置
	auto pos_height = pos + target_ + Vector3(0.0f, CameraHeight, 0.0f);				// 高さ込みのカメラ位置

	auto& field = world_->field();				// フィールドを取得
	Vector3 intersect;							// 壁との接触点
	Line line = Line(view_point, pos_height);	// 接触判定用線分

	// 壁があった場合、カメラの座標を補正
	if (field.collide_line(line.start, line.end, &intersect))
		position_ = intersect;
}

// カメラの上下振動
void TPCamera::camera_vibration_V(float delta_time)
{
	// 振動タイマーが0以下の場合は振動しない
	if (vibration_timer_.is_time_out()) return;

	// ============================================================
	// カメラの振動処理
	// ============================================================

	auto original_pos = position_;	// カメラの本来の位置

	// カメラのブレ度合いを取得し、振動させる
	float val_y = rand_.rand_float(min_pos_y_, max_pos_y_);
	Vector3 vib_pos = Vector3{ 0.0f, val_y, 0.0f };
	position_ += vib_pos;

	// 振動力は時間につれ減少する
	min_pos_y_ *= 0.9f;
	max_pos_y_ *= 0.9f;

	// ============================================================

	// 振動タイマーを更新
	vibration_timer_.update(delta_time);
}

// 振動開始
void TPCamera::start_vibration()
{
	// 振動力およびカメラの座標ブレ上限を設定
	current_power_ = VibrationPower;
	min_pos_y_ = -current_power_;
	max_pos_y_ = current_power_;

	// 振動タイマーをリセット
	vibration_timer_.reset();
}

// カメラリセット開始
void TPCamera::start_reset()
{
	// リセットボタンが押された時点でのプレイヤー背後方向を取得
	target_backward_ = target_ + get_player()->pose().Backward() * CameraDistance;

	// カメラを回転させる
	state_ = TPCameraState::Reset;
}

// プレイヤーの参照の取得
ActorPtr TPCamera::get_player()
{
	return world_->find_actor(ActorGroup::Player, "Player");
}

// プレイヤー向きの角度の取得（符号付き）
float TPCamera::get_angle_to_target(Vector3 target) const
{
	return 0.0f;
}

// プレイヤー向きの角度の取得（符号付き）
float TPCamera::get_unsigned_angle_to_target(Vector3 target) const
{
	return 0.0f;
}