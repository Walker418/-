#ifndef T_P_CAMERA_H_
#define T_P_CAMERA_H_

#include "../ActorPtr.h"
#include "../Actor.h"
#include "../../Math/CountdownTimer.h"
#include "../../Math/Random.h"

// �N���X�F�O�l�̃J����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �J�����̏�ԗ�
enum TPCameraState
{
	Normal,		// �ʏ�
	Reset,		// ���Z�b�g
};

class TPCamera : public Actor
{
private:
	// �e��萔
	const float YawSpeed{ 3.0f };			// �J�����̍��E��]���x
	const float PitchSpeed{ 1.5f };			// �J�����̏㉺��]���x
	const float CameraHeight{ 22.0f };		// �����_�̍���
	const float CameraDistance{ 60.0f };	// �����_�Ƃ̋���
	const float PitchMax{ 15.0f };			// �p�ő�l
	const float PitchMin{ -20.0f };			// �p�ŏ��l
	const float VibrationTime{ 20.0f };		// �J�����̐U������
	const float VibrationPower{ 0.3f };		// �J�����̐U����

public:
	// �R���X�g���N�^
	TPCamera(IWorld* world);
	// ���b�Z�[�W����
	virtual void handle_message(EventMessage message, void* param = nullptr) override;

private:
	// �X�V
	void update(float delta_time) override;
	// �`��
	void draw() const override;

private:
	// ��Ԃ̍X�V
	void update_state(float delta_time);
	// �ʏ��Ԃł̍X�V
	void normal(float delta_time);
	// �J�������Z�b�g���̍X�V
	void reset(float delta_time);
	// �ړ�����
	void move();
	// ��]����
	void rotate(float delta_time);
	// �ǂƂ̐ڐG����
	void intersect_wall();
	// �J�����̏㉺�U��
	void camera_vibration_V(float delta_time);
	// �U���J�n
	void start_vibration();

	// �J�������Z�b�g�J�n
	void start_reset();

	// �v���C���[�̎Q�Ƃ̎擾
	ActorPtr get_player();
	// �����_�ւ̊p�x���擾�i�����t���j
	float get_angle_to_target(Vector3 target) const;
	// �����_�ւ̊p�x���擾�i���������j
	float get_unsigned_angle_to_target(Vector3 target) const;

private:
	TPCameraState	state_{ TPCameraState::Normal };		// �J�����̏��
	Vector3			target_;								// �����_�i�v���C���[�j
	float			yaw_angle_{ 0.0f };						// y���p�x
	float			pitch_angle_{ 0.0f };					// x���p�x

	CountdownTimer	vibration_timer_{ VibrationTime };		// �J�����U���^�C�}�[
	float			current_power_{ VibrationPower };		// ���݂̐U����
	float			min_pos_y_{ 0.0f };						// �U������y���ŏ����W
	float			max_pos_y_{ 0.0f };						// �U������y���ő���W
	Random			rand_;									// ����������

	Vector3			reset_target_;							// �J�������Z�b�g���̒����_�ʒu
};

#endif // !T_P_CAMERA_H_