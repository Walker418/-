#ifndef BOUNDING_SPHERE_H_
#define BOUNDING_SPHERE_H_

#include "Body.h"
#include "Vector3.h"

// �N���X�F�Փ˔���p����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class BoundingSphere : public Body
{
public:
	// �R���X�g���N�^
	BoundingSphere(const Vector3& position, const float radius);
	// ���s�ړ�
	virtual IBodyPtr translate(const Vector3& position) const override;
	// ���W�ϊ�
	virtual IBodyPtr transform(const Matrix& matrix) const override;
	// �Փ˔���
	virtual bool is_collided(const IBody& other) const override;
	// ���̂Ƃ̏Փ˔���
	virtual bool intersects(const BoundingSphere& other) const override;
	// �J�v�Z���Ƃ̏Փ˔���
	virtual bool intersects(const BoundingCapsule& other) const override;
	// �`��
	virtual void draw() const override;

	// ���s�ړ�
	BoundingSphere translate_e(const Vector3& position) const;
	// ���W�ϊ�
	BoundingSphere transform_e(const Matrix& matrix) const;
};

#endif // !BOUNDING_SPHERE_H_