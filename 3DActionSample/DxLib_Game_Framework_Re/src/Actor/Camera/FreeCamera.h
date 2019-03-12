#ifndef FREE_CAMERA_H_
#define FREE_CAMERA_H_

#include "../Actor.h"

// �N���X�F�J�����i���R�ɓ�������j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class FreeCamera : public Actor
{
public:
	// �R���X�g���N�^
	FreeCamera(IWorld* world, const Vector3& position, const Matrix& rotation);

private:
	// �X�V
	void update(float delta_time) override;
	// �`��
	void draw() const override;
};

#endif // !FREE_CAMERA_H_