#ifndef DRAGON_BOAR_H_
#define DRAGON_BOAR_H_

#include "Actor.h"
#include "AnimatedMesh.h"
#include "IBodyPtr.h"
#include "BoundingSphere.h"
#include "SourceID.h"

// クラス：敵（イノシシ）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class DragonBoar : public Actor
{
private:
	// 列挙型：敵の状態
	enum class DragonBoarState
	{
		Idle,		// 静止待機
		Move,		// 移動
		Attack,		// 攻撃
		Dash,		// 突進
		Roar,		// 咆哮
		Wince,		// 怯む
		Death,		// 死亡
	};

	// 列挙型：敵のモーション
	enum DragonBoarMotion
	{
		MOTION_IDLE = 4,	// 静止待機
		MOTION_WALK = 6,	// 歩行
		MOTION_DASH = 5,	// ダッシュ
		MOTION_BITE = 3,	// 噛みつく
		MOTION_ROAR = 2,	// 咆哮
		MOTION_WINCE = 1,	// 怯み
		MOTION_DEATH = 0,	// 死亡
	};

public:
	// コンストラクタ
	DragonBoar(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3(0.0f, 12.5f, 0.0f), 11.0f));
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
	void change_state(DragonBoarState state, int motion);

	// 待機状態での更新
	void idle(float delta_time);
	// 移動状態での更新
	void move(float delta_time);
	// 攻撃状態での更新
	void attack(float delta_time);
	// 突進状態での更新
	void dash(float delta_time);
	// 咆哮状態での更新
	void roar(float delta_time);
	// 怯み状態での更新
	void wince(float delta_time);
	// 死亡状態での更新
	void death(float delta_time);

	// プレイヤーは前にいるか
	bool player_in_front() const;
	// プレイヤーは攻撃距離内にいるか
	bool player_in_range_distance() const;
	// プレイヤーが攻撃できる角度にいるか
	bool player_in_range_angle() const;
	// プレイヤーを攻撃できるか
	bool can_attack_player() const;

	// 次の行動を決定
	void next_move();
	// プレイヤーの位置を取得
	Vector3 get_player_position() const;
	// プレイヤーへの角度を取得
	float get_angle_to_player() const;

	// 地面との接触処理
	void intersect_ground();
	// 壁との接触処理
	void intersect_wall();

private:
	// アニメーションメッシュ
	AnimatedMesh	mesh_{ MESH_DRAGONBOAR };
	// モーション番号
	int				motion_{ DragonBoarMotion::MOTION_IDLE };
	// 敵の状態
	DragonBoarState	state_{ DragonBoarState::Idle };
	// 状態タイマー
	float			state_timer_{ 0.0f };

	// 現在の体力
	int				current_hp_;
	// 怯み累積値
	int				current_wince_{ 0 };
	// 攻撃判定を発生したのか
	bool			is_attack_{ false };
	// 怒り状態であるか
	bool			is_anger_{ false };
	// 怒りカウンター
	float			anger_counter_{ 0.0f };

	// 最大体力
	const int		HP{ 100 };
	// 移動速度
	const float		WalkSpeed{ 0.15f };
	// 突進速度
	const float		DashSpeed{ 0.5f };
	// 回転速度
	const float		RotateSpeed{ 0.015f };
	// 重力
	const float		Gravity{ 0.03f };

	// 怯み耐性
	const int		ToWince{ 15 };
};

#endif // !DRAGON_BOAR_H_