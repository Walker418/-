#include "Body.h"

// �N���X�F�{�f�B
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
Body::Body() :
	type_{ ShapeType::None }, position_{ Vector3::Zero }, matrix_{ Matrix::Identity }, radius_{ 0.0f }, length_{ 0.0f }, is_enable_{ false }
{ }

// ���̂̃R���X�g���N�^
Body::Body(const ShapeType& type, const Vector3& position, const float radius) :
	type_{ ShapeType::Sphere }, position_{ position }, matrix_{ Matrix::Identity }, radius_{ radius }, length_{ 0.0f }, is_enable_{ true }
{ }

// �J�v�Z���̃R���X�g���N�^
Body::Body(const ShapeType& type, const Vector3& position, const Matrix& matrix, const float radius, const float length) :
	type_{ ShapeType::Capsule }, position_{ position }, matrix_{ matrix }, radius_{ radius }, length_{ length }, is_enable_{ true }
{ }

// �f�X�g���N�^
Body::~Body()
{ }

// �ڐG�����On,Off
void Body::set_active(bool active)
{
	is_enable_ = active;
}

// �ڐG����͗L���ł��邩
bool Body::is_enable() const
{
	return is_enable_;
}

// �`��̎擾
ShapeType Body::type() const
{
	return type_;
}

// ���S���W�̎擾
Vector3 Body::position() const
{
	return position_;
}

// �ϊ��s��̎擾
Matrix Body::matrix() const
{
	return matrix_;
}

// ���a�̎擾
float Body::radius() const
{
	return radius_;
}

// �����̎擾
float Body::length() const
{
	return length_;
}

// ���_�̎擾
Vector3 Body::points(const int index) const
{
	return points_[index];
}