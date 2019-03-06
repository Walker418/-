#ifndef ENEMY_ATTACK_H_
#define ENEMY_ATTACK_H_

#include "Actor.h"
#include "IBodyPtr.h"
#include "BoundingSphere.h"

// クラス：敵の攻撃の当たり判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class EnemyAttack : public Actor
{
public:
	// コンストラクタ
	EnemyAttack(IWorld* world, const Vector3& position, int power = 0, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3::Zero, 6.0f));
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;

private:
	float destroy_counter_{ 0.0f };	// 消滅カウンター
	int power_{ 0 };				// 威力
};

#endif // !ENEMY_ATTACK_H_