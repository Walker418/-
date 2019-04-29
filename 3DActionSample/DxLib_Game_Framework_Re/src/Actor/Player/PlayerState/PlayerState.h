#ifndef PLAYER_STATE_ID_H_
#define PLAYER_STATE_ID_H_

// 列挙型：プレイヤーの状態
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
enum class PlayerState
{
	Normal,			// 通常
	Slash1,			// 攻撃（1段目）
	Slash2,			// 攻撃（2段目）
	Slash3,			// 攻撃（3段目）
	JumpAttack1,	// ジャンプ攻撃（1段目）
	JumpAttack2,	// ジャンプ攻撃（2段目）
	Damage,			// 怯み
	Guard,			// ガード
	Blocking,		// ガードによるノックバック
	GuardAttack,	// ガード攻撃
	GuardEnd,		// ガード終了
	Death,			// 死亡
	ForwardEvasion,	// 前回避
	LeftEvasion,	// 左回避
	RightEvasion,	// 右回避
};

#endif // !PLAYER_STATE_ID_H_