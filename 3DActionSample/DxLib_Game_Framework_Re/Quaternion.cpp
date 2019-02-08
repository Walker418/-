#include "Quaternion.h"
#include "MathHelper.h"
#include "Vector3.h"
#include "Matrix.h"
#include <cmath>

// 構造体：クオータニオン（四元数）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 単位クオータニオンを定義
const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

// コンストラクタ（各成分を直接入力）
Quaternion::Quaternion(float x, float y, float z, float w) :
	x{ x }, y{ y }, z{ z }, w{ w }
{ }

// コンストラクタ（指定軸回りの回転角度からクオータニオンを指定）
Quaternion::Quaternion(const Vector3& axis, float angle)
{
	float sin = MathHelper::sin(angle / 2.0f);
	x = sin * axis.x;
	y = sin * axis.y;
	z = sin * axis.z;
	w = MathHelper::cos(angle / 2.0f);
}

// 正規化
Quaternion Quaternion::Normalize(const Quaternion& q)
{
	return Quaternion(q).Normalize();
}

// 正規化
Quaternion& Quaternion::Normalize()
{
	float len = Length();
	if (len != 0.0f)
	{
		*this /= len;
	}

	return *this;
}

// 内積を求める
float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

// 長さを求める
float Quaternion::Length() const
{
	return std::sqrt(Dot(*this, *this));
}

// 線形補間
Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float cos = Dot(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		t2 = -q2;
	}

	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - cos) > 0.001f)
	{
		float theta = std::acos(cos);
		k0 = std::sin(theta * k0) / std::sin(theta);
		k1 = std::sin(theta * k1) / std::sin(theta);
	}

	return (q1 * k0) + (t2 * k1);
}

// 回転行列からクオータニオンを作成
Quaternion Quaternion::CreateFromRotationMatrix(const Matrix& matrix)
{
	Quaternion result;

	float tr = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2] + 1.0f;
	if (tr >= 1.0f)
	{
		float fourD = 2.0f * std::sqrt(tr);
		result.x = (matrix.m[1][2] - matrix.m[2][1]) / fourD;
		result.y = (matrix.m[2][0] - matrix.m[0][2]) / fourD;
		result.z = (matrix.m[0][1] - matrix.m[1][0]) / fourD;
		result.w = fourD / 4.0f;

		return result;
	}

	int i = 0;
	if (matrix.m[0][0] <= matrix.m[1][1])
	{
		i = 1;
	}
	if (matrix.m[2][2] > matrix.m[i][i])
	{
		i = 2;
	}

	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	tr = matrix.m[i][i] - matrix.m[j][j] - matrix.m[k][k] + 1.0f;
	float fourD = 2.0f * std::sqrt(tr);
	float qa[4];
	qa[i] = fourD / 4.0f;
	qa[j] = (matrix.m[j][i] + matrix.m[i][j]) / fourD;
	qa[k] = (matrix.m[k][i] + matrix.m[i][k]) / fourD;
	qa[3] = (matrix.m[j][k] - matrix.m[k][j]) / fourD;
	result.x = qa[0];
	result.y = qa[1];
	result.z = qa[2];
	result.w = qa[3];

	return result;
}

// 演算子オーバーロード
Quaternion& operator += (Quaternion& q1, const Quaternion& q2)
{
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	q1.w += q2.w;

	return q1;
}

Quaternion& operator -= (Quaternion& q1, const Quaternion& q2)
{
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	q1.w -= q2.w;

	return q1;
}

Quaternion& operator *= (Quaternion& q, float s)
{
	q.x *= s;
	q.y *= s;
	q.z *= s;
	q.w *= s;

	return q;
}

Quaternion& operator /= (Quaternion& q, float s)
{
	q.x /= s;
	q.y /= s;
	q.z /= s;
	q.w /= s;

	return q;
}

Quaternion& operator *= (Quaternion& q1, const Quaternion& q2)
{
	q1 = Quaternion
	(
		 q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
		-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
		 q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
		-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w
	);

	return q1;
}

Quaternion operator + (Quaternion q1, const Quaternion& q2)
{
	return q1 += q2;
}

Quaternion operator - (Quaternion q1, const Quaternion& q2)
{
	return q1 -= q2;
}

Quaternion operator * (Quaternion q1, const Quaternion& q2)
{
	return q1 *= q2;
}

Quaternion operator * (Quaternion q, float s)
{
	return q *= s;
}

Quaternion operator * (float s, Quaternion q)
{
	return q *= s;
}

Quaternion operator / (Quaternion q, float s)
{
	return q /= s;
}

Quaternion operator - (const Quaternion& q)
{
	return Quaternion(-q.x, -q.y, -q.z, -q.w);
}