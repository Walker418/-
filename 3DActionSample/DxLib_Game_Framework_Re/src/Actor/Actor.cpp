#include "Actor.h"

// クラス：アクター
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
Actor::Actor() :
	world_{ nullptr }, name_{ "null" }, position_{ Vector3::Zero }, body_{ std::shared_ptr<DummyBody>() }
{ }

// コンストラクタ
Actor::Actor(IWorld* world, const std::string& name, const Vector3& position, const IBodyPtr& body) :
	world_{ world }, name_{ name }, position_{ position }, body_{ body }
{ }

// 仮想デストラクタ
Actor::~Actor()
{ }

// 更新
void Actor::update(float delta_time)
{ }

// 描画
void Actor::draw() const
{ }

// 衝突リアクション
void Actor::react(Actor& other)
{ }

// メッセージ処理
void Actor::handle_message(EventMessage message, void* param)
{ }

// 衝突判定
void Actor::collide(Actor& other)
{
	if (is_collided(other))
	{
		react(other);
		other.react(*this);
	}
}

// 死亡
void Actor::die()
{
	is_dead_ = true;
}

// 衝突しているか
bool Actor::is_collided(const Actor& other) const
{
	return body()->is_collided(*other.body());
}

// 死亡しているか
bool Actor::is_dead() const
{
	return is_dead_;
}

// 外部からの移動指令
void Actor::move_order(Vector3 vector)
{
	position_ += vector;
}

// 名前の取得
const std::string& Actor::name() const
{
	return name_;
}

// 座標の取得
Vector3 Actor::position() const
{
	return position_;
}

// 回転行列の取得
Matrix Actor::rotation() const
{
	return rotation_;
}

// 変換行列の取得
Matrix Actor::pose() const
{
	return Matrix(rotation_).Translation(position_);
}

// 衝突判定データの取得
IBodyPtr Actor::body() const
{
	return body_->transform(pose());
}

// 体力の取得
int Actor::get_HP()
{
	return 0;
}

Vector3 Actor::get_velocity()
{
	// 移動量の取得
	return Vector3();
}
