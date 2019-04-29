#ifndef PLAYER_PARAMETER_H_
#define PLAYER_PARAMETER_H_

// プレイヤーのパラメータ
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
namespace PlayerParameter
{
	const int	HP{ 100 };					// 最大体力
	const float	WalkSpeed{ 0.3f };			// 歩行速度
	const float	DashSpeed{ 0.8f };			// ダッシュ速度
	const float	RotateSpeed{ 0.3f };		// 回転速度
	const float	Gravity{ 0.03f };			// 落下速度

	const int	Power_Atk1{ 3 };			// 攻撃1段目の威力
	const int	Power_Atk2{ 2 };			// 攻撃2段目の威力
	const int	Power_Atk3{ 5 };			// 攻撃3段目の威力
	const int	Power_JumpAtk1{ 5 };		// ジャンプ攻撃1段目の威力
	const int	Power_JumpAtk2{ 2 };		// ジャンプ攻撃2段目の威力
	const int	Power_GuardAtk{ 3 };		// ガード攻撃の威力

	const int	Wince_Atk1{ 1 };			// 攻撃1段目の怯み値
	const int	Wince_Atk2{ 1 };			// 攻撃2段目の怯み値
	const int	Wince_Atk3{ 3 };			// 攻撃3段目の怯み値
	const int	Wince_JumpAtk1{ 3 };		// ジャンプ攻撃1段目の怯み値
	const int	Wince_JumpAtk2{ 1 };		// ジャンプ攻撃1段目の怯み値
	const int	Wince_GuardAtk{ 1 };		// ガード攻撃の怯み値

	// ヒットストップ
	const float HitStop_Short{ 6.0f };		// 短い
	const float HitStop_Long{ 10.0f };		// 長い

	const float EvasionTime{ 30.0f };		// 回避状態維持フレーム数
	const float InvincibleTime{ 12.0f };	// 無敵状態維持フレーム数
	const float EvasionSpeed{ 1.2f };		// 回避時の移動速度
};

#endif // !PLAYER_PARAMETER_H_