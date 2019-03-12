#include "Quaternion.h"
#include "MathHelper.h"
#include "Vector3.h"
#include "Matrix.h"
#include <cmath>

// �\���́F�N�I�[�^�j�I���i�l�����j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �P�ʃN�I�[�^�j�I�����`
const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

// �R���X�g���N�^�i�e�����𒼐ړ��́j
Quaternion::Quaternion(float x, float y, float z, float w) :
	x{ x }, y{ y }, z{ z }, w{ w }
{ }

// �R���X�g���N�^�i�w�莲���̉�]�p�x����N�I�[�^�j�I�����w��j
Quaternion::Quaternion(const Vector3& axis, float angle)
{
	float sin = MathHelper::sin(angle / 2.0f);
	x = sin * axis.x;
	y = sin * axis.y;
	z = sin * axis.z;
	w = MathHelper::cos(angle / 2.0f);
}

// ���K��
Quaternion Quaternion::Normalize(const Quaternion& q)
{
	return Quaternion(q).Normalize();
}

// ���K��
Quaternion& Quaternion::Normalize()
{
	float len = Length();
	if (len != 0.0f)
	{
		*this /= len;
	}

	return *this;
}

// ���ς����߂�
float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

// ���������߂�
float Quaternion::Length() const
{
	return std::sqrt(Dot(*this, *this));
}

// ���`���
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

// ��]�s�񂩂�N�I�[�^�j�I�����쐬
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

// ���Z�q�I�[�o�[���[�h
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