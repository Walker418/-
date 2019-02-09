#ifndef ACTOR_H_
#define ACTOR_H_

#include <string>
#include "Vector3.h"
#include "Matrix.h"
#include "IBodyPtr.h"
#include "DummyBody.h"

// クラス：アクター
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class IWorld;
enum class EventMessage;

class Actor
{
public:
	// コンストラクタ
	Actor();
	// コンストラクタ
	Actor(IWorld* world, const std::string& name, const Vector3& position, const IBodyPtr& body = std::shared_ptr<DummyBody>());
	// 仮想デストラクタ
	virtual ~Actor();

public:
	// 更新
	virtual void update(float delta_time);
	// 描画
	virtual void draw() const;
	// 衝突リアクション
	virtual void react(Actor& other);
	// メッセージ処理
	virtual void handle_message(EventMessage message, void* param = nullptr);
	// 衝突判定
	void collide(Actor& other);
	// 死亡
	void die();
	// 衝突しているか
	bool is_collided(const Actor& other) const;
	// 死亡しているか
	bool is_dead() const;

	// 名前の取得
	const std::string& name() const;
	// 座標の取得
	Vector3 position() const;
	// 回転行列の取得
	Matrix rotation() const;
	// 変換行列の取得
	Matrix pose() const;
	// 衝突判定データの取得
	IBodyPtr body() const;

	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

protected:
	// ワールド
	IWorld*			world_{ nullptr };
	// 名前
	std::string		name_;
	// 座標
	Vector3			position_{ Vector3::Zero };
	// 回転行列
	Matrix			rotation_{ Matrix::Identity };
	// 移動量
	Vector3			velocity_{ Vector3::Zero };
	// 衝突判定データ
	IBodyPtr		body_;
	// 死亡フラグ
	bool			is_dead_{ false };
};

#endif // !ACTOR_H_