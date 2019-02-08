#ifndef PLAYER_ATTACK_H_
#define PLAYER_ATTACK_H_

#include "Actor.h"
#include "IBodyPtr.h"
#include "BoundingSphere.h"

// クラス：プレイヤーの攻撃判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class PlayerAttack : public Actor
{
public:
	// コンストラクタ
	PlayerAttack(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3::Zero, 1.5f));
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;

private:
	float destroy_counter_{ 0.0f };	// 消滅カウンター
};

#endif // !PLAYER_ATTACK_H_