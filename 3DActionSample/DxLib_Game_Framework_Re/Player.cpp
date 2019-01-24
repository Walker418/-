#include "Player.h"
#include "GamePad.h"
#include "MathHelper.h"
#include "IWorld.h"
#include "EventMessage.h"
#include <DxLib.h>
#include "Field.h"
#include "Line.h"
#include "FreeCamera.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Player::Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	state_timer_{ 0.0f }
{
	rotation_ = rotation;
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
}

// 更新
void Player::update(float delta_time)
{
	/*
	// モーションの切り替えテスト
	if (GamePad::trigger(GamePad::Up))
		motion_ = (motion_ + 1) % 25;
	if (GamePad::trigger(GamePad::Down))
		motion_ = ((motion_ - 1) + 25) % 25;

	// モーションを変更
	mesh_.change_motion(motion_);
	// アニメーションを更新
	mesh_.update(delta_time);
	// 行列を設定
	mesh_.transform(pose());
	*/

	// 落下処理
	velocity_ += Vector3::Down * Gravity;		// 重力加速度を計算
	position_.y += velocity_.y * delta_time;	// y軸座標を計算
	// 地面との接触処理
	intersect_ground();

	// プレーヤーの状態を更新
	update_state(delta_time);
	// モーションを変更
	mesh_.change_motion(motion_);
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を計算
	mesh_.transform(pose());

	// HPが0以下になると、死亡状態に移行
	if (current_hp_ <= 0 && state_ != PlayerState::Death)
	{
		change_state(PlayerState::Death, MOTION_DEATH);
	}
}

// 描画
void Player::draw() const
{
	mesh_.draw();							// メッシュを描画
	body_->translate(position_)->draw();	// コライダーを描画（デバッグモードのみ、調整用）

	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	// DrawFormatString(0, 30, Cr, "状態タイマー：（ %f ）", state_timer_);
	/*
	switch (state_)
	{
	case PlayerState::Normal:
		DrawFormatString(0, 30, Cr, "State = Normal");
		break;
	case PlayerState::Slash1:
		DrawFormatString(0, 30, Cr, "State = Slash1");
		break;
	case PlayerState::Slash2:
		DrawFormatString(0, 30, Cr, "State = Slash2");
		break;
	case PlayerState::Slash3:
		DrawFormatString(0, 30, Cr, "State = Slash3");
		break;
	case PlayerState::Guard:
		DrawFormatString(0, 30, Cr, "State = Guard");
		break;
	case PlayerState::GuardEnd:
		DrawFormatString(0, 30, Cr, "State = GuardEnd");
		break;
	case PlayerState::Blocking:
		DrawFormatString(0, 30, Cr, "State = Blocking");
		break;
	case PlayerState::GuardAttack:
		DrawFormatString(0, 30, Cr, "State = GuardAttack");
		break;
	case PlayerState::Damage:
		DrawFormatString(0, 30, Cr, "State = Damage");
		break;
	case PlayerState::Death:
		DrawFormatString(0, 30, Cr, "State = Death");
		break;
	default:
		break;
	}
	*/
}

// 衝突リアクション
void Player::react(Actor& other)
{

}

// メッセージ処理
void Player::handle_message(EventMessage message, void* param)
{

}

// 状態の更新
void Player::update_state(float delta_time)
{
	// 現在の状態に応じて、プレイヤーを更新
	switch (state_)
	{
	case PlayerState::Normal:
		normal(delta_time);
		break;
	case PlayerState::Slash1:
		slash1(delta_time);
		break;
	case PlayerState::Slash2:
		slash2(delta_time);
		break;
	case PlayerState::Slash3:
		slash3(delta_time);
		break;
	case PlayerState::Damage:
		damage(delta_time);
		break;
	case PlayerState::Guard:
		guard(delta_time);
		break;
	case PlayerState::Blocking:
		blocking(delta_time);
		break;
	case PlayerState::GuardAttack:
		guard_attack(delta_time);
		break;
	case PlayerState::GuardEnd:
		guard_end(delta_time);
		break;
	case PlayerState::Death:
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// 状態タイマーの加算
}

// 状態の変更
void Player::change_state(PlayerState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;
}

// 通常状態での更新
void Player::normal(float delta_time)
{
	// Xキーが押されると、攻撃する
	if (GamePad::trigger(GamePad::X))
	{
		// 攻撃状態に移行
		change_state(PlayerState::Slash1, MOTION_SLASH_1);

		return;
	}

	// Rキーが押されると、ガード状態に移行
	if (GamePad::state(GamePad::R))
	{
		change_state(PlayerState::Guard, MOTION_GUARD_BEGIN);

		return;
	}

	/*
	// 死亡テスト
	if (GamePad::trigger(GamePad::R))
	{
		change_state(PlayerState::Death, MOTION_DEATH);

		return;
	}
	*/

	// ============================================================
	// 以下は移動処理
	int motion{ MOTION_IDLE };		// 何もしなければ、待機モーションに変更
	// x,z軸移動量をリセット
	velocity_.x = 0;
	velocity_.z = 0;
	float forward_speed{ 0.0f };	// 前向き速度
	float left_speed{ 0.0f };		// 左向き速度

	// 前後移動
	if (GamePad::state(GamePad::Up))
	{
		forward_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Down))
	{
		forward_speed = -WalkSpeed;
	}
	// 左右移動
	if (GamePad::state(GamePad::Left))
	{
		left_speed = WalkSpeed;
	}
	else if (GamePad::state(GamePad::Right))
	{
		left_speed = -WalkSpeed;
	}
	// 移動していれば、歩行モーションに変更
	if (forward_speed != 0.0f || left_speed != 0.0f)
		motion = MOTION_WALK;

	// 状態を更新
	change_state(PlayerState::Normal, motion);

	// カメラを取得
	auto camera = world_->camera()->pose();
	// カメラの正面ベクトルを取得
	auto camera_forward = camera.Forward();
	// カメラのy軸成分を無視する
	camera_forward.y = 0;
	// 正規化
	camera_forward.Normalize();

	// 移動量を計算し、プレイヤーを移動させる
	velocity_ += camera_forward * forward_speed;
	velocity_ += camera.Left() * left_speed;
	position_ += velocity_ * delta_time;

	// 移動処理終了
	// ============================================================
}

// 攻撃（1段目）での更新
void Player::slash1(float delta_time)
{
	// モーション終了の前に、Xキーが押されると、攻撃の2段階目に移行
	if (GamePad::trigger(GamePad::X))
	{
		if (state_timer_ >= mesh_.motion_end_time() + 5.5f && state_timer_ < mesh_.motion_end_time() + 18.0f)
		{
			change_state(PlayerState::Slash2, MOTION_SLASH_2);
			return;
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		normal(delta_time);
	}
}

// 攻撃（2段目）での更新
void Player::slash2(float delta_time)
{
	// モーション終了の前に、Xキーが押されると、攻撃の3段階目に移行
	if (GamePad::trigger(GamePad::X))
	{
		if (state_timer_ >= mesh_.motion_end_time() + 5.0f && state_timer_ < mesh_.motion_end_time() + 18.0f)
		{
			change_state(PlayerState::Slash3, MOTION_SLASH_3);
			return;
		}
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() + 15.0f)
	{
		normal(delta_time);
	}
}

// 攻撃（3段目）での更新
void Player::slash3(float delta_time)
{
	// モーション再生の間、キャラクターを前進させる
	if (state_timer_ <= mesh_.motion_end_time() + 28.0f)
	{
		velocity_ = rotation_.Forward() * 0.15f;
		position_ += velocity_ * delta_time;
	}

	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() + 45.0f)
	{
		normal(delta_time);
	}
}

// 被弾状態での更新
void Player::damage(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time())
	{
		normal(delta_time);
	}
}

// ガード時の更新
void Player::guard(float delta_time)
{
	// ガード開始のモーション終了後、ガード待機モーションを再生し、
	// 以降Rキーが離れたら、ガード終了状態に移行
	float guard_ready_time = mesh_.motion_end_time() + 15.0f;	// ガード開始までの時間;
	if (state_timer_ >= guard_ready_time)
	{
		motion_ = MOTION_GUARD_IDLE;	// ガード中のモーションに移行

		// Xキーが押されると、ガード攻撃を使用
		if (GamePad::trigger(GamePad::X))
		{
			change_state(PlayerState::GuardAttack, MOTION_GUARD_SLASH);
		}

		// Rキーが離れたら、ガード終了状態に移行
		if (!GamePad::state(GamePad::R))
		{
			change_state(PlayerState::GuardEnd, MOTION_GUARD_END);
		}
	}
}

// ガードによるノックバック中の更新
void Player::blocking(float delta_time)
{
	// モーション終了後、ガード状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		state_ = PlayerState::Guard;
		guard(delta_time);
	}
}

// ガード攻撃での更新
void Player::guard_attack(float delta_time)
{
	// モーション終了後、ガード状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 1.8f)
	{
		state_ = PlayerState::Guard;
		guard(delta_time);
	}
}

// ガード終了時の更新
void Player::guard_end(float delta_time)
{
	// モーション終了後、通常状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() + 10.0f)
	{
		normal(delta_time);
	}
}

// 死亡状態での更新
void Player::death(float delta_time)
{
	// モーションが終了すると、死亡判定を有効に
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		world_->send_message(EventMessage::PlayerDead);
		die();
	}
}

// 地面との接触処理
void Player::intersect_ground()
{
	// フィールドを取得
	auto& field = world_->field();
	// 地面との接触点
	Vector3 intersect;

	// 地面に接触した場合、接触点を返す
	if (field.collide_line(position_ + Vector3(0.0f, 10.0f, 0.0f), position_ - Vector3(0.0f, 1.0f, 0.0f), &intersect))
	{
		// 接地した場合、y軸座標を補正する（地面のめり込まない）
		if (intersect.y >= position_.y)
		{
			velocity_.y = 0;			// y軸移動量を0にする
			position_.y = intersect.y;
		}
	}
}