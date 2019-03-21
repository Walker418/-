#ifndef TITLE_CAMERA_H_
#define TITLE_CAMERA_H_

#include "../Actor.h"

// �N���X�F�^�C�g����ʗp�J����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class TitleCamera : public Actor
{
public:
	// �R���X�g���N�^
	TitleCamera(IWorld* world, const Vector3& position);

private:
	// �X�V
	void update(float delta_time) override;
	// �`��
	void draw() const override;
	// �J�����̈ړ��A��]
	void rotate(float delta_time);

private:
	Vector3		target_{ Vector3::Zero };	// �����_
	float		yaw_angle_{ 0.0f };			// y���p�x
	float		pitch_angle_{ -28.0f };		// x���p�x
	
	const float CameraHeight{ 50.0f };		// �J�����̍���
	const float	CameraDistance{ 500.0f };	// �J�����̋���
	const float	RotateSpeed{ 0.3f };		// ��]���x
};

#endif // !TITLE_CAMERA_H_