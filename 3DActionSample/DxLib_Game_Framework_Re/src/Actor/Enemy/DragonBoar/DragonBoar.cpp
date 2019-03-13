#include "DragonBoar.h"
#include "../../../World/IWorld.h"
#include "../../../ID/EventMessage.h"

// クラス：敵（イノシシ）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Enemy(world, "DragonBoar", position, angle, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_IDLE },
	motion_{ DragonBoarMotion::MOTION_IDLE },
	state_{ DragonBoarState::Idle },
	state_timer_{ 0.0f },
	attack_on_{ false }
{
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
	rand_.randomize();
}

// 更新
void DragonBoar::update(float delta_time)
{
	// 落下処理
	velocity_ += Vector3::Down * Gravity;		// 重力加速度を計算
	position_.y += velocity_.y * delta_time;	// y軸座標を計算
	// 地面との接触処理
	intersect_ground();
	// 壁との接触処理
	intersect_wall();

	// 敵の状態を更新
	update_state(delta_time);
	// モーションを変更
	mesh_.change_motion(motion_);
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を計算
	mesh_.transform(pose());

	// HPが0以下になると、死亡状態に移行
	if (current_hp_ <= 0 && state_ != DragonBoarState::Death)
	{
		world_->send_message(EventMessage::EnemyDead);	// 死亡メッセージを送信
		change_state(DragonBoarState::Death, MOTION_DEATH);

		return;
	}

	// 怯み累積値が一定量を越えたら、怯み状態に移行
	if (current_wince_ >= ToWince && state_ != DragonBoarState::Wince)
	{
		current_wince_ = 0;
		change_state(DragonBoarState::Wince, MOTION_WINCE);
		return;
	}
}

// 描画
void DragonBoar::draw() const
{
	mesh_.draw();	// メッシュを描画

	// コライダーを描画（デバッグモードのみ、調整用）
	body_->transform(pose())->draw();
	// 線分で方向を示す（デバッグモードのみ）
	unsigned int Cr;
	Cr = GetColor(255, 0, 0);
	DrawLine3D(position_, position_ + pose().Forward() * 50.0f, Cr);

	// デバッグメッセージ
	// Cr = GetColor(255, 255, 255);
	// DrawFormatString(0, 0, Cr, "プレイヤーへの角度： %f", get_angle_to_player());
	/*
	if (player_in_front())
		DrawString(0, 0, "プレイヤーは前にいる", Cr);
	else
		DrawString(0, 0, "プレイヤーは後にいる", Cr);
		*/
}

// 衝突リアクション
void DragonBoar::react(Actor& other)
{
	// 死亡状態では反応しない
	if (state_ == DragonBoarState::Death) return;
}

// メッセージ処理
void DragonBoar::handle_message(EventMessage message, void* param)
{
	// 死亡状態では反応しない
	if (state_ == DragonBoarState::Death) return;
}

// 状態の更新
void DragonBoar::update_state(float delta_time)
{
	// 現在の状態に応じて、敵を更新
	switch (state_)
	{
	case(DragonBoarState::Idle):
		idle(delta_time);
		break;
	case(DragonBoarState::Wince):
		wince(delta_time);
		break;
	case(DragonBoarState::Death):
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// 状態タイマーを加算
}

// 状態の変更
void DragonBoar::change_state(DragonBoarState state, int motion)
{
	previous_state_ = state_;

	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
}

// 待機状態での更新
void DragonBoar::idle(float delta_time)
{

}

// 怯み状態での更新
void DragonBoar::wince(float delta_time)
{

}

// 死亡状態での更新
void DragonBoar::death(float delta_time)
{

}

// プレイヤーを攻撃できるか
bool DragonBoar::can_attack_player()
{
	// プレイヤーが存在しない場合、falseを返す
	if (!player_exists()) return false;
	// プレイヤーが前にいなければ、、Falseを返す
	if (!player_in_forward()) return false;
	// プレイヤーが攻撃できる距離内にいなければ、Falseを返す
	if (!player_in_range_distance(Range)) return false;
	// プレイヤーが攻撃できる角度内にいなければ、Falseを返す
	if (!player_in_range_angle(Angle)) return false;

	// 条件を全て満たしていれば、Trueを返す
	return true;
}