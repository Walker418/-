#ifndef VECTOR3_H_
#define VECTOR3_H_

#include <DxLib.h>

// �\���́F3D�x�N�g��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

struct Matrix;	// �ϊ��s��\����

struct Vector3
{
	float x{ 0.0f };	// x����
	float y{ 0.0f };	// y����
	float z{ 0.0f };	// z����

	// �f�t�H���g�R���X�g���N�^
	Vector3() = default;
	// �R���X�g���N�^
	Vector3(float x, float y, float z);

	// ���������߂�
	float Length() const;
	// �����̕��������߂�
	float LengthSquared() const;
	// ���K�������x�N�g�������߂�
	Vector3& Normalize();
	// �w�肵���x�N�g���𐳋K��
	static Vector3 Normalize(const Vector3& v);
	// 2�̃x�N�g���̓��ς����߂�
	static float Dot(const Vector3& v1, const Vector3& v2);
	// 2�̃x�N�g���̊O�ς����߂�
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);

	// 2�̃x�N�g���̋��������߂�
	static float Distance(const Vector3& v1, const Vector3& v2);
	// 2�̃x�N�g���̋����̕��������߂�
	static float DistanceSquared(const Vector3& v1, const Vector3& v2);
	// ���`��ԏ���
	static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
	// ��v����e�����y�A�̍ł��Ⴂ�l���܂ރx�N�g�������߂�
	static Vector3 Min(const Vector3& v1, const Vector3& v2);
	// ��v����e�����y�A�̍ł������l���܂ރx�N�g�������߂�
	static Vector3 Max(const Vector3& v1, const Vector3& v2);
	// ���l�𐧌�
	static Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max);
	// �s���3D�x�N�g���ɕϊ�
	static Vector3 Transform(const Vector3& position, const Matrix& matrix);
	// �s���@���x�N�g���ɕϊ�
	static Vector3 TransformNormal(const Vector3& position, const Matrix& matrix);
	// ���[�E�s�b�`����x�N�g�����쐬
	static Vector3 CreateFromYawPitch(float yaw, float pitch);
	// ���[�����߂�
	float Yaw() const;
	// �s�b�`�����߂�
	float Pitch() const;
	//  2�̃x�N�g���Ԃ̉s�p�����߂�
	static float Angle(const Vector3& from, const Vector3& to);

	// �萔
	static const Vector3 Up;		// Vector3( 0,  1,  0)
	static const Vector3 Down;      // Vector3( 0, -1,  0)
	static const Vector3 Left;      // Vector3(-1,  0,  0)
	static const Vector3 Right;     // Vector3( 1,  0,  0)
	static const Vector3 Backward;  // Vector3( 0,  0, -1)
	static const Vector3 Forward;   // Vector3( 0,  0,  1)
	static const Vector3 UnitX;     // Vector3( 1,  0,  0)
	static const Vector3 UnitY;     // Vector3( 0,  1,  0)
	static const Vector3 UnitZ;     // Vector3( 0,  1,  1)
	static const Vector3 One;       // Vector3( 1,  1,  1)
	static const Vector3 Zero;      // Vector3( 0,  0,  0)

#ifdef DX_LIB_H
	// Dxlib�p�ϊ��֐�
	Vector3(const DxLib::VECTOR& v) : x(v.x), y(v.y), z(v.z) { }
	// DxLib�̃x�N�g���ɕϊ�
	operator DxLib::VECTOR() const
	{
		return { x, y, z };
	}
#endif
};

// ���Z�q�I�[�o�[���[�h
Vector3& operator += (Vector3& v1, const Vector3& v2);
Vector3& operator -= (Vector3& v1, const Vector3& v2);
Vector3& operator *= (Vector3& v, float s);
Vector3& operator /= (Vector3& v, float s);

Vector3 operator + (Vector3 v1, const Vector3& v2);
Vector3 operator - (Vector3 v1, const Vector3& v2);
Vector3 operator * (Vector3 v, float s);
Vector3 operator * (float s, Vector3 v);
Vector3 operator / (Vector3 v, float s);
Vector3 operator - (const Vector3& v);

#endif // !VECTOR3_H_