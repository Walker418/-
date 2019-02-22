#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include "Actor.h"

// クラス：敵生成、管理
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class EnemyManager : public Actor
{
public:
	// コンストラクタ
	explicit EnemyManager(IWorld* world);
	// 更新
	virtual void update(float delta_time) override;
	// 描画
	virtual void draw() const override;
	// メッセージ処理
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// 敵生成タイマー
	float	generate_timer_{ 0.0f };
};

#endif // !ENEMY_MANAGER_H_