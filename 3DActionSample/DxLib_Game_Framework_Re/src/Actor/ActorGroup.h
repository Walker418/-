#ifndef ACTOR_GROUP_H_
#define ACTOR_GROUP_H_

// 列挙型：アクターグループ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
enum class ActorGroup
{
	None,			// 所属無し
	Player,			// プレイヤー
	PlayerAttack,	// プレイヤーの攻撃
	Enemy,			// 敵
	EnemyAttack,	// 敵の攻撃
	Effect,			// エフェクト
	UI,				// UI
};

#endif // !ACTOR_GROUP_H_