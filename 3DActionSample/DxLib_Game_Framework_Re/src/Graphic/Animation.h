#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <array>
#include "../Math/Matrix.h"

// �N���X�F�A�j���[�V��������
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Animation
{
public:
	using Matrices = std::array<Matrix, 256>;

public:
	// �R���X�g���N�^
	Animation(int model, int motion);
	// �X�V
	void update(float delta_time);
	// ���[�V�����̕ύX
	void change_motion(int motion);
	// ���ݍĐ����̃��[�V�����̎擾
	int motion() const;
	// �ϊ��s��̎擾
	const Matrices& local_matrices() const;
	// �{�[�����̎擾
	int bone_count() const;
	// �I�����Ԃ̎擾
	float end_time() const;

	// �A�j���[�V�����̍Đ����x�̕ύX
	void change_speed(float speed);
	// �A�j���[�V�����̍Đ����x�̃��Z�b�g
	void reset_speed();

private:
	// ��ԃt���[����
	const float LerpTime{ 10.0f };
	// �A�j���[�V�����ԍ�
	int			model_{ 0 };
	// �Đ����̃��[�V�����ԍ�
	int			motion_{ 0 };
	// �Đ����̃A�j���[�V�����^�C�}�[
	float		motion_timer_{ 0.0f };
	// �O��Đ��������[�V�����ԍ�
	int			prev_motion_{ 0 };
	// �O��Đ������ŏI�A�j���[�V�����^�C�}�[
	float		prev_motion_timer_{ 0.0f };
	// ��ԃA�j���[�V�����^�C�}�[
	float		lerp_timer_{ 0.0f };
	// �A�j���[�V�����ϊ��s��
	Matrices	local_matrices_;
	// �A�j���[�V�����̍Đ����x�{��
	float		animation_speed_{ 1.0f };
};

#endif // !ANIMATION_H_