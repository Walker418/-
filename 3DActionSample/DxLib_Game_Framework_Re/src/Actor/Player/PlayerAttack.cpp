#include "PlayerAttack.h"
#include "../../ID/EventMessage.h"
#include "../Damage.h"
#include "../../World/IWorld.h"

// クラス：プレイヤーの攻撃の当たり判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
PlayerAttack::PlayerAttack(IWorld* world, const Vector3& position, int power, int impact, float hit_stop, const IBodyPtr& body) :
	Actor(world, "PlayerAttack", position, body),
	destroy_counter_{ 0.0f }
{
	power_ = power;			// 威力を設定
	impact_ = impact;		// 怯み値を設定
	hit_stop_ = hit_stop;	// ヒットストップを設定
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
	// 敵へのダメージ構造体を生成
	Damage damage{ position_, power_, impact_ };
	// 敵へダメージメッセージを送る
	other.handle_message(EventMessage::EnemyDamage, &damage);
	// プレイヤーにヒットストップメッセージを送る
	world_->send_message(EventMessage::HitStop, &hit_stop_);
	// 敵に当たると消滅
	die();
}