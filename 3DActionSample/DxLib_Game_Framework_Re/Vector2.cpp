#include "Vector2.h"
#include "MathHelper.h"
#include <cmath>
#include <cfloat>
#include <algorithm>

// �\���́F2D�x�N�g��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �萔�̎��̂��`
const Vector2 Vector2::Zero{ 0.0f, 0.0f };
const Vector2 Vector2::One{ 1.0f, 1.0f };
const Vector2 Vector2::Left{ -1.0f, 0.0f };
const Vector2 Vector2::Right{ 1.0f, 0.0f };
const Vector2 Vector2::Up{ 0.0f, -1.0f };
const Vector2 Vector2::Down{ 0.0f, 1.0f };

// �R���X�g���N�^
Vector2::Vector2(float x, float y) :
	x{ x }, y{ y }
{ }

// ���ς����߂�
float Vector2::Dot(const Vector2& other) const
{
	return (x * other.x) + (y * other.y);
}

// �O�ς����߂�
float Vector2::Cross(const Vector2& other) const
{
	return (x * other.y) + (y * other.x);
}

// ���������߂�
float Vector2::Length() const
{
	return std::sqrt(Dot(*this));
}

// ���K�������x�N�g�������߂�
Vector2 Vector2::Normalize() const
{
	const auto len = Length();
	if (len < FLT_EPSILON) return Vector2::Zero;

	return *this / len;
}

// �p�x����x�N�g���̍쐬
Vector2 Vector2::FromAngle(float degree)
{
	const auto rad = MathHelper::degree_to_radian(degree);	// �ʓx�@�ɕϊ�

	return { std::cos(rad), std::sin(rad) };
}

// �x�N�g���������Ă���p�x�����߂�
float Vector2::ToAngle() const
{
	if (is_zero()) return 0.0f;		// �[���x�N�g���͊p�x�����߂��Ȃ�

	return MathHelper::radian_to_degree(std::atan2(y, x));	// �x���@�ɕϊ�
}

// ��]����
Vector2 Vector2::Rotate(float degree) const
{
	const auto rad = MathHelper::degree_to_radian(degree);	// �ʓx�@�ɕϊ�

	return { x * std::cos(rad) - y * std::sin(rad), x * std::sin(rad) + y * std::cos(rad) };
}

// 2�̃x�N�g���Ȃ��p�x�����߂�
float Vector2::InnerAngle(const Vector2& other) const
{
	// ���K�������p�x���擾
	const auto& n1 = Normalize();
	const auto& n2 = other.Normalize();
	// �p�x�̓��ς��擾
	const auto cos = n1.Dot(n2);

	// ���ς��p�x�ɕϊ����ĕԂ�
	return MathHelper::radian_to_degree(std::acos(cos));
}

// ���������߂�
float Vector2::Distance(const Vector2& other) const
{
	return (*this - other).Length();
}

// ���`��ԏ���
Vector2 Vector2::Lerp(const Vector2& end, float t) const
{
	return { MathHelper::lerp(x, end.x, t), MathHelper::lerp(y, end.y, t) };
}

// ���l�𐧌�
Vector2 Vector2::Clamp(const Vector2& min, const Vector2& max) const
{
	return { MathHelper::clamp(x, min.x, max.x), MathHelper::clamp(y, min.y, max.y) };
}

// �^�[�Q�b�g�����̃x�N�g�������߂�
Vector2 Vector2::ToTarget(const Vector2& target) const
{
	return (target - *this).Normalize();
}

// �^�[�Q�b�g�����̊p�x�����߂�
float Vector2::ToTargetAngle(const Vector2& target) const
{
	return ToTarget(target).ToAngle();
}

// �[���x�N�g���Ȃ̂�
bool Vector2::is_zero() const
{
	return (x == 0.0f) && (y == 0.0f);
}

// ���Z�q�I�[�o�[���[�h
Vector2& operator += (Vector2& lhs, const Vector2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;

	return lhs;
}

Vector2& operator -= (Vector2& lhs, const Vector2& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;

	return lhs;
}

Vector2& operator *= (Vector2& lhs, float scalar)
{
	lhs.x *= scalar;
	lhs.y *= scalar;

	return lhs;
}

Vector2& operator /= (Vector2& lhs, float scalar)
{
	lhs.x /= scalar;
	lhs.y /= scalar;

	return lhs;
}

const Vector2 operator + (const Vector2& lhs, const Vector2& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

const Vector2 operator - (const Vector2& lhs, const Vector2& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

const Vector2 operator * (const Vector2& lhs, float scalar)
{
	return { lhs.x * scalar, lhs.y * scalar };
}

const Vector2 operator * (float scalar, const Vector2& rhs)
{
	return { rhs.x * scalar, rhs.y * scalar };
}

const Vector2 operator / (const Vector2& lhs, float scalar)
{
	return { lhs.x / scalar, lhs.y / scalar };
}

const Vector2 operator - (const Vector2& v)
{
	return { -v.x, -v.y };
}

bool operator == (const Vector2& lhs, const Vector2& rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator != (const Vector2& lhs, const Vector2& rhs)
{
	return !(lhs == rhs);
}

bool operator < (const Vector2& lhs, const Vector2& rhs)
{
	if (lhs.x != rhs.x) return lhs.x < rhs.x;
	if (lhs.y != rhs.y) return lhs.y < rhs.y;

	return false;
}