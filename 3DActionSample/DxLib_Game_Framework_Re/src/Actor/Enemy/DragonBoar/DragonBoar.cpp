#include "DragonBoar.h"
#include "../../../World/IWorld.h"
#include "../../../ID/EventMessage.h"
#include "../../Damage.h"
#include "../EnemyAttack.h"
#include "../../ActorGroup.h"

// クラス：敵（イノシシ）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Enemy(world, "DragonBoar", position, angle, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_ROAR },
	motion_{ DragonBoarMotion::MOTION_ROAR },
	state_{ DragonBoarState::Roar },
	state_timer_{ 0.0f },
	attack_on_{ false },
	is_moving_{ false },
	is_anger_{ false },
	anger_timer_{ 0.0f },
	dash_timer_{ 0.0f }
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
		world_->send_message(EventMessage::BossDead);	// 死亡メッセージを送信
		change_state(DragonBoarState::Death, MOTION_DEATH);
		return;
	}

	/*
	// 体力が半分以下になったら、1回咆哮して怒り状態へ移行
	if (current_hp_ <= HP / 2 && !is_anger_)
	{
		current_wince_ /= 2;
		is_anger_ = true;
		anger_timer_ = 120.0f;
		change_state(DragonBoarState::Roar, MOTION_ROAR);
		return;
	}
	*/

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

	// 怒り突進中にプレイヤーを当たったら、ダメージを与える
	if (state_ == DragonBoarState::Anger && attack_on_)
	{
		// プレイヤーへのダメージ構造体を生成
		Damage damage = { position_, 25 };
		// プレイヤーへダメージメッセージを送る
		other.handle_message(EventMessage::PlayerDamage, &damage);
		// 攻撃判定を無効化
		attack_on_ = false;
		return;
	}
}

// メッセージ処理
void DragonBoar::handle_message(EventMessage message, void* param)
{
	// 死亡状態では反応しない
	if (state_ == DragonBoarState::Death) return;

	// プレイヤーからダメージを受ける
	if (message == EventMessage::EnemyDamage)
	{
		// メッセージからプレイヤーの攻撃力と怯み値を取得し、ダメージ計算を行う
		Damage* damage = (Damage*)param;
		if (state_ != DragonBoarState::Death)
			current_hp_ -= damage->power;
		if (state_ != DragonBoarState::Wince)
			current_wince_ += damage->impact;
	}
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
	case(DragonBoarState::Roar):
		roar(delta_time);
		break;
	case(DragonBoarState::Normal):
		normal(delta_time);
		break;
	case(DragonBoarState::Bite):
		bite(delta_time);
		break;
	case(DragonBoarState::Anger):
		anger(delta_time);
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
	interval_ = 0.0f;

	if (state == DragonBoarState::Normal || state == DragonBoarState::Anger)
		is_moving_ = true;

	if (state == DragonBoarState::Anger)
	{
		dash_timer_ = 60 * rand_.rand_int(2, 4);
	}
}

// 待機状態での更新
void DragonBoar::idle(float delta_time)
{
	// 5秒後、移動開始
	if (state_timer_ >= 300.0f)
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// 怒り移動へ移行
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// 通常移動へ移行
	}

	/*
	if (state_timer_ >= 120.0f)
	{
		change_state(DragonBoarState::Bite, DragonBoarMotion::MOTION_BITE);
	}
	*/
}

// 咆哮中の更新
void DragonBoar::roar(float delta_time)
{
	// モーション終了後、移動開始
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// 怒り移動へ移行
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// 通常移動へ移行
	}
}

// 通常状態での移動
void DragonBoar::normal(float delta_time)
{
	// ============================================================
	// 以下は移動中の処理

	// 目的地（プレイヤーの座標）を更新
	next_destination_ = get_player_position();

	// 目的地の方向に向いてから、目的地へ移動
	// 回転処理（ずっとプレイヤーに向けるように）
	motion_ = MOTION_IDLE;
	float angle_to_target = get_angle_to_target(next_destination_);

	if (angle_to_target >= 0.5f)
	{
		motion_ = MOTION_WALK;
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * delta_time);
	}
	else if (angle_to_target <= -0.5f)
	{
		motion_ = MOTION_WALK;
		rotation_ *= Matrix::CreateRotationY(-RotateSpeed * delta_time);
	}
	rotation_ = Matrix::NormalizeRotationMatrix(rotation_);		// 回転行列を初期化

	// プレイヤーに向いてる場合、移動する
	if (is_moving_ && get_unsigned_angle_to_target(next_destination_) <= 18.0f)
	{
		motion_ = MOTION_WALK;

		// 移動処理
		velocity_ = Vector3::Zero;						// 移動量をリセット
		velocity_ += rotation_.Forward() * WalkSpeed;	// 移動速度を加算
		position_ += velocity_ * delta_time;			// 次の位置を計算
	}

	// プレイヤーに接近すると、噛みつく
	if (is_moving_)
	{
		if (can_attack_player())
		{
			interval_ = state_timer_ + 12.0f;	// 次の行動は0.2秒後に実行
			is_moving_ = false;					// 移動完了
		}
	}

	// 移動中の処理終了
	// ============================================================

	// ============================================================
	// 以下は移動完了後の処理

	// 攻撃状態へ移行
	if (!is_moving_ && state_timer_ >= interval_)
	{
		change_state(DragonBoarState::Bite, DragonBoarMotion::MOTION_BITE);
	}

	// 移動完了後の処理終了
	// ============================================================
}

// 噛みつき中の更新
void DragonBoar::bite(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= 40.0f && !attack_on_)
	{
		attack_on_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 40.0f + Vector3(0.0f, 12.5f, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, 15, 10));
		interval_ = state_timer_ + 40.0f;
	}

	// 次の状態へ移行
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		next_action();
		// change_state(DragonBoarState::Idle, DragonBoarMotion::MOTION_IDLE);
	}
}

// 怒り状態での移動
void DragonBoar::anger(float delta_time)
{
	motion_ = MOTION_IDLE;

	// 2秒間、プレイヤーと軸合わせ
	if (state_timer_ <= 120.0f)
	{
		next_destination_ = get_player_position();						// 目的地（プレイヤーの座標）を更新
		float angle_to_target = get_angle_to_target(next_destination_);	// プレイヤー向きの角度を求める
		// プレイヤーに向けて回転
		if (angle_to_target >= 0.5f)
		{
			motion_ = MOTION_WALK;
			rotation_ *= Matrix::CreateRotationY(RotateSpeed * 2 * delta_time);
		}
		else if (angle_to_target <= -0.5f)
		{
			motion_ = MOTION_WALK;
			rotation_ *= Matrix::CreateRotationY(-RotateSpeed * 2 * delta_time);
		}
		rotation_ = Matrix::NormalizeRotationMatrix(rotation_);			// 回転行列を初期化
		return;
	}

	// 目的地に向かって突進
	
}

// 怯み状態での更新
void DragonBoar::wince(float delta_time)
{
	// モーション終了後、次の行動を抽選
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		next_action();
	}
}

// 死亡状態での更新
void DragonBoar::death(float delta_time)
{
	// モーションが終了すると、死亡判定を有効に
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		die();
	}
}

// 次の行動を抽選
void DragonBoar::next_action()
{
	// 乱数で次の行動を決定
	int i = rand_.rand_int(0, 3);

	// 待機、移動への移行確率は1/4、3/4になっている
	// 待機状態への移行
	if (i == 0)
	{
		// 待機行動は連続に発生しない
		if (previous_state_ == DragonBoarState::Idle)
		{
			next_action();
			return;
		}
		change_state(DragonBoarState::Idle, DragonBoarMotion::MOTION_IDLE);
		return;
	}
	// 移動状態への移行
	else
	{
		(anger_timer_ > 0.0f) ?
			change_state(DragonBoarState::Anger, DragonBoarMotion::MOTION_DASH) :	// 怒り移動へ移行
			change_state(DragonBoarState::Normal, DragonBoarMotion::MOTION_WALK);	// 通常移動へ移行
	}
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