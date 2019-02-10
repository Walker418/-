#include "DragonBoar.h"
#include "IWorld.h"
#include "EventMessage.h"
#include "Field.h"
#include "Line.h"
#include "ActorGroup.h"
#include "Random.h"

// クラス：敵（イノシシ）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
// コンストラクタ
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "DragonBoar", position, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_ROAR },
	motion_{ DragonBoarMotion::MOTION_ROAR },
	state_{ DragonBoarState::Roar },
	state_timer_{ 0.0f }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
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
		change_state(DragonBoarState::Death, MOTION_DEATH);
	}
}

// 描画
void DragonBoar::draw() const
{
	mesh_.draw();	// メッシュを描画

	// コライダーを描画（デバッグモードのみ、調整用）
	body_->transform(pose())->draw();

	
	// デバッグメッセージ
	unsigned int Cr;
	Cr = GetColor(255, 255, 255);

	if (can_attack_player())
	{
		DrawString(0, 0, "攻撃できる", Cr);
	}
	else
	{
		DrawString(0, 0, "攻撃できない", Cr);
	}
	
}

// 衝突リアクション
void DragonBoar::react(Actor& other)
{
	
}

// メッセージ処理
void DragonBoar::handle_message(EventMessage message, void* param)
{

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
	case(DragonBoarState::Move):
		move(delta_time);
		break;
	case(DragonBoarState::Attack):
		attack(delta_time);
		break;
	case(DragonBoarState::Dash):
		dash(delta_time);
		break;
	case(DragonBoarState::Roar):
		roar(delta_time);
		break;
	case(DragonBoarState::Damage):
		damage(delta_time);
		break;
	case(DragonBoarState::Death):
		death(delta_time);
		break;
	default:
		break;
	}

	state_timer_ += delta_time;			// 状態タイマーの加算
}

// 状態の変更
void DragonBoar::change_state(DragonBoarState state, int motion)
{
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;
}

// 待機状態での更新
void DragonBoar::idle(float delta_time)
{
	// 3秒後、次の状態へ移行
	if (state_timer_ >= 180.0f)
	{
		change_state(DragonBoarState::Roar, MOTION_ROAR);
	}
}

// 移動状態での更新
void DragonBoar::move(float delta_time)
{
	// プレイヤーが攻撃範囲内に居れば、噛みつく攻撃を行う
	if (can_attack_player())
	{
		change_state(DragonBoarState::Attack, MOTION_BITE);
	}

	// ============================================================
	// プレイヤーに向かって移動
	Vector3 next_position = get_player_position();	// プレイヤーの位置を取得
	next_position.y = 0.0f;							// プレイヤー座標の高さを無視
	float angle_to_player = get_angle_to_player();	// プレイヤー向きの角度を取得

	// プレイヤーに向ける
	Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, next_position.Normalize(), Vector3::Up);	// 新しい方向を設定
	rotation_ = Matrix::Lerp(rotation_, new_rotation, RotateSpeed);		// 補間で方向を転換する
	// 移動処理


	// 移動処理終了
	// ============================================================

	/*
	// 移動状態が8秒間維持したら、次の状態を抽選し、移行する
	if (state_timer_ >= 480.0f)
	{
		next_move();
	}
	*/
}

// 攻撃状態での更新
void DragonBoar::attack(float delta_time)
{
	// モーション終了後、移動状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		change_state(DragonBoarState::Move, MOTION_IDLE);
	}
}

// 突進状態での更新
void DragonBoar::dash(float delta_time)
{
}

// 咆哮状態での更新
void DragonBoar::roar(float delta_time)
{
	// モーション終了後、移動状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		change_state(DragonBoarState::Move, MOTION_IDLE);
	}
}

// 怯み状態での更新
void DragonBoar::damage(float delta_time)
{
	// モーション終了後、移動状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		change_state(DragonBoarState::Move, MOTION_IDLE);
	}
}

// 死亡状態での更新
void DragonBoar::death(float delta_time)
{
	// モーションが終了すると、死亡判定を有効に
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		world_->send_message(EventMessage::EnemyDead);
		die();
	}
}

// プレイヤーは攻撃距離内にいるか
bool DragonBoar::player_in_range_distance() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、falseを返す
	if (player == nullptr) return false;

	// 自身からプレイヤーまでの距離を求め、攻撃距離内であればTrueを返す
	return (Vector3::Distance(position_, get_player_position()) <= 50.0f);
}

// プレイヤーが攻撃できる角度にいるか
bool DragonBoar::player_in_range_angle() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、falseを返す
	if (player == nullptr) return false;

	// 自身からプレイヤーまでの角度を求め、攻撃角度内であればTrueを返す
	return (get_angle_to_player() <= 20.0f);
}

// プレイヤーを攻撃できるか
bool DragonBoar::can_attack_player() const
{
	// プレイヤーが攻撃できる距離内にいなければ、Falseを返す
	if (!player_in_range_distance()) return false;
	// プレイヤーが攻撃できる角度内にいなければ、Falseを返す
	if (!player_in_range_angle()) return false;

	// 条件を全て満たしていれば、Trueを返す
	return true;
}

// 次の行動を決定
void DragonBoar::next_move()
{
	// 乱数生成器で乱数を生成し、次の行動を決める
	Random rand = Random();
	rand.randomize();

	int i = rand.rand(0, 10);
	

}

// プレイヤーの位置を取得
Vector3 DragonBoar::get_player_position() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、現在の座標を返す
	if (player == nullptr) return position_;

	// プレイヤーが存在する場合、その座標を返す
	return player->position();
}

// プレイヤーへの角度を取得
float DragonBoar::get_angle_to_player() const
{
	// プレイヤーの参照を取得
	auto player = world_->find_actor(ActorGroup::Player, "Player");

	// プレイヤーが存在しない場合、0°を返す
	if (player == nullptr) return 0.0f;

	return Vector3::Angle(rotation_.Forward(), player->position());
}

// 地面との接触処理
void DragonBoar::intersect_ground()
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
void DragonBoar::intersect_wall()
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