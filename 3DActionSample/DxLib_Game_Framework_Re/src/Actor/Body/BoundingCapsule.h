#ifndef BOUNDING_CAPSULE_H_
#define BOUNDING_CAPSULE_H_

#include "Body.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix.h"

// �N���X�F�Փ˔���p�J�v�Z��
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class BoundingCapsule : public Body
{
public:
	// �R���X�g���N�^
	BoundingCapsule(const Vector3& position, const Matrix& matrix, const float radius, const float length);
	// ���s�ړ�
	virtual IBodyPtr translate(const Vector3& position) const override;
	// ���W�ϊ�
	virtual IBodyPtr transform(const Matrix& matrix) const override;
	// �Փ˔���
	virtual bool is_collided(const IBody& other) const override;
	// �J�v�Z���Ƃ̏Փ˔���
	virtual bool intersects(const BoundingCapsule& other) const override;
	// ���̂Ƃ̏Փ˔���
	virtual bool intersects(const BoundingSphere& other) const override;
	// �`��
	virtual void draw() const override;

	// ���s�ړ�
	BoundingCapsule translate_e(const Vector3& position) const;
	// ���W�ϊ�
	BoundingCapsule transform_e(const Matrix& matrix) const;
};

#endif // !BOUNDING_CAPSULE_H_