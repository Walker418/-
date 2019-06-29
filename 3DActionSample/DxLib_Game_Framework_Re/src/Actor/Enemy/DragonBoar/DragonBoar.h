#ifndef DRAGON_BOAR_H_
#define DRAGON_BOAR_H_

#include "../Enemy.h"
#include "../../../Graphic/AnimatedMesh.h"
#include "../../Body/IBodyPtr.h"
#include "../../Body/BoundingSphere.h"
#include "../../../ID/SourceID.h"
#include "../../../Math/Random.h"

// クラス：敵（イノシシ）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class DragonBoar : public Enemy
{
private:
	// 列挙型：敵の状態
	enum class DragonBoarState
	{
		Idle,		// 静止待機
		Roar,		// 咆哮
		Normal,		// 通常
		Bite,		// 噛みつく
		Anger,		// 怒り
		Wince,		// 怯む
		Death,		// 死亡
	};

	// 列挙型：敵のモーション
	enum DragonBoarMotion
	{
		MOTION_IDLE = 4,	// 静止
		MOTION_ROAR = 2,	// 咆哮
		MOTION_WALK = 6,	// 歩行
		MOTION_BITE = 3,	// 噛みつく
		MOTION_DASH = 5,	// 突進
		MOTION_WINCE = 1,	// 怯む
		MOTION_DEATH = 0,	// 死亡
	};

public:
	// コンストラクタ
	DragonBoar(IWorld* world, const Vector3& position, float angle = 0.0f, const IBodyPtr& body = std::make_shared<BoundingSphere>(Vector3(0.0f, 14.0f, 0.0f), 10.5f));
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
	// 咆哮中の更新
	void roar(float delta_time);
	// 通常状態での移動
	void normal(float delta_time);
	// 噛みつき中の更新
	void bite(float delta_time);
	// 怒り状態での移動
	void anger(float delta_time);
	// 怯み状態での更新
	void wince(float delta_time);
	// 死亡状態での更新
	void death(float delta_time);

	// 次の行動を抽選
	void next_action();

	// プレイヤーを攻撃できるか
	virtual bool can_attack_player() override;

private:
	// アニメーションメッシュ
	AnimatedMesh		mesh_{ MESH_DRAGONBOAR };
	// モーション番号
	int					motion_{ DragonBoarMotion::MOTION_IDLE };
	// 敵の状態
	DragonBoarState		state_{ DragonBoarState::Idle };
	// 前の状態
	DragonBoarState		previous_state_;
	// 状態タイマー
	float				state_timer_{ 0.0f };
	// 乱数生成器
	Random				rand_;

	// 攻撃判定を発生したのか
	bool				attack_on_{ false };
	// 突進攻撃の判定を発生したのか
	bool				dash_attack_on_{ false };
	// 次の目的地
	Vector3				next_destination_;
	// 咆哮のSEを再生したか
	bool				roar_started_{ false };
	// 移動中なのか
	bool				is_moving_{ false };
	// 怒り状態なのか
	bool				is_anger_{ false };
	// 怒り状態タイマー
	float				anger_timer_{ 0.0f };
	// 怒り突進の持続時間
	float				dash_timer_{ 0.0f };

	// 最大体力
	const int			HP{ 100 };
	// 歩行速度
	const float			WalkSpeed{ 0.28f };
	// 突進速度
	const float			DashSpeed{ 0.5f };
	// 回転速度
	const float			RotateSpeed{ 0.8f };
	// 重力
	const float			Gravity{ 0.03f };

	// 怯み耐性
	const int			ToWince{ 15 };
	// 索敵距離
	const float			Range{ 45.0f };
	// 索敵角度
	const float			Angle{ 10.0f };
};

#endif // !DRAGON_BOAR_H_