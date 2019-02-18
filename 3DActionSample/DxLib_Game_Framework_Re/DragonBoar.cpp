#include "DragonBoar.h"
#include "IWorld.h"
#include "EventMessage.h"
#include "Field.h"
#include "Line.h"
#include "ActorGroup.h"
#include "Random.h"
#include "EnemyAttack.h"
#include "Damage.h"

// クラス：敵（イノシシ）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
DragonBoar::DragonBoar(IWorld* world, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, "DragonBoar", position, body),
	mesh_{ MESH_DRAGONBOAR, DragonBoarMotion::MOTION_ROAR },
	motion_{ DragonBoarMotion::MOTION_ROAR },
	state_{ DragonBoarState::Roar },
	state_timer_{ 0.0f },
	is_attack_{ false }
{
	rotation_ = Matrix::CreateRotationY(angle);
	velocity_ = Vector3::Zero;
	current_hp_ = HP;
	current_wince_ = 0;
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

		return;
	}

	// 怯み累積値が一定量を越えたら、怯み状態に移行
	if (current_wince_ >= ToWince && state_ != DragonBoarState::Wince)
	{
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
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;

	is_attack_ = false;
}

// 待機状態での更新
void DragonBoar::idle(float delta_time)
{
	// 待機3秒後、次の状態へ移行
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

		return;
	}

	// ============================================================
	// 以下は移動処理
	
	// 何もしていない場合、待機モーションに変更
	int motion{ MOTION_IDLE };

	Vector3 next_position = get_player_position();	// プレイヤーの位置を取得
	next_position.y = 0.0f;							// プレイヤー座標の高さを無視

	// プレイヤーに向ける
	Matrix new_rotation = Matrix::CreateWorld(Vector3::Zero, next_position.Normalize(), Vector3::Up);	// 新しい方向を設定
	rotation_ = Matrix::Lerp(rotation_, new_rotation, RotateSpeed);		// 補間で方向を転換する
	/*
	// 向いている方向へ移動
	velocity_ = Vector3::Zero;						// 移動量をリセット
	velocity_ += rotation_.Forward() * WalkSpeed;	// 移動速度を加算
	position_ += velocity_ * delta_time;			// 次の位置を計算

	// 移動していれば、移動モーションに変更
	if (velocity_.x != 0.0f || velocity_.z != 0.0f)
		motion = MOTION_WALK;
		*/
	// 状態を更新
	change_state(DragonBoarState::Move, motion);

	// 移動処理終了
	// ============================================================
}

// 攻撃状態での更新
void DragonBoar::attack(float delta_time)
{
	// 攻撃判定を発生
	if (state_timer_ >= mesh_.motion_end_time() && !is_attack_)
	{
		is_attack_ = true;
		Vector3 attack_position = position_ + pose().Forward() * 43.0f + Vector3(0.0f, 12.5f, 0.0f);
		world_->add_actor(ActorGroup::EnemyAttack, new_actor<EnemyAttack>(world_, attack_position, 20));
	}

	// モーション終了後、移動状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		move(delta_time);
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
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		move(delta_time);
	}
}

// 怯み状態での更新
void DragonBoar::wince(float delta_time)
{
	// モーション終了後、移動状態に戻る
	if (state_timer_ >= mesh_.motion_end_time() * 2.0f)
	{
		current_wince_ = 0;
		// change_state(DragonBoarState::Move, MOTION_IDLE);
		move(delta_time);
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

// プレイヤーは前にいるか
bool DragonBoar::player_in_front() const
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
	return (get_angle_to_player() <= 10.0f);
}

// プレイヤーを攻撃できるか
bool DragonBoar::can_attack_player() const
{
	// プレイヤーが前にいなければ、、Falseを返す
	if (!player_in_front()) return false;
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

	// return Vector3::Angle(rotation_.Forward(), player->position());
	return Vector3::Angle(position_ + pose().Forward(), player->position());
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