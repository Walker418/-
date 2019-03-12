#ifndef VECTOR2_H_
#define VECTOR2_H_

// �\���́F2D�x�N�g��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
struct Vector2
{
	float x{ 0.0f };	// x����
	float y{ 0.0f };	// y����

	// �f�t�H���g�R���X�g���N�^
	Vector2() = default;
	// �R���X�g���N�^
	Vector2(float x, float y);

	// ���ς����߂�
	float Dot(const Vector2& other) const;
	// �O�ς����߂�
	float Cross(const Vector2& other) const;
	// ���������߂�
	float Length() const;
	// ���K�������x�N�g�������߂�
	Vector2 Normalize() const;

	// �p�x����x�N�g���̍쐬
	static Vector2 FromAngle(float degree);
	// �x�N�g���������Ă���p�x�����߂�
	float ToAngle() const;
	// ��]����
	Vector2 Rotate(float degree) const;
	// 2�̃x�N�g���Ȃ��p�x�����߂�
	float InnerAngle(const Vector2& other) const;

	// ���������߂�
	float Distance(const Vector2& other) const;
	// ���`��ԏ���
	Vector2 Lerp(const Vector2& end, float t) const;
	// ���l�𐧌�
	Vector2 Clamp(const Vector2& min, const Vector2& max) const;
	// �^�[�Q�b�g�����̃x�N�g�������߂�
	Vector2 ToTarget(const Vector2& target) const;
	// �^�[�Q�b�g�����̊p�x�����߂�
	float ToTargetAngle(const Vector2& target) const;

	// �[���x�N�g���Ȃ̂�
	bool is_zero() const;

	// �萔
	static const Vector2 Zero;  //  0.0f  0.0f
	static const Vector2 One;   //  1.0f  1.0f
	static const Vector2 Left;  // -1.0f  0.0f
	static const Vector2 Right; //  1.0f  0.0f
	static const Vector2 Down;  //  0.0f  1.0f
	static const Vector2 Up;    //  0.0f -1.0f
};

// ���Z�q�I�[�o�[���[�h
Vector2& operator += (Vector2& lhs, const Vector2& rhs);
Vector2& operator -= (Vector2& lhs, const Vector2& rhs);
Vector2& operator *= (Vector2& lhs, float scalar);
Vector2& operator /= (Vector2& lhs, float scalar);

const Vector2 operator + (const Vector2& lhs, const Vector2& rhs);
const Vector2 operator - (const Vector2& lhs, const Vector2& rhs);
const Vector2 operator * (const Vector2& lhs, float scalar);
const Vector2 operator * (float scalar, const Vector2& rhs);
const Vector2 operator / (const Vector2& lhs, float scalar);
const Vector2 operator - (const Vector2& v);

bool operator == (const Vector2& lhs, const Vector2& rhs);
bool operator != (const Vector2& lhs, const Vector2& rhs);
bool operator < (const Vector2& lhs, const Vector2& rhs);

#endif // !VECTOR2_H_