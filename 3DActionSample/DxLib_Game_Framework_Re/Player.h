#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "BoundingCapsule.h"
#include "SourceID.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Player : public Actor
{
private:
	// 列挙型：プレイヤーの状態
	enum class PlayerState
	{
		Normal,		// 通常
		Attack,		// 攻撃
		Guard,		// ガード
		Damage,		// 被弾
		Dead,		// 死亡
	};

	// 列挙型：プレイヤーのモーション
	enum PlayerMotion
	{
		MOTION_IDLE = 28,	// 静止（通常待機）
	};

public:
	// コンストラクタ
	Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3{ 0.0f, 9.2f, 0.0f }, Matrix::Identity, 2.2f, 14.5f));
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
	void change_state(PlayerState state, int motion);
	// 通常状態での更新
	void normal_update(float delta_time);
	// 攻撃状態での更新
	void attack_update(float delta_time);
	// ガード状態での更新
	void guard_update(float delta_time);
	// 被弾状態での更新
	void damage_update(float delta_time);
	// 死亡状態での更新
	void dead_update(float delta_time);

private:
	// アニメーションメッシュ
	AnimatedMesh	mesh_{ MESH_PALADIN };
	// モーション番号
	int				motion_{ PlayerMotion::MOTION_IDLE };
	// プレーヤーの状態
	PlayerState		state_{ PlayerState::Normal };
	// 状態タイマー
	float			state_timer_{ 0.0f };
};

#endif // !PLAYER_H_