#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

// クラス：計算補助
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class MathHelper
{
public:
	// 円周率の定数
	static const float PI;

	// ラジアンを角度に変換
	static float radian_to_degree(float radian);
	// 角度をラジアンに変換
	static float degree_to_radian(float degree);
	// 線形補間
	static float lerp(float start, float end, float t);
	// 数値を制限
	static float clamp(float value, float min, float max);

	// サインを求める
	static float sin(float degree);
	// コサインを求める
	static float cos(float degree);
	// タンジェントを求める
	static float tan(float degree);
	// サインから角度へ逆算
	static float a_sin(float s);
	// コサインから角度へ逆算
	static float a_cos(float c);
	// タンジェントから角度へ逆算
	static float a_tan(float y, float x);
};

#endif // !MATH_HELPER_H_