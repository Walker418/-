#include "Color.h"
#include "MathHelper.h"
#include <algorithm>

// �\���́F�J���[
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �萔�̎��̂��`
const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);	// ��
const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);	// ��

// �R���X�g���N�^
Color::Color(float r, float g, float b, float a) :
	r{ r }, g{ g }, b{ b }, a{ a }
{ }

// ���`���
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

// ��v����e�����y�A�̍ł��Ⴂ�l���܂ރJ���[�����߂�
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

// ��v����e�����y�A�̍ł������l���܂ރJ���[�����߂�
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

// ���l�𐧌�
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

// ���Z�q�I�[�o�[���[�h
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