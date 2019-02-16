#include "PlayerAttack.h"

// クラス：プレイヤーの攻撃判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
PlayerAttack::PlayerAttack(IWorld* world, const Vector3& position, int power, int impact, const IBodyPtr& body) :
	Actor(world, "PlayerAttack", position, body),
	destroy_counter_{ 0.0f }
{
	power_ = power;		// 威力を設定
	impact_ = impact;	// 怯み値を設定
}

// 更新
void PlayerAttack::update(float delta_time)
{
	// 0.2秒後消滅
	if (destroy_counter_ >= 12.0f)
	{
		die();
	}

	destroy_counter_ += delta_time;		// 消滅カウンターを加算
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

// 威力の取得
int PlayerAttack::power() const
{
	return power_;
}

// 怯み値の取得
int PlayerAttack::impact() const
{
	return impact_;
}