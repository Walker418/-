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
	position_ = Vector3(0.0f, CameraHeight, CameraDistance) + target_;

	move();							// 移動
	camera_vibration_V(delta_time);	// 振動
	intersect_wall();				// 壁との接触処理

	// 状態に応じて更新
	update_state(delta_time);

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

	DrawFormatString(0, 200, GetColor(255, 255, 255), "プレイヤー向き角度： %f", get_unsigned_angle_to_target(target_));
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
	rotate(delta_time);				// 回転
	/*
	// カメラリセット
	if (PlayerInput::camera_reset())
	{
		start_reset();
	}
	*/
}

// カメラリセット中の更新
void TPCamera::reset(float delta_time)
{
	Vector3 to_pos = position_ + reset_target_;
	float angle_to_target = get_angle_to_target(to_pos);
	const float AngleToRotate = 0.5f;

	if (angle_to_target > AngleToRotate)
	{

	}
	else if (angle_to_target < -AngleToRotate)
	{

	}

	if (get_unsigned_angle_to_target(to_pos) <= AngleToRotate)
	{
		state_ = TPCameraState::Normal;
	}
}

// 移動処理
void TPCamera::move()
{
	// カメラの基本座標と回転角度を設定
	auto position = Vector3{ 0.0f, 0.0f, CameraDistance } * (Matrix::CreateRotationX(pitch_angle_) * Matrix::CreateRotationY(yaw_angle_));

	// カメラの座標に高さを加算
	position_ = position + target_ + Vector3(0.0f, CameraHeight, 0.0f);
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
	reset_target_ = target_;

	state_ = TPCameraState::Reset;
}

// プレイヤーの参照の取得
ActorPtr TPCamera::get_player()
{
	return world_->find_actor(ActorGroup::Player, "Player");
}

// 注視点への角度を取得（符号付き）
float TPCamera::get_angle_to_target(Vector3 target) const
{
	Vector3 forward_cross_target = Vector3::Cross(rotation_.Forward(), target);

	// 外積で目標への角度を計算し、角度の値を返す
	float angle = (forward_cross_target.y >= 0.0f) ?
		Vector3::Angle(position_, target) :
		-Vector3::Angle(position_, target);

	return angle;
}

// 注視点への角度を取得（符号無し）
float TPCamera::get_unsigned_angle_to_target(Vector3 target) const
{
	return std::abs(get_angle_to_target(target));
}