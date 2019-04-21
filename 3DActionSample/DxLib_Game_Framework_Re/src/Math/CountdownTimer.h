#ifndef COUNTDOWN_TIMER_H_
#define COUNTDOWN_TIMER_H_

// �N���X�F�J�E���g�_�E���^�C�}�[�iDeltaTime���j
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j

class CountdownTimer
{
public:
	// �R���X�g���N�^�i���Ԃ̓t���[�����œ��́j
	CountdownTimer(float time);
	// �X�V
	void update(float delta_time);
	// ���Z�b�g
	void reset();
	// �^�C���A�E�g��
	bool is_time_out() const;

private:
	float time_current_{ 0.0f };	// ���݂̎���
	float time_set_{ 0.0f };		// �ݒ莞��
};

#endif // !COUNTDOWN_TIMER_H_