#include "Color.h"
#include "MathHelper.h"
#include <algorithm>

// 構造体：カラー
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 定数の実体を定義
const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);	// 黒
const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);	// 白

// コンストラクタ
Color::Color(float r, float g, float b, float a) :
	r{ r }, g{ g }, b{ b }, a{ a }
{ }

// 線形補間
Color Color::Lerp(const Color& v1, const Color& v2, float amount)
{
	return Color
	(
		MathHelper::lerp(v1.r, v2.r, amount),
		MathHelper::lerp(v1.g, v2.g, amount),
		MathHelper::lerp(v1.b, v2.b, amount),
		MathHelper::lerp(v1.a, v2.a, amount)
	);
}

// 一致する各成分ペアの最も低い値を含むカラーを求める
Color Color::Min(const Color& v1, const Color& v2)
{
	return Color
	(
		std::min(v1.r, v2.r),
		std::min(v1.g, v2.g),
		std::min(v1.b, v2.b),
		std::min(v1.a, v2.a)
	);
}

// 一致する各成分ペアの最も高い値を含むカラーを求める
Color Color::Max(const Color& v1, const Color& v2)
{
	return Color
	(
		std::max(v1.r, v2.r),
		std::max(v1.g, v2.g),
		std::max(v1.b, v2.b),
		std::max(v1.a, v2.a)
	);
}

// 数値を制限
Color Color::Clamp(const Color& value, const Color& min, const Color& max)
{
	return Color
	(
		MathHelper::clamp(value.r, min.r, max.r),
		MathHelper::clamp(value.g, min.g, max.g),
		MathHelper::clamp(value.b, min.b, max.b),
		MathHelper::clamp(value.a, min.a, max.a)
	);
}

// 演算子オーバーロード
Color& operator += (Color & c1, const Color & c2)
{
	c1.r += c2.r;
	c1.g += c2.g;
	c1.b += c2.b;
	c1.a += c2.a;

	return c1;
}

Color& operator -= (Color & c1, const Color & c2)
{
	c1.r -= c2.r;
	c1.g -= c2.g;
	c1.b -= c2.b;
	c1.a -= c2.a;

	return c1;
}

Color& operator *= (Color & c1, const Color & c2)
{
	c1.r *= c2.r;
	c1.g *= c2.g;
	c1.b *= c2.b;
	c1.a *= c2.a;

	return c1;
}

Color& operator *= (Color & c, float s)
{
	c.r *= s;
	c.g *= s;
	c.b *= s;
	c.a *= s;

	return c;
}

Color& operator /= (Color & c, float s)
{
	c.r /= s;
	c.g /= s;
	c.b /= s;
	c.a /= s;

	return c;
}

Color operator + (Color c1, const Color & c2)
{
	return c1 += c2;
}

Color operator - (Color c1, const Color & c2)
{
	return c1 -= c2;
}

Color operator * (Color c1, const Color & c2)
{
	return c1 *= c2;
}

Color operator * (Color c, float s)
{
	return c *= s;
}

Color operator * (float s, Color c)
{
	return c *= s;
}

Color operator / (Color c, float s)
{
	return c /= s;
}