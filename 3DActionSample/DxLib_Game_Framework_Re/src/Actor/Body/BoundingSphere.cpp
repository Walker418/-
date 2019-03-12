#include "BoundingSphere.h"
#include "BoundingCapsule.h"
#include "../../Math/Collision/Collision.h"

// �N���X�F�Փ˔���p����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^
BoundingSphere::BoundingSphere(const Vector3& position, const float radius) :
	Body(ShapeType::Sphere, position, radius)
{ }

// ���s�ړ�
IBodyPtr BoundingSphere::translate(const Vector3& position) const
{
	return std::make_shared<BoundingSphere>(translate_e(position));
}

// ���W�ϊ�
IBodyPtr BoundingSphere::transform(const Matrix& matrix) const
{
	return  std::make_shared<BoundingSphere>(transform_e(matrix));
}

// �Փ˔���
bool BoundingSphere::is_collided(const IBody& other) const
{
	// �ǂ��炩�̔��肪�����ł���΁Afalse��Ԃ�
	if (!is_enable() || !other.is_enable()) return false;

	return other.intersects(*this);
}

// ���̂Ƃ̏Փ˔���
bool BoundingSphere::intersects(const BoundingSphere& other) const
{
	return Collision::sphere_to_sphere(position_, radius_, other.position_, other.radius_);
}

// �J�v�Z���Ƃ̏Փ˔���
bool BoundingSphere::intersects(const BoundingCapsule& other) const
{
	return Collision::sphere_to_capsule(position_, radius_, other.position(), other.matrix(), other.length(), other.radius());
}

// �`��
void BoundingSphere::draw() const
{
	// �f�o�b�O���ł����`�悵�Ȃ�
#ifdef _DEBUG
	DrawSphere3D(position_, radius_, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
#endif
}

// ���s�ړ�
BoundingSphere BoundingSphere::translate_e(const Vector3& position) const
{
	return BoundingSphere(position_ + position, radius_);
}

// ���W�ϊ�
BoundingSphere BoundingSphere::transform_e(const Matrix& matrix) const
{
	return BoundingSphere(position_ + matrix.Translation(), radius_ * matrix.Scale().y);
}