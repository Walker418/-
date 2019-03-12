#include "EnemyAttack.h"
#include "../../ID/EventMessage.h"
#include "../Damage.h"

// クラス：敵の攻撃の当たり判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
EnemyAttack::EnemyAttack(IWorld* world, const Vector3& position, int power, const IBodyPtr& body) :
	Actor(world, "EnemyAttack", position, body),
	destroy_counter_{ 0.0f }
{
	power_ = power;		// 威力を設定
}

// 更新
void EnemyAttack::update(float delta_time)
{
	// 0.2秒後消滅
	if (destroy_counter_ >= 12.0f)
	{
		die();
	}

	destroy_counter_ += delta_time;		// 消滅カウンターを加算
}

// 描画
void EnemyAttack::draw() const
{
	body_->transform(pose())->draw();	// コライダーを描画（デバッグモードのみ、調整用）
}

// 衝突リアクション
void EnemyAttack::react(Actor& other)
{
	// プレイヤーへのダメージ構造体を生成
	Damage damage = { position_, power_ };
	// プレイヤーへダメージメッセージを送る
	other.handle_message(EventMessage::PlayerDamage, &damage);
	// プレイヤーに当たると消滅
	die();
}