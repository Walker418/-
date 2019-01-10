#ifndef IBODY_H_
#define IBODY_H_

#include "IBodyPtr.h"
#include "ShapeType.h"

// �C���^�[�t�F�[�X�F�Փ˔���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

struct Vector3;
struct Matrix;

class BoundingSphere;
class BoundingCapsule;

class IBody
{
public:
	// �R���X�g���N�^
	IBody() { };
	// ���z�f�X�g���N�^
	virtual ~IBody() = default;

	// ���s�ړ�
	virtual IBodyPtr translate(const Vector3& position) const = 0;
	// ���W�ϊ�
	virtual IBodyPtr transform(const Matrix& matrix) const = 0;
	// �Փ˔���
	virtual bool is_collide(const IBody& other) const = 0;
	// ���̂Ƃ̏Փ˔���
	virtual bool intersects(const BoundingSphere& other) const = 0;
	// �J�v�Z���Ƃ̏Փ˔���
	virtual bool intersects(const BoundingCapsule& other) const = 0;

	// �ڐG�����On,Off
	virtual void set_active(bool active) = 0;
	// �ڐG����͗L���ł��邩
	virtual bool is_enable() const = 0;
	// �`��
	virtual void draw() const = 0;
	// �`��̎擾
	virtual ShapeType type() const = 0;
	// ���S���W�̎擾
	virtual Vector3 position() const = 0;
	// �ϊ��s��̎擾
	virtual Matrix matrix() const = 0;
	// ���a�̎擾
	virtual float radius() const = 0;
	// �����̎擾
	virtual float length() const = 0;

	// ���_�̎擾
	virtual Vector3 points(const int index) const = 0;

};

#endif // !IBODY_H_