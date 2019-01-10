#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include "Actor.h"
#include "ActorPtr.h"
#include <list>
#include <functional>

// クラス：アクター管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

enum class EventMessage;

class ActorManager
{
private:
	// アクターリスト
	using ActorList = std::list<ActorPtr>;

public:
	// デフォルトコンストラクタ
	ActorManager() = default;
	// アクターの追加
	void add(const ActorPtr& actor);
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// 衝突判定
	void collide();
	// 衝突判定
	void collide(Actor& other);
	// 衝突判定
	void collide(ActorManager& other);
	// 削除
	void remove();
	// アクターの取得
	ActorPtr find(const std::string& name) const;
	// アクター数の取得
	unsigned int count() const;
	// アクターリストの巡回
	void each(std::function<void(const ActorPtr&)>fn) const;
	// メッセージ処理
	void handle_message(EventMessage message, void* param);
	// 消去
	void clear();

	// コピー禁止
	ActorManager(const ActorManager& other) = delete;
	ActorManager& operator = (const ActorManager& other) = delete;

private:
	// アクターリスト
	ActorList actors_;
};

#endif // !ACTOR_MANAGER_H_