#include "Player.h"
#include "GamePad.h"

#define STR(var) #var

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Player::Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE },
	state_{ PlayerState::Normal },
	state_timer_{ 0.0f },
	current_hp_{ HP }
{
	rotation_ = rotation;
	velocity_ = Vector3::Zero;
}

// 更新
void Player::update(float delta_time)
{
	// プレーヤーの状態を更新
	update_state(delta_time);
	// モーションを変更
	mesh_.change_motion(motion_);
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を計算
	mesh_.transform(pose());

	// HPが0以下になると、死亡状態に移行
	
}

// 描画
void Player::draw() const
{
	mesh_.draw();							// メッシュを描画
	body_->translate(position_)->draw();	// コライダーを描画（デバッグモードのみ、調整用）

	// 以下はデバッグ表示
	// プレイヤーの状態を表示
	/*
	switch (state_)
	{
	case (PlayerState::Normal):
		DrawString(0, 0, "プレイヤーの状態：Normal", GetColor(255, 255, 255));
		break;
	case (PlayerState::Attack):
		DrawString(0, 0, "プレイヤーの状態：Attack", GetColor(255, 255, 255));
		break;
	default:
		break;
	}
	// 状態タイマーを表示
	DrawFormatString(0, 15, GetColor(255, 255, 255), "状態タイマー： %f ", state_timer_);

	if (GamePad::state(GamePad::A))
	{
		DrawString(0, 30, "Aキーが押された", GetColor(255, 255, 255));
	}
	*/
	/*
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);
	DrawFormatString(0, 0, Cr, "プレイヤーの座標： （ %f 、 %f）", position_.x, position_.y);
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
	// 状態遷移
	switch (state_)
	{
	case PlayerState::Normal:
		normal_update(delta_time);
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
void Player::normal_update(float delta_time)
{
	// Xキーが押されると、攻撃する
	if (GamePad::state(GamePad::X))
	{
		// 攻撃状態に移行

	}

	// ============================================================
	// 以下は移動処理
	int motion{ MOTION_IDLE };		// 何もしなければ、待機モーションに変更
	velocity_ = Vector3::Zero;		// 移動量をリセット
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
	// 移動量を計算し、プレイヤーを移動させる
	velocity_ += rotation_.Forward() * forward_speed;
	velocity_ += rotation_.Left() * left_speed;
	position_ += velocity_ * delta_time;

	// 移動処理終了
	// ============================================================

}