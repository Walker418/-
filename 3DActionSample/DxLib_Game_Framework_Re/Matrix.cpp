#include "Matrix.h"
#include "MathHelper.h"
#include "Vector3.h"
#include "Quaternion.h"

// �\���́F�ϊ��s��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �萔�̎��̂��`
// �P�ʍs��
const Matrix Matrix::Identity
(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);
// �[���s��
const Matrix Matrix::Zero
(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
);

// �R���X�g���N�^
Matrix::Matrix(float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31,float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
{
	m[0][0] = m11; m[0][1] = m12; m[0][2] = m13; m[0][3] = m14;
	m[1][0] = m21; m[1][1] = m22; m[1][2] = m23; m[1][3] = m24;
	m[2][0] = m31; m[2][1] = m32; m[2][2] = m33; m[2][3] = m34;
	m[3][0] = m41; m[3][1] = m42; m[3][2] = m43; m[3][3] = m44;
}

// �g��k���s����쐬
Matrix Matrix::CreateScale(const Vector3& scale)
{
	return Matrix
	(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

// ���s�ړ��s����쐬
Matrix Matrix::CreateTranslation(const Vector3& translation)
{
	return Matrix
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translation.x, translation.y, translation.z, 1.0f
	);
}

// �C�ӂ̃x�N�g���̎��͂���]����s����쐬
Matrix Matrix::CreateFromAxisAngle(const Vector3& axis, float degree)
{
	return CreateFromQuaternion(Quaternion(axis, degree));
}

// �N�H�[�^�j�I�������]�s����쐬
Matrix Matrix::CreateFromQuaternion(const Quaternion& rotate)
{
	float xx = rotate.x * rotate.x * 2.0f;
	float yy = rotate.y * rotate.y * 2.0f;
	float zz = rotate.z * rotate.z * 2.0f;
	float xy = rotate.x * rotate.y * 2.0f;
	float xz = rotate.x * rotate.z * 2.0f;
	float yz = rotate.y * rotate.z * 2.0f;
	float wx = rotate.w * rotate.x * 2.0f;
	float wy = rotate.w * rotate.y * 2.0f;
	float wz = rotate.w * rotate.z * 2.0f;

	return Matrix
	(
		1.0f - yy - zz, xy + wz, xz - wy, 0.0f,
		xy - wz, 1.0f - xx - zz, yz + wx, 0.0f,
		xz + wy, yz - wx, 1.0f - xx - yy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

// �w�肵�����[�A�s�b�`�A���[���ōs����쐬
Matrix Matrix::CreateFromYawPitchRoll(float yaw, float pitch, float roll)
{
	return Matrix::CreateRotationZ(roll)
		* Matrix::CreateRotationX(pitch)
		* Matrix::CreateRotationY(yaw);
}

// x���܂��̉�]�s����쐬
Matrix Matrix::CreateRotationX(float degree)
{
	float sin = MathHelper::sin(degree);
	float cos = MathHelper::cos(degree);

	return Matrix
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos, sin, 0.0f,
		0.0f, -sin, cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

// y���܂��̉�]�s����쐬
Matrix Matrix::CreateRotationY(float degree)
{
	float sin = MathHelper::sin(degree);
	float cos = MathHelper::cos(degree);

	return Matrix
	(
		cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin, 0.0f, cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

// z���܂��̉�]�s����쐬
Matrix Matrix::CreateRotationZ(float degree)
{
	float sin = MathHelper::sin(degree);
	float cos = MathHelper::cos(degree);

	return Matrix
	(
		cos, sin, 0.0f, 0.0f,
		-sin, cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

// ���[���h�ϊ��s����쐬
Matrix Matrix::CreateWorld(const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
{
	return CreateScale(scale) * CreateFromQuaternion(rotation) * CreateTranslation(translation);
}

// ���[���h�ϊ��s����쐬
Matrix Matrix::CreateWorld(const Vector3& scale, const Matrix& rotation, const Vector3& translation)
{
	return CreateScale(scale) * rotation * CreateTranslation(translation);
}

// ���[���h�ϊ��s����쐬
Matrix Matrix::CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up)
{
	Vector3 l = Vector3::Normalize(Vector3::Cross(up, forward));
	Vector3 u = Vector3::Normalize(Vector3::Cross(forward, l));
	Vector3 f = Vector3::Cross(l, u);

	return Matrix(Matrix::Identity).Forward(f).Up(u).Left(l).Translation(position);
}

#ifdef __DXLIB
// ���i�s����쐬
Matrix Matrix::CreatePerspectiveFieldOfView(float fov, float aspect, float zn, float zf)
{
	float h = 1.0f / MathHelper::tan(fov / 2.0f);
	float w = h / aspect;

	return Matrix
	(
		w, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, zf / (zf - zn), 1.0f,
		0.0f, 0.0f, -zn * zf / (zf - zn), 0.0f
	);
}

// ����s����쐬
Matrix Matrix::CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
	Vector3 z = Vector3::Normalize(target - position);
	Vector3 x = Vector3::Normalize(Vector3::Cross(up, z));
	Vector3 y = Vector3::Cross(z, x);

	return Matrix
	(
		x.x, y.x, z.x, 0.0f,
		x.y, y.y, z.y, 0.0f,
		x.z, y.z, z.z, 0.0f,
		-Vector3::Dot(position, x), -Vector3::Dot(position, y), -Vector3::Dot(position, z), 1.0f
	);
}

#else
// ���i�s����쐬
Matrix Matrix::CreatePerspectiveFieldOfView(float fov, float aspect, float zn, float zf)
{
	float ys = 1.0f / MathHelper::tan(fov / 2.0f);
	float xs = ys / aspect;

	return Matrix
	(
		xs, 0.0f, 0.0f, 0.0f,
		0.0f, ys, 0.0f, 0.0f,
		0.0f, 0.0f, (zf + zn) / (zn - zf), -1.0f,
		0.0f, 0.0f, (2.0f * zf * zn) / (zn - zf), 0.0f
	);
}

// ����s����쐬
Matrix Matrix::CreateLookAt(const Vector3 & position, const Vector3 & target, const Vector3 & up)
{
	Vector3 z = Vector3::Normalize(position - target);
	Vector3 x = Vector3::Normalize(Vector3::Cross(up, z));
	Vector3 y = Vector3::Cross(z, x);

	return Matrix
	(
		x.x, y.x, z.x, 0.0f,
		x.y, y.y, z.y, 0.0f,
		x.z, y.z, z.z, 0.0f,
		-Vector3::Dot(position, x), -Vector3::Dot(position, y), -Vector3::Dot(position, z), 1.0f
	);
}
#endif

// �X�N���[�����W�s����쐬
Matrix Matrix::CreateScreen(float x, float y, float width, float height)
{
	float w = width / 2.0f;
	float h = height / 2.0f;

	return Matrix
	(
		w, 0.0f, 0.0f, 0.0f,
		0.0f, -h, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		w + x, h + y, 0.0f, 1.0f
	);
}

// �t�s������߂�
Matrix Matrix::Invert(const Matrix& matrix)
{
	float a0 = matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0];
	float a1 = matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0];
	float a2 = matrix.m[0][0] * matrix.m[1][3] - matrix.m[0][3] * matrix.m[1][0];
	float a3 = matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1];
	float a4 = matrix.m[0][1] * matrix.m[1][3] - matrix.m[0][3] * matrix.m[1][1];
	float a5 = matrix.m[0][2] * matrix.m[1][3] - matrix.m[0][3] * matrix.m[1][2];
	float b0 = matrix.m[2][0] * matrix.m[3][1] - matrix.m[2][1] * matrix.m[3][0];
	float b1 = matrix.m[2][0] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][0];
	float b2 = matrix.m[2][0] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][0];
	float b3 = matrix.m[2][1] * matrix.m[3][2] - matrix.m[2][2] * matrix.m[3][1];
	float b4 = matrix.m[2][1] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][1];
	float b5 = matrix.m[2][2] * matrix.m[3][3] - matrix.m[2][3] * matrix.m[3][2];
	float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

	if (det == 0.0f)
	{
		return matrix;	// �t�s�񂪑��݂��Ȃ�
	}

	float invDet = 1.0f / det;

	return Matrix
	(
		(matrix.m[1][1] * b5 - matrix.m[1][2] * b4 + matrix.m[1][3] * b3) * invDet,
		(-matrix.m[0][1] * b5 + matrix.m[0][2] * b4 - matrix.m[0][3] * b3) * invDet,
		(matrix.m[3][1] * a5 - matrix.m[3][2] * a4 + matrix.m[3][3] * a3) * invDet,
		(-matrix.m[2][1] * a5 + matrix.m[2][2] * a4 - matrix.m[2][3] * a3) * invDet,
		(-matrix.m[1][0] * b5 + matrix.m[1][2] * b2 - matrix.m[1][3] * b1) * invDet,
		(matrix.m[0][0] * b5 - matrix.m[0][2] * b2 + matrix.m[0][3] * b1) * invDet,
		(-matrix.m[3][0] * a5 + matrix.m[3][2] * a2 - matrix.m[3][3] * a1) * invDet,
		(matrix.m[2][0] * a5 - matrix.m[2][2] * a2 + matrix.m[2][3] * a1) * invDet,
		(matrix.m[1][0] * b4 - matrix.m[1][1] * b2 + matrix.m[1][3] * b0) * invDet,
		(-matrix.m[0][0] * b4 + matrix.m[0][1] * b2 - matrix.m[0][3] * b0) * invDet,
		(matrix.m[3][0] * a4 - matrix.m[3][1] * a2 + matrix.m[3][3] * a0) * invDet,
		(-matrix.m[2][0] * a4 + matrix.m[2][1] * a2 - matrix.m[2][3] * a0) * invDet,
		(-matrix.m[1][0] * b3 + matrix.m[1][1] * b1 - matrix.m[1][2] * b0) * invDet,
		(matrix.m[0][0] * b3 - matrix.m[0][1] * b1 + matrix.m[0][2] * b0) * invDet,
		(-matrix.m[3][0] * a3 + matrix.m[3][1] * a1 - matrix.m[3][2] * a0) * invDet,
		(matrix.m[2][0] * a3 - matrix.m[2][1] * a1 + matrix.m[2][2] * a0) * invDet
	);
}

// �t�s������߂�i��]�ƕ��s�ړ��̂݁j
Matrix Matrix::InvertFast(const Matrix& matrix)
{
	// ��]�s��̋t�s����쐬
	Matrix inv_rotation = Matrix::Transpose(Matrix(matrix).Translation(Vector3::Zero));
	// ���s�ړ��������t�ϊ�
	Vector3 inv_translation = Vector3::Transform(-matrix.Translation(), inv_rotation);
	// �t�s����쐬
	return inv_rotation.Translation(inv_translation);
}

// �s�Ɨ�����ւ���
Matrix Matrix::Transpose(const Matrix& matrix)
{
	return Matrix
	(
		matrix.m[0][0], matrix.m[1][0], matrix.m[2][0], matrix.m[3][0],
		matrix.m[0][1], matrix.m[1][1], matrix.m[2][1], matrix.m[3][1],
		matrix.m[0][2], matrix.m[1][2], matrix.m[2][2], matrix.m[3][2],
		matrix.m[0][3], matrix.m[1][3], matrix.m[2][3], matrix.m[3][3]
	);
}

// ���`��ԏ���
Matrix Matrix::Lerp(const Matrix& v1, const Matrix& v2, float t)
{
	Vector3 scale = Vector3::Lerp(v1.Scale(), v2.Scale(), t);
	Quaternion rotate = Quaternion::Slerp(v1.Rotation(), v2.Rotation(), t);
	Vector3 translation = Vector3::Lerp(v1.Translation(), v2.Translation(), t);

	return Matrix::CreateWorld(scale, rotate, translation);
}

// �s��𕪉�
void Matrix::Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) const
{
	scale = Scale();
	rotation = Rotation();
	translation = Translation();
}

// ���K��
Matrix Matrix::NormalizeRotationMatrix(const Matrix& matrix)
{
	return Matrix(matrix).NormalizeRotationMatrix();
}

// ���K��
Matrix & Matrix::NormalizeRotationMatrix()
{
	Vector3 f(m[2][0], m[2][1], m[2][2]);
	Vector3 u(m[1][0], m[1][1], m[1][2]);
	Vector3 l(Vector3::Normalize(Vector3::Cross(u, f)));

	u = Vector3::Normalize(Vector3::Cross(f, l));
	f = Vector3::Cross(l, u);

	m[0][0] = l.x; m[0][1] = l.y; m[0][2] = l.z;
	m[1][0] = u.x; m[1][1] = u.y; m[1][2] = u.z;
	m[2][0] = f.x; m[2][1] = f.y; m[2][2] = f.z;

	return *this;
}

#ifdef __DXLIB
// �O���x�N�g�����擾
Vector3 Matrix::Forward() const
{
	return Vector3(-m[2][0], -m[2][1], -m[2][2]);
}

// �O���x�N�g����ݒ�
Matrix& Matrix::Forward(const Vector3& forward)
{
	m[2][0] = -forward.x; m[2][1] = -forward.y; m[2][2] = -forward.z;

	return *this;
}

#else
// �O���x�N�g�����擾
Vector3 Matrix::Forward() const
{
	return Vector3(m[2][0], m[2][1], m[2][2]);
}

// �O���x�N�g����ݒ�
Matrix& Matrix::Forward(const Vector3& forward)
{
	m[2][0] = forward.x; m[2][1] = forward.y; m[2][2] = forward.z;
	return *this;
}

#endif // __DXLIB

// ����x�N�g�����擾
Vector3 Matrix::Backward() const
{
	return -Forward();
}

// ����x�N�g����ݒ�
Matrix& Matrix::Backward(const Vector3& backward) {
	return Forward(-backward);
}

// ����x�N�g�����擾
Vector3 Matrix::Up() const
{
	return Vector3(m[1][0], m[1][1], m[1][2]);
}

// ����x�N�g����ݒ�
Matrix& Matrix::Up(const Vector3& up)
{
	m[1][0] = up.x; m[1][1] = up.y; m[1][2] = up.z;
	return *this;
}

// �����x�N�g�����擾
Vector3 Matrix::Down() const
{
	return -Up();
}

// �����x�N�g����ݒ�
Matrix& Matrix::Down(const Vector3& down)
{
	return Up(-down);
}

// ���x�N�g�����擾
Vector3 Matrix::Left() const
{
	return Vector3(m[0][0], m[0][1], m[0][2]);
}

// ���x�N�g����ݒ�
Matrix& Matrix::Left(const Vector3& left)
{
	m[0][0] = left.x; m[0][1] = left.y; m[0][2] = left.z;
	return *this;
}

// �E�x�N�g�����擾
Matrix& Matrix::Right(const Vector3& right)
{
	return Left(-right);
}

// �E�x�N�g����ݒ�
Vector3 Matrix::Right() const
{
	return -Left();
}

// ���s�ړ��x�N�g�����擾
Vector3 Matrix::Translation() const
{
	return Vector3(m[3][0], m[3][1], m[3][2]);
}

// ���s�ړ��x�N�g����ݒ�
Matrix& Matrix::Translation(const Vector3& translation)
{
	m[3][0] = translation.x; m[3][1] = translation.y; m[3][2] = translation.z;
	return *this;
}

// �X�P�[���l���擾
Vector3 Matrix::Scale() const
{
	return Vector3(Left().Length(), Up().Length(), Forward().Length());
}

// ��]�̎擾
Quaternion Matrix::Rotation() const
{
	return Quaternion::CreateFromRotationMatrix(NormalizeRotationMatrix(*this));
}

// ��]�s��̎擾
Matrix Matrix::RotationMatrix() const
{
	return Matrix(*this).Translation(Vector3::Zero);
}

// ���Z�q�I�[�o�[���[�h
Matrix& operator += (Matrix& m1, const Matrix& m2)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m1.m[i][j] += m2.m[i][j];
		}
	}

	return m1;
}

Matrix& operator -= (Matrix& m1, const Matrix& m2)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m1.m[i][j] -= m2.m[i][j];
		}
	}

	return m1;
}

Matrix& operator *= (Matrix& m1, const Matrix& m2)
{
	Matrix result = Matrix::Zero;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	m1 = result;

	return m1;
}

Matrix& operator *= (Matrix& m, float s)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m.m[i][j] *= s;
		}
	}
	return m;
}

Matrix& operator /= (Matrix& m, float s)
{
	return m *= (1.0f / s);
}

Matrix operator * (Matrix m1, const Matrix& m2)
{
	return m1 *= m2;
}

Matrix operator + (Matrix m1, const Matrix& m2)
{
	return m1 += m2;
}

Matrix operator - (Matrix m1, const Matrix& m2)
{
	return m1 -= m2;
}

Matrix operator * (Matrix m, float s)
{
	return m *= s;
}

Matrix operator * (float s, Matrix m)
{
	return m *= s;
}

Matrix operator / (Matrix m, float s)
{
	return m /= s;
}

Vector3 operator * (const Vector3& v, const Matrix& m)
{
	return Vector3::Transform(v, m);
}

Matrix operator - (const Matrix& m)
{
	return Matrix(
		-m.m[0][0], -m.m[0][1], -m.m[0][2], -m.m[0][3],
		-m.m[1][0], -m.m[1][1], -m.m[1][2], -m.m[1][3],
		-m.m[2][0], -m.m[2][1], -m.m[2][2], -m.m[2][3],
		-m.m[3][0], -m.m[3][1], -m.m[3][2], -m.m[3][3]);
}