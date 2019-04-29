#ifndef ATTACK_PARAMETER_H_
#define ATTACK_PARAMETER_H_

#include "../Math/Vector3.h"

// 構造体：攻撃パラメータ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
struct AttackParameter
{
	Vector3	position{ Vector3::Zero };	// コライダーの位置
	int		power{ 0 };					// 攻撃力
	int		impact{ 0 };				// 怯み値（プレイヤーのみ使用）
	float	hit_stop{ 0.0f };			// ヒットストップ（プレイヤーのみ使用）
};

#endif // !ATTACK_PARAMETER_H_