#ifndef DUMMY_BODY_H_
#define DUMMY_BODY_H_

#include "Body.h"

// �N���X�F�_�~�[�̏Փ˔���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class DummyBody : public Body
{
public:
	// �R���X�g���N�^
	DummyBody() { }
	// ���s�ړ�
	virtual IBodyPtr translate(const Vector3& position) const override { return std::make_shared<DummyBody>(); }
	// ���W�ϊ�
	virtual IBodyPtr transform(const Matrix& matrix) const override { return std::make_shared<DummyBody>(); }
	// �Փ˔���
	virtual bool is_collide(const IBody& other) const override { return false; }
	// ���̂Ƃ̏Փ˔���
	virtual bool intersects(const BoundingSphere& other) const override { return false; }
	// �J�v�Z���Ƃ̏Փ˔���
	virtual bool intersects(const BoundingCapsule& other) const override { return false; }
	// �`��
	virtual void draw() const override { }
};

#endif // !DUMMY_BODY_H_