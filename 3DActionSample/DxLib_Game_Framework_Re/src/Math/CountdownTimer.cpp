#include "CountdownTimer.h"
#include <algorithm>

// �N���X�F�J�E���g�_�E���^�C�}�[�iDeltaTime���j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^�i���Ԃ̓t���[�����œ��́j
CountdownTimer::CountdownTimer(float time)
{
	time_set_ = time;
}

// �X�V
void CountdownTimer::update(float delta_time)
{
	// �ݒ肳�ꂽ���Ԃ̊ԁA���ݎ��Ԃ����Z
	time_current_ = std::max(time_current_ - delta_time, 0.0f);
}

// ���Z�b�g
void CountdownTimer::reset()
{
	time_current_ = time_set_;
}

// �����I��
void CountdownTimer::shut()
{
	time_current_ = 0.0f;
}

// �^�C���A�E�g��
bool CountdownTimer::is_time_out() const
{
	return time_current_ <= 0.0f;
}