#include "EnemyManager.h"

// クラス：敵生成、管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ
EnemyManager::EnemyManager(IWorld* world) :
	Actor(world, "EnemyManager")
{
	generate_timer_ = 0.0f;
}

// 更新
void EnemyManager::update(float delta_time)
{

}

// 描画
void EnemyManager::draw() const
{

}

// メッセージ処理
void EnemyManager::handle_message(EventMessage message, void* param)
{

}