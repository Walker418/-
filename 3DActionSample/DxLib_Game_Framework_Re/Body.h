#ifndef BODY_H_
#define BODY_H_

#include "IBody.h"
#include "Vector3.h"
#include "Matrix.h"

// �N���X�F�Փ˔���
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Body : public IBody
{
public:
	// �R���X�g���N�^
	Body();
	// ���̂̃R���X�g���N�^
	Body(const ShapeType& type, const Vector3& position, const float radius);
	// �J�v�Z���̃R���X�g���N�^
	Body(const ShapeType& type, const Vector3& position, const Matrix& matrix, const float radius, const float length);
	// �f�X�g���N�^
	~Body();

	// �ڐG�����On,Off
	virtual void set_active(bool active) override;
	// �ڐG����͗L���ł��邩
	virtual bool is_enable() const override;
	// �`��̎擾
	virtual ShapeType type() const override;
	// ���S���W�̎擾
	virtual Vector3 position() const override;
	// �ϊ��s��̎擾
	virtual Matrix matrix() const override;
	// ���a�̎擾
	virtual float radius() const override;
	// �����̎擾
	virtual float length() const override;

	// ���_�̎擾
	virtual Vector3 points(const int index) const override;

protected:
	// �Փ˔���̌`��
	ShapeType	type_;
	// ���S���W
	Vector3		position_;
	// �ϊ��s��
	Matrix		matrix_;
	// ���a
	float		radius_;
	// ����
	float		length_;
	// ���_
	Vector3		points_[4];
	// �ڐG����͗L���ł��邩
	bool		is_enable_;
};

#endif // !BODY_H_