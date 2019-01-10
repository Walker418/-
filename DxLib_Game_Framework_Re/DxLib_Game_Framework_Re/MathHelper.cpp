#include "MathHelper.h"
#include <cmath>
#include <algorithm>

// クラス：計算補助
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 円周率の実体を定義
const float MathHelper::PI{ 3.14159265358979323846264f };

// ラジアンを角度に変換
float MathHelper::radian_to_degree(float radian)
{
	return radian * (180.0f / PI);
}

// 角度をラジアンに変換
float MathHelper::degree_to_radian(float degree)
{
	return degree * (PI / 180.0f);
}

// 線形補間
float MathHelper::lerp(float start, float end, float t)
{
	return start * (1.0f - t) + end * t;
}

// 数値を制限
float MathHelper::clamp(float value, float min, float max)
{
	return (value < min) ? min : (value > max) ? max : value;
}

// サインを求める
float MathHelper::sin(float degree)
{
	return std::sin(degree_to_radian(degree));
}

// コサインを求める
float MathHelper::cos(float degree)
{
	return std::cos(degree_to_radian(degree));
}

// タンジェントを求める
float MathHelper::tan(float degree)
{
	return std::tan(degree_to_radian(degree));
}

// サインから角度へ逆算
float MathHelper::a_sin(float s)
{
	return radian_to_degree(std::asin(s));
}

// コサインから角度へ逆算
float MathHelper::a_cos(float c)
{
	return radian_to_degree(std::acos(c));
}

// タンジェントから角度へ逆算
float MathHelper::a_tan(float y, float x)
{
	return radian_to_degree(std::atan2(y, x));
}