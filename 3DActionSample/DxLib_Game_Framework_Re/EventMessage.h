#ifndef EVENT_MESSAGE_H_
#define EVENT_MESSAGE_H_

// 列挙型：イベントメッセージ種類
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
enum class EventMessage
{
	None,			// ダミー
	PlayerDead,		// プレイヤー死亡
	EnemyDead,		// 敵死亡
	PlayerDamage,	// プレイヤーがダメージを受ける
	EnemyDamage,	// 敵がダメージを受ける
};

#endif // !EVENT_MESSAGE_H_