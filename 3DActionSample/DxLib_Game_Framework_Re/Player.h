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
	};

	// 列挙型：プレイヤーのモーション
	enum PlayerMotion
	{
		MOTION_IDLE = 16,				// 静止待機
		MOTION_WALK = 23,				// 前方へ歩行
		MOTION_DASH = 9,				// 前方へダッシュ
		MOTION_JUMP = 18,				// ジャンプ
		MOTION_SLASH_1 = 20,			// 攻撃（1段目）
		MOTION_SLASH_2 = 21,			// 攻撃（2段目）
		MOTION_SLASH_3 = 22,			// 攻撃（3段目）
		MOTION_JUMP_ATTACK = 19,		// ジャンプ攻撃
		MOTION_IMPACT = 17,				// 被弾
		MOTION_GUARD_BEGIN = 11,		// ガード開始
		MOTION_GUARD_IDLE = 13,			// ガード中
		MOTION_GUARD_BLOCK = 14,		// ガード成功
		MOTION_GUARD_SLASH = 15,		// ガード攻撃
		MOTION_GUARD_END = 12,			// ガード終了
		MOTION_CROUCH_BEGIN = 0,		// しゃがみ開始
		MOTION_CROUCH_IDLE = 6,			// しゃがみ中待機
		MOITON_CROUCH_IMPACT = 7,		// しゃがみ中被弾
		MOTION_CROUCH_ATTACK = 8,		// しゃがみ中攻撃
		MOTION_CROUCH_GUARD_BEGIN = 2,	// しゃがみガード開始
		MOTION_CROUCH_GUARD_IDLE = 4,	// しゃがみガード中
		MOTION_CROUCH_GUARD_BLOCK = 5,	// しゃがみガード成功
		MOTION_CROUCH_GUARD_ATTACK = 8,	// しゃがみガード攻撃（しゃがみ中攻撃と同じ）
		MOTION_CROUCH_GUARD_END = 3,	// しゃがみガード終了
		MOTION_CROUCH_END = 1,			// しゃがみ終了
		MOTION_DEATH = 10,				// 死亡
	};

public:
	// コンストラクタ
	Player(IWorld* world, const Vector3& position, const Matrix& rotation, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3::Zero, Matrix::Identity, 2.2f, 1.5f));
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

private:
	// アニメーションメッシュ
	AnimatedMesh	mesh_{ MESH_PALADIN };
	// モーション番号
	int				motion_{ PlayerMotion::MOTION_IDLE };
	// プレーヤーの状態
	PlayerState		state_{ PlayerState::Normal };
	// 状態タイマー
	float			state_timer_{ 0.0f };

	// 現在の体力
	int				current_hp_{ HP };

	// 最大体力
	const int		HP{ 100 };
	// 移動速度
	const float		WalkSpeed{ 0.28f };
	// 重力
	const float		Gravity{ -0.5f };
};

#endif // !PLAYER_H_