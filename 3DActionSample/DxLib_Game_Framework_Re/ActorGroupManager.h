#ifndef ACTOR_GROUP_MANAGER_H_
#define ACTOR_GROUP_MANAGER_H_

#include "ActorManager.h"
#include <map>
#include <functional>

// クラス：アクターグループ管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

enum class ActorGroup;

class ActorGroupManager
{
private:
	// アクターグループマップ
	using ActorGroupMap = std::map<ActorGroup, ActorManager>;

public:
	// デフォルトコンストラクタ
	ActorGroupManager() = default;
	// グループの追加
	void add_group(ActorGroup group);
	// アクターの追加
	void add_actor(ActorGroup group, const ActorPtr& actor);
	// 更新
	void update(float delta_time);
	// 描画
	void draw() const;
	// 描画
	void draw(ActorGroup group) const;
	// 消去
	void clear();
	// アクターの取得
	ActorPtr find(ActorGroup group, const std::string& name) const;
	// アクター数の取得
	unsigned int count(ActorGroup group) const;
	// アクターリストの巡回
	void each(ActorGroup group, std::function<void(const ActorPtr&)> fn) const;
	// 衝突判定
	void collide(ActorGroup group1, ActorGroup group2);
	// 削除
	void remove();
	// メッセージ処理
	void handle_message(EventMessage message, void* param);

	// コピー禁止
	ActorGroupManager(const ActorGroupManager& other) = delete;
	ActorGroupManager& operator = (const ActorGroupManager& other) = delete;

private:
	// アクターグループマップ
	ActorGroupMap actor_group_map_;
};

#endif // !ACTOR_GROUP_MANAGER_H_