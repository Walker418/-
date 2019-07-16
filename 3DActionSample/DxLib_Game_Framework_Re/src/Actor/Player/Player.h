#ifndef PLAYER_H_
#define PLAYER_H_

#include "../Actor.h"
#include "../../Graphic/AnimatedMesh.h"
#include "../Body/IBodyPtr.h"
#include "../Body/BoundingCapsule.h"
#include "../../ID/SourceID.h"
#include "../../Math/Timer.h"
#include "../../Math/CountdownTimer.h"
#include "PlayerParameter.h"

// クラス：プレイヤー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Player : public Actor
{
private:
	// 列挙型：プレイヤーの状態
	enum class PlayerState
	{
		Normal,			// 通常
		Slash1,			// 攻撃（1段目）
		Slash2,			// 攻撃（2段目）
		Slash3,			// 攻撃（3段目）
		JumpAttack1,	// ジャンプ攻撃（1段目）
		JumpAttack2,	// ジャンプ攻撃（2段目）
		Damage,			// 怯み
		Guard,			// ガード
		Blocking,		// ガードによるノックバック
		GuardAttack,	// ガード攻撃
		GuardEnd,		// ガード終了
		Death,			// 死亡
		ForwardEvasion,	// 前回避
		LeftEvasion,	// 左回避
		RightEvasion,	// 右回避
	};

	// 列挙型：プレイヤーのモーション
	enum PlayerMotion
	{
		MOTION_IDLE = 16,				// 静止待機
		MOTION_WALK = 25,				// 前方へ歩行
		MOTION_DASH = 9,				// 前方へダッシュ
		MOTION_JUMP = 18,				// ジャンプ
		MOTION_SLASH_1 = 20,			// 攻撃（1段目）
		MOTION_SLASH_2 = 21,			// 攻撃（2段目）
		MOTION_SLASH_3 = 22,			// 攻撃（3段目）
		MOTION_JUMP_ATTACK = 19,		// ジャンプ攻撃
		MOTION_IMPACT = 17,				// 怯み
		MOTION_GUARD_BEGIN = 11,		// ガード開始
		MOTION_GUARD_IDLE = 14,			// ガード中
		MOTION_GUARD_BLOCK = 12,		// ガードによるノックバック
		MOTION_GUARD_SLASH = 15,		// ガード攻撃
		MOTION_GUARD_END = 13,			// ガード終了
		MOTION_CROUCH_BEGIN = 0,		// しゃがみ開始
		MOTION_CROUCH_IDLE = 6,			// しゃがみ中待機
		MOITON_CROUCH_IMPACT = 7,		// しゃがみ中被弾
		MOTION_CROUCH_ATTACK = 8,		// しゃがみ中攻撃
		MOTION_CROUCH_GUARD_BEGIN = 2,	// しゃがみガード開始
		MOTION_CROUCH_GUARD_IDLE = 5,	// しゃがみガード中
		MOTION_CROUCH_GUARD_BLOCK = 3,	// しゃがみガードによるノックバック
		MOTION_CROUCH_GUARD_ATTACK = 8,	// しゃがみガード攻撃（しゃがみ中攻撃と同じ）
		MOTION_CROUCH_GUARD_END = 4,	// しゃがみガード終了
		MOTION_CROUCH_END = 1,			// しゃがみ終了
		MOTION_DEATH = 10,				// 死亡
		MOTION_STRAFE_LEFT = 23,		// 左ダッシュ
		MOTION_STRAFE_RIGHT = 24,		// 右ダッシュ
	};

public:
	// コンストラクタ
	Player(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingCapsule>(Vector3(0.0f, 8.5f, 0.0f), Matrix::Identity, 11.0f, 2.5f));
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// 衝突リアクション
	virtual void react(Actor& other) override;
	// メッセージ処理
	virtual void handle_message(EventMessage message, void* param = nullptr) override;
	// 体力の取得
	virtual int get_HP() override;
	// 移動量の取得
	virtual Vector3 get_velocity() override;

private:
	// 状態の更新
	void update_state(float delta_time);
	// 状態の変更
	void change_state(PlayerState state, int motion);

	// 通常状態での更新
	void normal(float delta_time);
	// 攻撃（1段目）での更新
	void slash1(float delta_time);
	// 攻撃（2段目）での更新
	void slash2(float delta_time);
	// 攻撃（3段目）での更新
	void slash3(float delta_time);
	// ジャンプ攻撃（1段目）での更新
	void jump_attack1(float delta_time);
	// ジャンプ攻撃（2段目）での更新
	void jump_attack2(float delta_time);
	// 被弾状態での更新
	void damage(float delta_time);
	// ガード時の更新
	void guard(float delta_time);
	// ガードによるノックバック中の更新
	void blocking(float delta_time);
	// ガード攻撃での更新
	void guard_attack(float delta_time);
	// ガード終了時の更新
	void guard_end(float delta_time);
	// 死亡状態での更新
	void death(float delta_time);
	// 前回避状態での更新
	void forward_evasion(float delta_time);
	// 左回避状態での更新
	void left_evasion(float delta_time);
	// 右回避状態での更新
	void right_evasion(float delta_time);
	// 攻撃後の回避行動移行
	void attack_to_evasion();

	// 地面との接触処理
	void intersect_ground();
	// 壁との接触処理
	void intersect_wall();
	// 座標制限
	void clamp_position();
	// 回避準備
	void ready_for_evasion();
	// ヒットストップタイマーのリセット
	void reset_hit_stop(float time);

	// 自動回復
	void recovery(float delta_time);

	// ガードは成立するか
	bool can_block(Vector3 atk_pos) const;
	// 無敵時間内であるか
	bool is_invincible() const;
	// スーパーアーマー状態であるか
	bool is_super_armor() const;
	// ヒットストップ中なのか
	bool is_hit_stop() const;

private:
	// アニメーションメッシュ
	AnimatedMesh	mesh_{ MESH_PALADIN };
	// モーション番号
	int				motion_{ PlayerMotion::MOTION_IDLE };
	// プレーヤーの状態
	PlayerState		state_{ PlayerState::Normal };
	// 状態タイマー
	Timer			state_timer_;
	// 無敵時間タイマー
	CountdownTimer	invincible_timer_{ PlayerParameter::InvincibleTime };
	// 回避タイマー
	CountdownTimer	evasion_timer_{ PlayerParameter::EvasionTime };
	// ヒットストップタイマー
	CountdownTimer	hit_stop_timer_{ 0.0f };
	// 自動回復タイマー
	CountdownTimer	recovery_timer_{ PlayerParameter::RecoveryInterval * 60.0f };
	// 接地しているか
	bool			is_ground_{ false };
	// ガードしているか
	bool			is_guard_{ false };
	// 攻撃判定を発生したのか
	bool			attack_on_{ false };
	// ジャンプ攻撃は開始したか
	bool			jump_attack_started_{ false };
	// 現在の体力
	int				current_hp_;
	// プレイヤーは倒れたか（死亡=消滅とは別判定）
	bool			is_defeated_{ false };
};

#endif // !PLAYER_H_