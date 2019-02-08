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

private:
	// �J�����̈ړ��A��]����
	void move(float delta_time);

private:
	Vector3 target_;						// �����_�i�v���C���[�j
	float yaw_angle_{ 0.0f };				// y���p�x
	float pitch_angle_{ 0.0f };				// x���p�x

	const float RotateSpeed{ 3.0f };		// �J�����̉�]���x
	const float CameraHeight{ 22.0f };		// �����_�̍���
	const float CameraDistance{ 60.0f };	// �����_�Ƃ̋���
	const float PitchMax{ 15.0f };			// �p�ő�l
	const float PitchMin{ -20.0f };			// �p�ŏ��l
};

#endif // !T_P_CAMERA_H_