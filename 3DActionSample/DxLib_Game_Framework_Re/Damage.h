#ifndef DAMAGE_H_
#define DAMAGE_H_

// ダメージ構造体
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

struct Damage
{
	// 威力と怯み値を記憶（怯み値は敵のみ）
	int power = 0, impact = 0;
};

#endif // !DAMAGE_H_