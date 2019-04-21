#include "Timer.h"
#include <algorithm>

// �N���X�F�^�C�}�[�iDeltaTime���j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

// �R���X�g���N�^�i�������Ԃ�ݒ肷��j
Timer::Timer(float limit_time)
{
	time_limit_ = limit_time;
}

// �X�V
void Timer::update(float delta_time)
{
	// �������Ԃ�ݒ肳��Ă��Ȃ��ꍇ�A������DeltaTime�����Z
	// �������Ԃ�ݒ肳��Ă���ꍇ�A�������ԂɒB����܂�DeltaTime�����Z
	(time_limit_ <= 0.0f) ? time_current_ += delta_time : time_current_ = std::min(time_current_ + delta_time, time_limit_);
}

// ���݂̎��Ԃ��擾
float Timer::get_time() const
{
	return time_current_;
}

// ���Z�b�g
void Timer::reset()
{
	time_current_ = 0.0f;
}

// �^�C���A�E�g��
bool Timer::is_time_out() const
{
	if (time_limit_ <= 0.0f) return false;	// �������Ԃ�ݒ肳��Ă��Ȃ��ꍇ�Afalse��Ԃ�

	return time_current_ >= time_limit_;
}