#include "Player.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Player::Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "Player", position, body),
	mesh_{ MESH_PALADIN, MOTION_IDLE },
	motion_{ MOTION_IDLE }
{
	rotation_ = rotation;
	velocity_ = Vector3::Zero;
	state_ = PlayerState::Normal;
	state_timer_ = 0.0f;
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
}

// 描画
void Player::draw() const
{
	mesh_.draw();	// メッシュを描画
	body_->draw();	// コライダーを描画（調整用）
}

// 衝突リアクション
void Player::react(Actor & other)
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
	case Player::PlayerState::Normal:
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

}

// 攻撃状態での更新
void Player::attack_update(float delta_time)
{

}

// ガード状態での更新
void Player::guard_update(float delta_time)
{

}

// 被弾状態での更新
void Player::damage_update(float delta_time)
{

}

// 死亡状態での更新
void Player::dead_update(float delta_time)
{

}