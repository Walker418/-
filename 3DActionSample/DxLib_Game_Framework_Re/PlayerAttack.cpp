#include "PlayerAttack.h"

// クラス：プレイヤーの攻撃判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
PlayerAttack::PlayerAttack(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body) :
	Actor(world, "PlayerAttack", position, body),
	destroy_counter_{ 0.0f }
{
	rotation_ = rotation;
}

// 更新
void PlayerAttack::update(float delta_time)
{
	if (destroy_counter_ >= 30.0f)
	{
		die();
	}

	destroy_counter_ += delta_time;
}

// 描画
void PlayerAttack::draw() const
{
	body_->transform(pose())->draw();	// コライダーを描画（デバッグモードのみ、調整用）
}

// 衝突リアクション
void PlayerAttack::react(Actor& other)
{

}