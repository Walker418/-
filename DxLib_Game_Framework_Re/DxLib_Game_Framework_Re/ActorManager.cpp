#include "ActorManager.h"

// クラス：アクター管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// アクターの追加
void ActorManager::add(const ActorPtr& actor)
{
	actors_.push_front(actor);
}

// 更新
void ActorManager::update(float delta_time)
{
	for (const auto& actor : actors_)
	{
		actor->update(delta_time);
	}
}

// 描画
void ActorManager::draw() const
{
	for (const auto& actor : actors_)
	{
		actor->draw();
	}
}

// 衝突判定
void ActorManager::collide()
{
	for (auto i = actors_.begin(); i != actors_.end(); ++i)
	{
		for (auto j = std::next(i); j != actors_.end(); ++j)
		{
			(*i)->collide(**j);
		}
	}
}

// 衝突判定
void ActorManager::collide(Actor& other)
{
	for (const auto& actor : actors_)
	{
		other.collide(*actor);
	}
}

// 衝突判定
void ActorManager::collide(ActorManager& other)
{
	for (const auto& actor : actors_)
	{
		other.collide(*actor);
	}
}

// 削除
void ActorManager::remove()
{
	// 死亡したアクターを削除
	actors_.remove_if([](const ActorPtr& actor) {return actor->is_dead(); });
}

// アクターの取得
ActorPtr ActorManager::find(const std::string& name) const
{
	for (const auto& actor : actors_)
	{
		if (actor->name() == name)
		{
			return actor;
		}
	}
}

// アクター数の取得
unsigned int ActorManager::count() const
{
	return actors_.size();
}

// アクターリストの巡回
void ActorManager::each(std::function<void(const ActorPtr&)> fn) const
{
	for (const auto& actor : actors_)
	{
		fn(actor);
	}
}

// メッセージ処理
void ActorManager::handle_message(EventMessage message, void* param)
{
	for (const auto& actor : actors_)
	{
		actor->handle_message(message, param);
	}
}

// 消去
void ActorManager::clear()
{
	// アクターリストの内容を全て消去
	actors_.clear();
}