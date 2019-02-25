#include "Ghoul.h"
#include "IWorld.h"
#include "EventMessage.h"
#include "Field.h"
#include "Line.h"
#include "ActorGroup.h"
#include "Random.h"
#include "EnemyAttack.h"
#include "Damage.h"

// クラス：グール
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Ghoul::Ghoul(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "Ghoul", position, body),
	mesh_{ MESH_GHOUL, GhoulMotion::MOTION_IDLE },
	motion_{ GhoulMotion::MOTION_IDLE },
	state_{ GhoulState::Idle },
	state_timer_{ 0.0f },
	is_attack_{ false },
	is_following_player_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
	previous_state_ = state_;
	next_destination_ = Vector3::Zero;
}

// 更新
void Ghoul::update(float delta_time)
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
	if (current_hp_ <= 0 && state_ != GhoulState::Death)
	{
		change_state(GhoulState::Death, MOTION_DEATH);

		return;
	}

	// 怯み累積値が一定量を越えたら、怯み状態に移行
	if (current_wince_ >= ToWince && state_ != GhoulState::Wince)
	{
		change_state(GhoulState::Wince, MOTION_WINCE);

		return;
	}
}

// 描画
void Ghoul::draw() const
{
	mesh_.draw();	// メッシュを描画

	// コライダーを描画（デバッグモードのみ、調整用）
	body_->transform(pose())->draw();
	// 線分で方向を示す（デバッグモードのみ）
	unsigned int Cr;
	Cr = GetColor(255, 0, 0);

	DrawLine3D(position_, position_ + pose().Forward() * 25.0f, Cr);
}

// 衝突リアクション
void Ghoul::react(Actor& other)
{
	// 死亡状態では反応しない
	if (state_ == GhoulState::Death) return;
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
		current_hp_ -= damage->power;
		current_wince_ += damage->impact;

		return;
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

	is_attack_ = false;
}

// 待機状態での更新
void Ghoul::idle(float delta_time)
{
	// 3秒後、次の行動を抽選
	if (state_timer_ >= 180.0f)
		next_move();
}

// 移動状態での更新
void Ghoul::move(float delta_time)
{
	// プレイヤーを追従中の場合、常時目的地の座標をプレイヤーの座標にする
	if (is_following_player_)
	{
		next_destination_ = get_player_position();
	}

	// 目的地に向かって移動
	// 目的地に向く
	Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, next_destination_.Normalize(), Vector3::Up);	// 新しい方向を設定
	rotation_ = Matrix::Lerp(rotation_, new_rotation, RotateSpeed);		// 補間で方向を転換する
	rotation_.NormalizeRotationMatrix();								// 回転行列を正規化

	velocity_ = Vector3::Zero;						// 移動量をリセット
	velocity_ += rotation_.Forward() * WalkSpeed;	// 移動速度を加算
	position_ += velocity_ * delta_time;			// 次の位置を計算

	// プレイヤーが攻撃範囲内にいれば、攻撃する
	if (can_attack_player())
	{
		change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
		return;
	}

	// 3秒後、次の行動を抽選
	if (state_timer_ >= 180.0f)
		next_move();
}

// 怯み状態での更新
void Ghoul::wince(float delta_time)
{
	// モーション終了後、次の行動を抽選
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		current_wince_ = 0;
		// change_state(GhoulState::Move, MOTION_IDLE);
		next_move();
	}
}

// 攻撃状態での更新
void Ghoul::attack(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= mesh_.motion_end_time() && !is_attack_)
	{
		is_attack_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 15.0f + Vector3(0.0f, 12.5f, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, 8));
	}

	// モーション終了後、次の行動を抽選
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		// move(delta_time);
		next_move();
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

// 地面との接触処理
void Ghoul::intersect_ground()
{
	// フィールドを取得
	auto& field = world_->field();
	// 地面との接触点
	Vector3 intersect;
	// 接触判定用線分
	Line line = Line(position_ + Vector3(0.0f, 2.5f, 0.0f), position_ - Vector3(0.0f, 1.0f, 0.0f));

	// 地面との接触点を取得
	if (field.collide_line(line.start, line.end, &intersect))
	{
		// 接地した場合、y軸座標を補正する（地面にめり込まない）
		if (intersect.y >= position_.y)
		{
			velocity_.y = 0;			// y軸移動量を0にする
			position_.y = intersect.y;	// y軸位置を補正
		}
	}
}

// 壁との接触処理
void Ghoul::intersect_wall()
{
	// フィールドを取得
	auto& field = world_->field();
	// 壁との接触点
	Vector3 intersect;
	// 接触判定用球体
	BoundingSphere sphere = BoundingSphere(position_ + Vector3(0.0f, 2.5f, 0.0f), 2.5f);

	// 壁との接触点を取得
	if (field.collide_sphere(sphere.position(), sphere.radius(), &intersect))
	{
		// プレイヤーの座標を補正
		position_.x = intersect.x;
		position_.z = intersect.z;
	}
}

// 次の行動を決定
void Ghoul::next_move()
{
	Random rand = Random();
	rand.randomize();

	// 乱数で次の行動を決定
	int i = rand.rand_int(0, 4);

	switch (i)
	{
	case 0:
		// 待機行動は連続に発生しない
		if (previous_state_ == GhoulState::Idle)
		{
			next_move();
			return;
		}
		change_state(GhoulState::Idle, GhoulMotion::MOTION_IDLE);
		break;
	case 1:
		next_destination();		// 次の目的地を決定
		change_state(GhoulState::Move, GhoulMotion::MOTION_WALK);
		break;
	case 2:
		next_destination();		// 次の目的地を決定
		change_state(GhoulState::Move, GhoulMotion::MOTION_WALK);
		break;
	case 3:
		change_state(GhoulState::Attack, GhoulMotion::MOTION_ATTACK);
		break;
	default:
		next_move();
		break;
	}
}

// 次の目的地を決定
void Ghoul::next_destination()
{
	Random rand = Random();
	rand.randomize();

	int i = rand.rand_int(0, 3);

	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// i = 1、またはプレイヤーは存在しない場合、ランダムで座標を生成する
	if (i == 1 || player == nullptr)
	{
		// プレイヤー追従を解除
		is_following_player_ = false;

		// x軸の座標を生成
		rand.randomize();
		float x = rand.rand_float(-100, 101);
		// z軸の座標を生成
		rand.randomize();
		float z = rand.rand_float(-100, 101);
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

// プレイヤーの位置を取得
Vector3 Ghoul::get_player_position() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、現在の座標を返す
	if (player == nullptr) return position_;

	// プレイヤーが存在する場合、その座標を返す
	return player->position();
}

// プレイヤーへの角度を取得
float Ghoul::get_angle_to_player() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、0°を返す
	if (player == nullptr) return 0.0f;

	return Vector3::Angle(position_ + pose().Forward(), player->position());
}

// プレイヤーは存在するか
bool Ghoul::player_exists() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在するかどうかを返す
	return (player != nullptr);
}

// プレイヤーは前にいるか
bool Ghoul::player_in_forward() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、falseを返す
	if (player == nullptr) return false;

	// 内積でプレイヤーがいる方向を判定
	Vector3 to_target = player->position() - position_;								// プレイヤー方向のベクトル
	float forward_dot_target = Vector3::Dot(rotation_.Forward(), to_target);		// 前方向とプレイヤーの内積

	// 前方向とプレイヤーの内積が0以上であれば、Trueを返す
	return (forward_dot_target >= 0.0f);
}

// プレイヤーは攻撃距離内にいるか
bool Ghoul::player_in_range_distance() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、falseを返す
	if (player == nullptr) return false;

	// 自身からプレイヤーまでの距離を求め、攻撃距離内であればTrueを返す
	return (Vector3::Distance(position_, get_player_position()) <= 25.0f);
}

// プレイヤーは攻撃できる角度にいるか
bool Ghoul::player_in_range_angle() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、falseを返す
	if (player == nullptr) return false;

	// 自身からプレイヤーまでの角度を求め、攻撃角度内であればTrueを返す
	return (get_angle_to_player() <= 10.0f);
}

// プレイヤーを攻撃できるか
bool Ghoul::can_attack_player() const
{
	// プレイヤーが存在しない場合、falseを返す
	if (!player_exists()) return false;
	// プレイヤーが前にいなければ、、Falseを返す
	if (!player_in_forward()) return false;
	// プレイヤーが攻撃できる距離内にいなければ、Falseを返す
	if (!player_in_range_distance()) return false;
	// プレイヤーが攻撃できる角度内にいなければ、Falseを返す
	if (!player_in_range_angle()) return false;

	// 条件を全て満たしていれば、Trueを返す
	return true;
}