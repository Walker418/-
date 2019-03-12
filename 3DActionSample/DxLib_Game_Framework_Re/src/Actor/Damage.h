#ifndef DAMAGE_H_
#define DAMAGE_H_

#include "../Math/Vector3.h"

// ダメージ構造体
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

struct Damage
{
	Vector3 position{ Vector3::Zero };	// 攻撃判定の位置
	int power{ 0 };						// 攻撃力
	int impact{ 0 };					// 怯み値（敵に対して有効）
};

#endif // !DAMAGE_H_