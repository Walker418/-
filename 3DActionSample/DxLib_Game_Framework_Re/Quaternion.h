#ifndef QUATERNION_H_
#define QUATERNION_H_

struct Vector3;		// 3D�x�N�g���\����
struct Matrix;		// �ϊ��s��\����

// �\���́F�N�I�[�^�j�I���i�l�����j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
struct Quaternion
{
	float x{ 0.0f };	// x����
	float y{ 0.0f };    // y����
	float z{ 0.0f };    // z����
	float w{ 1.0f };    // w����

	// �f�t�H���g�R���X�g���N�^
	Quaternion() = default;
	// �R���X�g���N�^�i�e�����𒼐ړ��́j
	Quaternion(float x, float y, float z, float w);
	// �R���X�g���N�^�i�w�莲���̉�]�p�x����N�I�[�^�j�I�����w��j
	Quaternion(const Vector3& axis, float angle);

	// ���K��
	static Quaternion Normalize(const Quaternion& q);
	// ���K��
	Quaternion& Normalize();

	// ���ς����߂�
	static float Dot(const Quaternion& q1, const Quaternion& q2);
	// ���������߂�
	float Length() const;

	// ���`���
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
	// ��]�s�񂩂�N�I�[�^�j�I�����쐬
	static Quaternion CreateFromRotationMatrix(const Matrix& matrix);
	// �P�ʃN�I�[�^�j�I��
	static const Quaternion Identity;
};

// ���Z�q�I�[�o�[���[�h
Quaternion& operator += (Quaternion& q1, const Quaternion& q2);
Quaternion& operator -= (Quaternion& q1, const Quaternion& q2);
Quaternion& operator *= (Quaternion& q, float s);
Quaternion& operator /= (Quaternion& q, float s);
Quaternion& operator *= (Quaternion& q1, const Quaternion& q2);

Quaternion operator + (Quaternion q1, const Quaternion& q2);
Quaternion operator - (Quaternion q1, const Quaternion& q2);
Quaternion operator * (Quaternion q1, const Quaternion& q2);
Quaternion operator * (Quaternion q, float s);
Quaternion operator * (float s, Quaternion q);
Quaternion operator / (Quaternion q, float s);
Quaternion operator - (const Quaternion& q);

#endif // !QUATERNION_H_