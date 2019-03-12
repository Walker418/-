#include "Vector2.h"
#include "MathHelper.h"
#include <cmath>
#include <cfloat>
#include <algorithm>

// 構造体：2Dベクトル
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 定数の実体を定義
const Vector2 Vector2::Zero{ 0.0f, 0.0f };
const Vector2 Vector2::One{ 1.0f, 1.0f };
const Vector2 Vector2::Left{ -1.0f, 0.0f };
const Vector2 Vector2::Right{ 1.0f, 0.0f };
const Vector2 Vector2::Up{ 0.0f, -1.0f };
const Vector2 Vector2::Down{ 0.0f, 1.0f };

// コンストラクタ
Vector2::Vector2(float x, float y) :
	x{ x }, y{ y }
{ }

// 内積を求める
float Vector2::Dot(const Vector2& other) const
{
	return (x * other.x) + (y * other.y);
}

// 外積を求める
float Vector2::Cross(const Vector2& other) const
{
	return (x * other.y) + (y * other.x);
}

// 長さを求める
float Vector2::Length() const
{
	return std::sqrt(Dot(*this));
}

// 正規化したベクトルを求める
Vector2 Vector2::Normalize() const
{
	const auto len = Length();
	if (len < FLT_EPSILON) return Vector2::Zero;

	return *this / len;
}

// 角度からベクトルの作成
Vector2 Vector2::FromAngle(float degree)
{
	const auto rad = MathHelper::degree_to_radian(degree);	// 弧度法に変換

	return { std::cos(rad), std::sin(rad) };
}

// ベクトルが向いている角度を求める
float Vector2::ToAngle() const
{
	if (is_zero()) return 0.0f;		// ゼロベクトルは角度を求められない

	return MathHelper::radian_to_degree(std::atan2(y, x));	// 度数法に変換
}

// 回転処理
Vector2 Vector2::Rotate(float degree) const
{
	const auto rad = MathHelper::degree_to_radian(degree);	// 弧度法に変換

	return { x * std::cos(rad) - y * std::sin(rad), x * std::sin(rad) + y * std::cos(rad) };
}

// 2つのベクトルなす角度を求める
float Vector2::InnerAngle(const Vector2& other) const
{
	// 正規化した角度を取得
	const auto& n1 = Normalize();
	const auto& n2 = other.Normalize();
	// 角度の内積を取得
	const auto cos = n1.Dot(n2);

	// 内積を角度に変換して返す
	return MathHelper::radian_to_degree(std::acos(cos));
}

// 距離を求める
float Vector2::Distance(const Vector2& other) const
{
	return (*this - other).Length();
}

// 線形補間処理
Vector2 Vector2::Lerp(const Vector2& end, float t) const
{
	return { MathHelper::lerp(x, end.x, t), MathHelper::lerp(y, end.y, t) };
}

// 数値を制限
Vector2 Vector2::Clamp(const Vector2& min, const Vector2& max) const
{
	return { MathHelper::clamp(x, min.x, max.x), MathHelper::clamp(y, min.y, max.y) };
}

// ターゲット方向のベクトルを求める
Vector2 Vector2::ToTarget(const Vector2& target) const
{
	return (target - *this).Normalize();
}

// ターゲット方向の角度を求める
float Vector2::ToTargetAngle(const Vector2& target) const
{
	return ToTarget(target).ToAngle();
}

// ゼロベクトルなのか
bool Vector2::is_zero() const
{
	return (x == 0.0f) && (y == 0.0f);
}

// 演算子オーバーロード
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