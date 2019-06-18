#ifndef MATRIX_H_
#define MATRIX_H_

#include <DxLib.h>

// �\���́F�ϊ��s��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

struct Vector3;		// 3D�x�N�g���\����
struct Quaternion;	// �N�I�[�^�j�I���\����

struct Matrix
{
	// �P�ʍs��
	float m[4][4]
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	// �f�t�H���g�R���X�g���N�^
	Matrix() = default;
	// �R���X�g���N�^
	Matrix
	(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
	);

	// �g��k���s����쐬
	static Matrix CreateScale(const Vector3& scale);
	// ���s�ړ��s����쐬
	static Matrix CreateTranslation(const Vector3& translation);
	// �C�ӂ̃x�N�g���̎��͂���]����s����쐬
	static Matrix CreateFromAxisAngle(const Vector3& axis, float degree);
	// �N�H�[�^�j�I�������]�s����쐬
	static Matrix CreateFromQuaternion(const Quaternion& rotate);
	// �w�肵�����[�A�s�b�`�A���[���ōs����쐬
	static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll);
	// x���܂��̉�]�s����쐬
	static Matrix CreateRotationX(float degree);
	// y���܂��̉�]�s����쐬
	static Matrix CreateRotationY(float degree);
	// z���܂��̉�]�s����쐬
	static Matrix CreateRotationZ(float degree);

	// ���[���h�ϊ��s����쐬
	static Matrix CreateWorld(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
	// ���[���h�ϊ��s����쐬
	static Matrix CreateWorld(const Vector3& scale, const Matrix& rotation, const Vector3& translation);
	// ���[���h�ϊ��s����쐬
	static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up);

	// ���i�s����쐬
	static Matrix CreatePerspectiveFieldOfView(float fov, float aspect, float near, float far);
	// ����s����쐬
	static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);

	// �X�N���[�����W�s����쐬
	static Matrix CreateScreen(float x, float y, float width, float height);

	// �t�s������߂�
	static Matrix Invert(const Matrix& matrix);
	// �t�s������߂�i��]�ƕ��s�ړ��̂݁j
	static Matrix InvertFast(const Matrix& matrix);
	// �s�Ɨ�����ւ���
	static Matrix Transpose(const Matrix& matrix);
	// ���`��ԏ���
	static Matrix Lerp(const Matrix& v1, const Matrix& v2, float t);
	// �s��𕪉�
	void Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const;
	// ���K��
	static Matrix NormalizeRotationMatrix(const Matrix& matrix);
	// ���K��
	Matrix& NormalizeRotationMatrix();

	// �O���x�N�g�����擾
	Vector3 Forward() const;
	// �O���x�N�g����ݒ�
	Matrix& Forward(const Vector3& forward);
	// ����x�N�g�����擾
	Vector3 Backward() const;
	// ����x�N�g����ݒ�
	Matrix& Backward(const Vector3& backward);
	// ����x�N�g�����擾
	Vector3 Up() const;
	// ����x�N�g����ݒ�
	Matrix& Up(const Vector3& up);
	// �����x�N�g�����擾
	Vector3 Down() const;
	// �����x�N�g����ݒ�
	Matrix& Down(const Vector3& down);
	// ���x�N�g�����擾
	Vector3 Left() const;
	// ���x�N�g����ݒ�
	Matrix& Left(const Vector3& left);
	// �E�x�N�g�����擾
	Vector3 Right() const;
	// �E�x�N�g����ݒ�
	Matrix& Right(const Vector3& rignt);
	// ���s�ړ��x�N�g�����擾
	Vector3 Translation() const;
	// ���s�ړ��x�N�g����ݒ�
	Matrix& Translation(const Vector3& translation);

	// �X�P�[���l���擾
	Vector3 Scale() const;
	// ��]�̎擾
	Quaternion Rotation() const;
	// ��]�s��̎擾
	Matrix RotationMatrix() const;

	// �P�ʍs��̒萔
	static const Matrix Identity;
	// �[���s��̒萔
	static const Matrix Zero;

#ifdef  DX_LIB_H
	// Dxlib�p�ϊ��֐�
	Matrix(const DxLib::MATRIX& mat)
	{
		memcpy(this, &mat, sizeof(Matrix));
	}
	// DxLib�̍s��ɕϊ�
	operator DxLib::MATRIX() const
	{
		DxLib::MATRIX result;
		memcpy(&result, this, sizeof(DxLib::MATRIX));
		return result;
	}
#endif
};

// ���Z�q�I�[�o�[���[�h
Matrix& operator += (Matrix& m1, const Matrix& m2);
Matrix& operator -= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m1, const Matrix& m2);
Matrix& operator *= (Matrix& m, float s);
Matrix& operator /= (Matrix& m, float s);

Matrix operator * (Matrix m1, const Matrix& m2);
Matrix operator * (Matrix m, float s);
Matrix operator * (Matrix s, float m);
Vector3 operator * (const Vector3& v, const Matrix& m);

Matrix operator + (Matrix m1, const Matrix& m2);
Matrix operator - (Matrix m1, const Matrix& m2);
Matrix operator * (Matrix m, float s);
Matrix operator * (float s, Matrix m);
Matrix operator / (Matrix m, float s);
Matrix operator - (const Matrix& v);

#endif // !MATRIX_H_