#ifndef T_P_CAMERA_H_
#define T_P_CAMERA_H_

#include "Actor.h"

// �N���X�F�O�l�̃J����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class TPCamera : public Actor
{
public:
	// �R���X�g���N�^
	TPCamera(IWorld* world);

private:
	// �X�V
	void update(float delta_time) override;
	// �`��
	void draw() const override;
	// �ړ�
	void move(
		const Vector3& rest_position,		// �o�l�̐Î~�ʒu
		float stiffness,					// �o�l�̋���
		float friction,						// ���C��
		float mass							// ����
	);
};

#endif // !T_P_CAMERA_H_