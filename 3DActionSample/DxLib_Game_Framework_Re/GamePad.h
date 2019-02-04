#ifndef GAME_PAD_H_
#define GAME_PAD_H_

// �N���X�F�Q�[���p�b�h����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class GamePad
{
public:
	// �Q�[���p�b�h�̃{�^���̒�`
	static const int Left;		// ��
	static const int Right;		// �E
	static const int Up;		// ��
	static const int Down;		// ��
	static const int A;			// A
	static const int B;			// B
	static const int X;			// X
	static const int Y;			// Y
	static const int L;			// L
	static const int R;			// R
	static const int Back;		// Back
	static const int Start;		// Start

	// ������
	static void initialize();
	// �X�V
	static void update();
	// �{�^����������Ă��邩
	static bool state(int button);
	// �{�^���������ꂽ��
	static bool trigger(int button);
	// �{�^���������ꂽ��
	static bool release(int button);
	// ���A�i���O��x�����͏�Ԃ̎擾
	static int get_analogL_X();
	// ���A�i���O��y�����͏�Ԃ̎擾
	static int get_analogL_Y();
	// �E�A�i���O��x�����͏�Ԃ̎擾
	static int get_analogR_X();
	// �E�A�i���O��y�����͏�Ԃ̎擾
	static int get_analogR_Y();
	// �I������
	static void finalize();

private:
	// ����̃{�^�����͏��
	static int current_state_;
	// �O��̃{�^�����͏��
	static int previous_state_;

	// ���A�i���O��x�����͏��
	static int analogL_X_;
	// ���A�i���O��y�����͏��
	static int analogL_Y_;

	// �E�A�i���O��x�����͏��
	static int analogR_X_;
	// �E�A�i���O��y�����͏��
	static int analogR_Y_;
};

#endif // !GAME_PAD_H_