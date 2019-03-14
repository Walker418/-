#ifndef GHOUL_H_
#define GHOUL_H_

#include "../Enemy.h"
#include "../../../Graphic/AnimatedMesh.h"
#include "../../Body/IBodyPtr.h"
#include "../../Body/BoundingCapsule.h"
#include "../../../ID/SourceID.h"
#include "../../../Math/Random.h"

// クラス：グール
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class Ghoul : public Enemy
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
		MOTION_TURN_LEFT = 2,
		MOTION_TURN_RIGHT = 3,
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

	// 次の行動を決定
	void next_move();
	// 次の目的地を決定
	void next_destination();

	// プレイヤーを攻撃できるか
	virtual bool can_attack_player() override;

	// 次の状態への移行準備
	void ready_to_next_state(int min, int max);

private:
	// アニメーションメッシュ
	AnimatedMesh	mesh_{ MESH_GHOUL };
	// モーション番号
	int				motion_{ GhoulMotion::MOTION_IDLE };
	// 敵の状態
	GhoulState		state_{ GhoulState::Idle };
	// 前の状態
	GhoulState		previous_state_;
	// 状態タイマー
	float			state_timer_{ 0.0f };
	// 移動状態タイマー（移動状態中、プレイヤーが攻撃範囲にいなければ加算）
	float			move_timer_{ 0.0f };
	// 乱数生成器
	Random			rand_;

	// 攻撃判定を発生したのか
	bool			attack_on_{ false };
	// 次の目的地
	Vector3			next_destination_;
	// プレイヤーに追従中なのか
	bool			is_following_player_{ false };
	// 移動中なのか
	bool			is_moving_{ false };
	// 次の状態維持時間
	float			state_time_{ 60.0f };
	// 攻撃の間隔時間
	float			attack_interval_{ 0.0f };

	// 最大体力
	const int		HP{ 40 };
	// 移動速度
	const float		WalkSpeed{ 0.28f };
	// 回転速度
	const float		RotateSpeed{ 1.0f };
	// 重力
	const float		Gravity{ 0.03f };

	// 怯み耐性
	const int		ToWince{ 10 };
	// 索敵距離
	const float		Range{ 12.0f };
	// 索敵角度
	const float		Angle{ 10.0f };
};

#endif // !GHOUL_H_