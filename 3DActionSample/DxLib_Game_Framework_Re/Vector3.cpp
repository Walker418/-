#include "Vector3.h"
#include "MathHelper.h"
#include "Matrix.h"
#include <cmath>
#include <algorithm>

// �\���́F3D�x�N�g��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �萔�̎��̂��`
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
#ifdef __DXLIB
const Vector3 Vector3::Backward(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, -1.0f);
#else
const Vector3 Vector3::Backward(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, 1.0f);
#endif
const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);

// �R���X�g���N�^
Vector3::Vector3(float x, float y, float z) :
	x{ x }, y{ y }, z{ z }
{ }

// ���������߂�
float Vector3::Length() const
{
	return std::sqrt(LengthSquared());
}

// �����̕��������߂�
float Vector3::LengthSquared() const
{
	return Dot(*this, *this);
}

// ���K�������x�N�g�������߂�
Vector3& Vector3::Normalize()
{
	const float len = Length();
	if (len != 0.0f)
	{
		*this /= len;
	}

	return *this;
}

// �w�肵���x�N�g���𐳋K��
Vector3 Vector3::Normalize(const Vector3& v)
{
	return Vector3(v).Normalize();
}

// 2�̃x�N�g���̓��ς����߂�
float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

// 2�̃x�N�g���̊O�ς����߂�
Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3
	{
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

// 2�̃x�N�g���̋��������߂�
float Vector3::Distance(const Vector3& v1, const Vector3& v2)
{
	return (v2 - v1).Length();
}

float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2)
{
	return (v2 - v1).LengthSquared();
}

// ���`��ԏ���
Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return Vector3
	(
		MathHelper::lerp(v1.x, v2.x, t),
		MathHelper::lerp(v1.y, v2.y, t),
		MathHelper::lerp(v1.z, v2.z, t)
	);
}

// ��v����e�����y�A�̍ł��Ⴂ�l���܂ރx�N�g�������߂�
Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2)
{
	return Vector3
	(
		std::fmin(v1.x, v2.x),
		std::fmin(v1.y, v2.y),
		std::fmin(v1.z, v2.z)
	);
}

// ��v����e�����y�A�̍ł������l���܂ރx�N�g�������߂�
Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2)
{
	return Vector3();
}

// ���l�𐧌�
Vector3 Vector3::Clamp(const Vector3& value, const Vector3& min, const Vector3& max)
{
	return Vector3
	(
		MathHelper::clamp(value.x, min.x, max.x),
		MathHelper::clamp(value.y, min.y, max.y),
		MathHelper::clamp(value.z, min.z, max.z)
	);
}

// �s���3D�x�N�g���ɕϊ�
Vector3 Vector3::Transform(const Vector3& position, const Matrix& matrix)
{
	float w = position.x * matrix.m[0][3] + position.y * matrix.m[1][3] + position.z * matrix.m[2][3] + matrix.m[3][3];

	return Vector3
	(
		(position.x * matrix.m[0][0] + position.y * matrix.m[1][0] + position.z * matrix.m[2][0] + matrix.m[3][0]) / w,
		(position.x * matrix.m[0][1] + position.y * matrix.m[1][1] + position.z * matrix.m[2][1] + matrix.m[3][1]) / w,
		(position.x * matrix.m[0][2] + position.y * matrix.m[1][2] + position.z * matrix.m[2][2] + matrix.m[3][2]) / w
	);
}

// �s���@���x�N�g���ɕϊ�
Vector3 Vector3::TransformNormal(const Vector3& position, const Matrix& matrix)
{
	return Vector3
	(
		position.x * matrix.m[0][0] + position.y * matrix.m[1][0] + position.z * matrix.m[2][0],
		position.x * matrix.m[0][1] + position.y * matrix.m[1][1] + position.z * matrix.m[2][1],
		position.x * matrix.m[0][2] + position.y * matrix.m[1][2] + position.z * matrix.m[2][2]
	);
}

// ���[�E�s�b�`����x�N�g�����쐬
Vector3 Vector3::CreateFromYawPitch(float yaw, float pitch)
{
	return Vector3
	(
		MathHelper::cos(pitch) * MathHelper::sin(yaw),
		-MathHelper::sin(pitch),
		MathHelper::cos(pitch) * MathHelper::cos(yaw)
	);
}

// ���[�����߂�
float Vector3::Yaw() const
{
	if (Length() == 0.0f)
	{
		return 0.0f;
	}

	return MathHelper::a_tan(x, z);
}

// �s�b�`�����߂�
float Vector3::Pitch() const
{
	float len = Length();
	if (len == 0.0f)
{
		return 0.0f;
	}

	return MathHelper::a_sin(-y / len);
}

//  2�̃x�N�g���Ԃ̉s�p�����߂�
float Vector3::Angle(const Vector3& from, const Vector3& to)
{
	float d = Dot(Normalize(from), Normalize(to));

	return MathHelper::a_cos(MathHelper::clamp(d, -1.0f, 1.0f));
}

// ���Z�q�I�[�o�[���[�h
Vector3& operator += (Vector3& v1, const Vector3& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

Vector3& operator -= (Vector3& v1, const Vector3& v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

Vector3& operator *= (Vector3& v, float s)
{
	v.x *= s;
	v.y *= s;
	v.z *= s;

	return v;
}

Vector3& operator /= (Vector3& v, float s)
{
	v.x /= s;
	v.y /= s;
	v.z /= s;

	return v;
}

Vector3 operator + (Vector3 v1, const Vector3& v2)
{
	return v1 += v2;
}

Vector3 operator - (Vector3 v1, const Vector3& v2)
{
	return v1 -= v2;
}

Vector3 operator * (Vector3 v, float s)
{
	return v *= s;
}

Vector3 operator * (float s, Vector3 v)
{
	return v *= s;
}

Vector3 operator / (Vector3 v, float s)
{
	return v /= s;
}

Vector3 operator - (const Vector3& v)
{
	return Vector3(-v.x, -v.y, -v.z);
}