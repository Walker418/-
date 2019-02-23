#ifndef GHOUL_H_
#define GHOUL_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "IBodyPtr.h"
#include "BoundingCapsule.h"
#include "SourceID.h"

// クラス：グール
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class Ghoul : public Actor
{
private:
	// 列挙型：敵の状態
	enum class GhoulState
	{
		Idle,	// 静止待機
		Move,	// 移動
		Attack,	// 攻撃
		Wince,	// 怯む
		Death,	// 死亡
	};

	// 列挙型：敵のモーション
	enum GhoulMotion
	{
		MOTION_IDLE = 0,
		MOTION_WALK = 1,
		MOTION_WINCE = 4,
		MOTION_ATTACK = 5,
		MOTION_DEATH = 6
	};

public:
	// コンストラクタ
	Ghoul(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3(0.0f, 9.2f, 0.0f), Matrix::Identity, 13.5f, 2.5f));
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
	// メッセージ処理
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// 状態の更新
	void update_state(float delta_time);
	// 状態の変更
	void change_state(GhoulState state, int motion);

	// 待機状態での更新
	void idle(float delta_time);
	// 移動状態での更新
	void move(float delta_time);
	// 怯み状態での更新
	void wince(float delta_time);
	// 攻撃状態での更新
	void attack(float delta_time);
	// 死亡状態での更新
	void death(float delta_time);

	// 地面との接触処理
	void intersect_ground();
	// 壁との接触処理
	void intersect_wall();

private:
	// アニメーションメッシュ
	AnimatedMesh	mesh_{ MESH_GHOUL };
	// モーション番号
	int				motion_{ GhoulMotion::MOTION_IDLE };
	// 敵の状態
	GhoulState		state_{ GhoulState::Idle };
	// 状態タイマー
	float			state_timer_{ 0.0f };

	// 現在の体力
	int				current_hp_;
	// 怯み累積値
	int				current_wince_{ 0 };
	// 攻撃判定を発生したのか
	bool			is_attack_{ false };

	// 最大体力
	const int		HP{ 20 };
	// 移動速度
	const float		WalkSpeed{ 0.15f };
	// 回転速度
	const float		RotateSpeed{ 0.015f };
	// 重力
	const float		Gravity{ 0.03f };

	// 怯み耐性
	const int		ToWince{ 5 };
};

#endif // !GHOUL_H_