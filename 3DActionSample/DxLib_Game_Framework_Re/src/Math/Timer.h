#ifndef TIMER_H_
#define TIMER_H_

// �N���X�F�^�C�}�[�iDeltaTime���j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class Timer
{
public:
	// �f�t�H���g�R���X�g���N�^�i�������Ԃ�ݒ肵�Ȃ��j
	Timer() = default;
	// �R���X�g���N�^�i�������Ԃ�ݒ肷��A�������Ԃ̓t���[�����œ��́j
	Timer(float limit_time);
	// �X�V
	void update(float delta_time);
	// ���݂̎��Ԃ��擾
	float get_time() const;
	// ���Z�b�g
	void reset();
	// �^�C���A�E�g��
	bool is_time_out() const;

private:
	float time_current_{ 0.0f };	// ���݂̎���
	float time_limit_{ 0.0f };		// ��������
};

#endif // !TIMER_H_