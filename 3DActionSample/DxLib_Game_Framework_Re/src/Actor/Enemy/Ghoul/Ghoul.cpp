#include "Ghoul.h"
#include "../../../World/IWorld.h"
#include "../../../ID/EventMessage.h"
#include "../../../Field/Field.h"
#include "../../../Actor/Body/Line.h"
#include "../../ActorGroup.h"
#include "../EnemyAttack.h"
#include "../../Damage.h"
#include "../../../Sound/Sound.h"
#include "../../../ID/SourceID.h"

// クラス：グール
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

//--------------------------------------------------

const int Power = 8;					// 攻撃力
const float AttackInterval = 120.0f;	// 攻撃間隔

//--------------------------------------------------

// コンストラクタ
Ghoul::Ghoul(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Enemy(world, "Ghoul", position, angle, body),
	mesh_{ MESH_GHOUL, GhoulMotion::MOTION_IDLE },
	motion_{ GhoulMotion::MOTION_IDLE },
	state_{ GhoulState::Idle },
	state_timer_{ 0.0f },
	attack_on_{ false },
	is_following_player_{ false },
	is_moving_{ false },
	attack_interval_{ 0.0f }
{
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
	rand_.randomize();

	const int min = 1, max = 3;	// 次の状態持続時間の最小値と最大値（秒）
	ready_to_next_state(min, max);
}

// 更新
void Ghoul::update(float delta_time)
{
	// 落下処理
	velocity_ += Vector3::Down * Gravity;		// 重力加速度を計算
	position_.y += velocity_.y * delta_time;	// y軸座標を計算

	intersect_ground();		// 地面との接触処理
	intersect_wall();		// 壁との接触処理
	clamp_position();		// 座標制限

	// 敵の状態を更新
	update_state(delta_time);
	// モーションを変更
	mesh_.change_motion(motion_);
	// メッシュを更新
	mesh_.update(delta_time);
	// 行列を計算
	mesh_.transform(pose());

	// HPが0以下になると、死亡状態に移行
	if (current_hp_ <= 0)
	{
		if (state_ == GhoulState::Death) return;
		change_state(GhoulState::Death, MOTION_DEATH);
		world_->send_message(EventMessage::EnemyDead);	// 死亡メッセージを送信
		return;
	}

	// 怯み累積値が一定量を越えたら、怯み状態に移行
	if (current_wince_ >= ToWince && state_ != GhoulState::Wince)
	{
		current_wince_ = 0;
		change_state(GhoulState::Wince, MOTION_WINCE);
		return;
	}

	// 攻撃間隔カウンターを減算
	if (attack_interval_ > 0.0f)
		attack_interval_ -= delta_time;
}

// 描画
void Ghoul::draw() const
{
	mesh_.draw();	// メッシュを描画

	// コライダーを描画（デバッグモードのみ、調整用）
	body_->transform(pose())->draw();
}

// 衝突リアクション
void Ghoul::react(Actor& other)
{
	// 死亡状態では反応しない
	if (state_ == GhoulState::Death) return;

	Enemy::react(other);
}

// メッセージ処理
void Ghoul::handle_message(EventMessage message, void* param)
{
	// 死亡状態では反応しない
	if (state_ == GhoulState::Death) return;

	// プレイヤーからダメージを受ける
	if (message == EventMessage::EnemyDamage)
	{
		// メッセージからプレイヤーの攻撃力と怯み値を取得し、ダメージ計算を行う
		Damage* damage = (Damage*)param;
		if (state_ != GhoulState::Death)
			current_hp_ -= damage->power;
		if (state_ != GhoulState::Wince)
			current_wince_ += damage->impact;
	}

	// 即死メッセージを受ける
	if (message == EventMessage::EnemyDestroy)
	{
		if (state_ != GhoulState::Death)
			current_hp_ = 0;
	}
}

// 状態の更新
void Ghoul::update_state(float delta_time)
{
	// 現在の状態に応じて、敵を更新
	switch (state_)
	{
	case(GhoulState::Idle):
		idle(delta_time);
		break;
	case(GhoulState::Move):
		move(delta_time);
		break;
	case(GhoulState::Attack):
		attack(delta_time);
		break;
	case(GhoulState::Wince):
		wince(delta_time);
		break;
	case(GhoulState::Death):
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// 状態タイマーを加算
}

// 状態の変更
void Ghoul::change_state(GhoulState state, int motion)
{
	previous_state_ = state_;

	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	attack_on_ = false;
	interval_ = 0.0f;
}

// 待機状態での更新
void Ghoul::idle(float delta_time)
{
	// 一定時間後、次の行動を抽選（待機状態の維持時間は乱数で決める）
	if (state_timer_ >= state_time_)
		next_action();
}

// 移動状態での更新
void Ghoul::move(float delta_time)
{
	// ============================================================
	// 以下は移動中の処理

	// プレイヤーを追従していれば、常時目的地の座標を更新
	if (is_following_player_)	next_destination_ = get_player_position();

	// 目的地の方向に向いてから、目的地へ移動
	// 回転処理（プレイヤーに追従している場合、移動完了後でもプレイヤーに向けるようになっている）
	motion_ = MOTION_IDLE;
	float angle_to_target = get_angle_to_target(next_destination_);

	if (angle_to_target >= 0.5f)
	{
		motion_ = MOTION_TURN_RIGHT;
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * delta_time);
	}
	else if (angle_to_target <= -0.5f)
	{
		motion_ = MOTION_TURN_LEFT;
		rotation_ *= Matrix::CreateRotationY(-RotateSpeed * delta_time);
	}

	// プレイヤーとの角度差が大きい場合、回転モーションを再生し、回転してから移動する
	if (angle_to_target >= 135.0f)
	{
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * 2 * delta_time);
	}
	else if (angle_to_target <= -135.0f)
	{
		rotation_ *= Matrix::CreateRotationY(RotateSpeed * 2 * delta_time);
	}

	rotation_ = Matrix::NormalizeRotationMatrix(rotation_);		// 回転行列を初期化

	if (is_moving_ && get_unsigned_angle_to_target(next_destination_) <= 18.0f)
	{
		motion_ = MOTION_WALK;

		// 移動処理
		velocity_ = Vector3::Zero;						// 移動量をリセット
		velocity_ += rotation_.Forward() * WalkSpeed;	// 移動速度を加算
		position_ += velocity_ * delta_time;			// 次の位置を計算
	}

	if (is_moving_)
	{
		// 目的地に着くと、移動完了
		if (can_attack_player() || Vector3::Distance(position_, next_destination_) <= 12.0f)
		{
			interval_ = state_timer_ + 12.0f;	// 次の行動は0.2秒後に実行
			is_moving_ = false;					// 移動完了
		}

		// 行動の維持時間を超えたら、次の行動を抽選
		if (move_timer_ >= state_time_)
		{
			next_action();
		}

		move_timer_ += delta_time;
	}

	// 移動中の処理終了
	// ============================================================

	// ============================================================
	// 以下は移動完了後の処理

	if (!is_moving_)
	{
		// 次の行動を実行
		if (state_timer_ >= interval_)
		{
			// プレイヤーは近くにいる場合、攻撃行動に移行
			if (can_attack_player())
			{
				attack_interval_ = AttackInterval;
				change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
			}
			else
			{
				next_action();	// 次の行動を抽選
			}
		}
	}

	// 移動完了後の処理終了
	// ============================================================
}

// 怯み状態での更新
void Ghoul::wince(float delta_time)
{
	// モーション終了後、次の行動を抽選
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		next_action();
	}
}

// 攻撃状態での更新
void Ghoul::attack(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= 9.0f && !attack_on_)
	{
		attack_on_ = true;

		float distance = 8.0f;					// 攻撃判定の発生距離（前方からどれぐらい）
		float height = 12.5f;					// 攻撃判定の高さ
		Vector3 attack_position = position_ + pose().Forward() * distance + Vector3(0.0f, height, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, Power));
		Sound::play_se(SE_ENEMY_ATK_LIGHT);		// SEを再生
		interval_ = state_timer_ + 40.0f;
	}

	// モーション終了後、次の行動を抽選
	if (state_timer_ >= 26.0f)
	{
		motion_ = MOTION_IDLE;
		if (state_timer_ >= interval_)	next_action();
	}
}

// 死亡状態での更新
void Ghoul::death(float delta_time)
{
	// モーションが終了すると、死亡判定を有効に
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		die();
	}
}

// 次の行動を決定
void Ghoul::next_action()
{
	// 乱数で次の行動を決定（最小値：0、最大値：6）
	int i = rand_.rand_int(0, 6);

	// 待機、移動、攻撃状態への移行確率は1/6、2/3、1/6になっている
	// 待機状態への移行
	if (i == 0)
	{
		// 待機行動は連続に発生しない
		if (previous_state_ == GhoulState::Idle)
		{
			next_action();
			return;
		}

		const int min = 2, max = 4;		// 次の状態持続時間の最小値と最大値（秒）
		ready_to_next_state(min, max);
		change_state(GhoulState::Idle, GhoulMotion::MOTION_IDLE);
		return;
	}
	// 移動状態への移行
	else if (i <= 5)
	{
		next_destination();			// 次の目的地を決定
		// 移動状態は4～8秒間維持
		const int min = 4, max = 8;		// 次の状態持続時間の最小値と最大値（秒）
		ready_to_next_state(min, max);

		move_timer_ = 0.0f;			// 移動状態タイマーをリセット
		is_moving_ = true;
		change_state(GhoulState::Move, GhoulMotion::MOTION_WALK);
		return;
	}
	// 攻撃状態への移行
	else
	{
		attack_interval_ = AttackInterval;
		change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
		return;
	}
}

// 次の目的地を決定
void Ghoul::next_destination()
{
	int i = rand_.rand_int(0, 2);

	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");
	// フィールドの参照を取得
	auto& field = world_->field();
	// フィールドの最大と最小座標を取得
	auto max_pos = field.max_position();
	auto min_pos = field.min_position();

	// i = 0、またはプレイヤーは存在しない場合、ランダムで座標を生成する
	if (i == 0 || player == nullptr)
	{
		// プレイヤー追従を解除
		is_following_player_ = false;

		// x軸の座標を生成;
		float x = rand_.rand_float(min_pos.x, max_pos.x);
		// z軸の座標を生成
		float z = rand_.rand_float(min_pos.z, max_pos.z);
		// 生成した座標を目的地にする
		Vector3 new_dest = Vector3(x, 0.0f, z);
		next_destination_ = new_dest;
	}
	else
	{
		// プレイヤー追従開始
		is_following_player_ = true;

		// プレイヤーの位置を取得し、そこを目的地にする
		next_destination_ = get_player_position();
	}
}

// プレイヤーを攻撃できるか
bool Ghoul::can_attack_player()
{
	// プレイヤーが存在しない場合、falseを返す
	if (!player_exists()) return false;
	// プレイヤーが前にいなければ、、Falseを返す
	if (!player_in_forward()) return false;
	// プレイヤーが攻撃できる距離内にいなければ、Falseを返す
	if (!player_in_range_distance(Range)) return false;
	// プレイヤーが攻撃できる角度内にいなければ、Falseを返す
	if (!player_in_range_angle(Angle)) return false;
	// 攻撃間隔が0より大きい場合、Falseを返す
	if (attack_interval_ > 0.0f) return false;

	// 条件を全て満たしていれば、Trueを返す
	return true;
}

// 次の状態への移行準備
void Ghoul::ready_to_next_state(int min, int max)
{
	// 乱数で次の状態持続時間を決める
	int i = rand_.rand_int(min, max);
	state_time_ = 60 * (float)i;
}