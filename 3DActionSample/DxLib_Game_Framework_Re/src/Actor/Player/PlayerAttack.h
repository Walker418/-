#ifndef PLAYER_ATTACK_H_
#define PLAYER_ATTACK_H_

#include "../Actor.h"
#include "../Body/IBodyPtr.h"
#include "../Body/BoundingSphere.h"
#include "../AttackParameter.h"

// クラス：プレイヤーの攻撃の当たり判定
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class PlayerAttack : public Actor
{
public:
	// コンストラクタ
	PlayerAttack(IWorld* world, const Vector3& position, int power = 0, int impact = 0, float hit_stop = 0.0f, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3::Zero, 9.0f));
	// コンストラクタ（攻撃パラメータ構造体を使用）
	PlayerAttack(IWorld* world, AttackParameter parameter, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3::Zero, 9.0f));
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;

private:
	float destroy_counter_{ 0.0f };	// 消滅カウンター
	int power_{ 0 };				// 威力
	int impact_{ 0 };				// 怯み値
	float hit_stop_{ 0.0f };		// ヒットストップ
};

#endif // !PLAYER_ATTACK_H_