#include "Enemy.h"
#include "../../World/IWorld.h"
#include "../../Field/Field.h"
#include "../Body/Line.h"
#include "../Body/BoundingSphere.h"
#include "../ActorGroup.h"
#include "../../Math/MathHelper.h"

// コンストラクタ
Enemy::Enemy(IWorld* world, const std::string& name, const Vector3& position, float angle, const IBodyPtr& body) :
	Actor(world, name, position, body),
	interval_{ 0.0f }
{
	rotation_ = Matrix::CreateRotationY(angle);
}

// 地面との接触処理
void Enemy::intersect_ground()
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
void Enemy::intersect_wall()
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

// 座標の制限
void Enemy::clamp_position()
{
	// フィールドを取得
	auto& field = world_->field();
	// フィールドの最大と最小座標を取得
	auto max_pos = field.max_position();
	auto min_pos = field.min_position();

	position_.x = MathHelper::clamp(position_.x, min_pos.x, max_pos.x);
	position_.z = MathHelper::clamp(position_.z, min_pos.z, max_pos.z);
}

// プレイヤーを取得
ActorPtr Enemy::get_player()
{
	return world_->find_actor(ActorGroup::Player, "Player");
}

// プレイヤーの位置を取得
Vector3 Enemy::get_player_position()
{
	// プレイヤーが存在しない場合、現在自分の座標を返す
	if (get_player() == nullptr) return position_;

	// プレイヤーが存在する場合、その座標を返す（y成分は無視する）
	auto pos = get_player()->position();
	pos.y = 0.0f;

	return pos;
}

// 目標への角度を取得（符号付き）
float Enemy::get_angle_to_target(Vector3 target) const
{
	// 目的地方向へのベクトル
	Vector3 to_target = target - position_;
	// 前方向とターゲットの外積
	Vector3 forward_cross_target = Vector3::Cross(rotation_.Forward(), to_target);

	// 外積で目標への角度を計算し、角度の値を返す
	float angle = (forward_cross_target.y >= 0.0f) ? Vector3::Angle(pose().Forward(), to_target) : -Vector3::Angle(pose().Forward(), to_target);

	return angle;
}

// 目標への角度を取得（符号無し）
float Enemy::get_unsigned_angle_to_target(Vector3 target) const
{
	return std::abs(get_angle_to_target(target));
}

// プレイヤーは存在するか
bool Enemy::player_exists()
{
	return get_player() != nullptr;
}

// プレイヤーは前にいるか
bool Enemy::player_in_forward()
{
	// プレイヤーが存在しない場合、Falseを返す
	if (get_player() == nullptr) return false;

	// プレイヤーへの角度は90°以内かどうかを返す
	return (get_unsigned_angle_to_target(get_player_position()) <= 90.0f);
}

// プレイヤーは攻撃距離内にいるか
bool Enemy::player_in_range_distance(float distance)
{
	// プレイヤーが存在しない場合、Falseを返す
	if (get_player() == nullptr) return false;

	// 自身からプレイヤーまでの距離を求め、攻撃距離内であればTrueを返す
	return (Vector3::Distance(position_, get_player_position()) <= distance);
}

// プレイヤーは攻撃できる角度にいるか
bool Enemy::player_in_range_angle(float angle)
{
	// プレイヤーが存在しない場合、Falseを返す
	if (get_player() == nullptr) return false;

	// 自身からプレイヤーまでの角度を求め、攻撃角度内であればTrueを返す
	return (get_unsigned_angle_to_target(get_player_position()) <= angle);
}