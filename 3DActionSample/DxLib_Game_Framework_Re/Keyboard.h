#ifndef KEYBOARD_H_
#define KEYBOARD_H_

// �N���X�F�L�[�{�[�h����
// ����ҁF�� ���Q�i"Jacky" Ho Siu Ki�j
class Keyboard
{
public:
	// �L�[�{�[�h�̃L�[�̒�`
	static const int Esc;			// �G�X�P�[�v
	static const int Enter;			// �G���^�[
	static const int Space;			// �X�y�[�X
	static const int LeftCtrl;		// ���R���g���[��
	static const int RightCtrl;		// �E�R���g���[��
	static const int LeftShift;		// ���V�t�g
	static const int RightShift;	// �E�V�t�g

	static const int Up;			// ��
	static const int Down;			// ��
	static const int Left;			// ��
	static const int Right;			// �E

	// �ȉ��̓e���L�[
	static const int Num0;
	static const int Num1;
	static const int Num2;
	static const int Num3;
	static const int Num4;
	static const int Num5;
	static const int Num6;
	static const int Num7;
	static const int Num8;
	static const int Num9;

	// �ȉ��̓A���t�@�x�b�g
	static const int A;
	static const int B;
	static const int C;
	static const int D;
	static const int E;
	static const int F;
	static const int G;
	static const int H;
	static const int I;
	static const int J;
	static const int K;
	static const int L;
	static const int M;
	static const int N;
	static const int O;
	static const int P;
	static const int Q;
	static const int R;
	static const int S;
	static const int T;
	static const int U;
	static const int V;
	static const int W;
	static const int X;
	static const int Y;
	static const int Z;

	// �ȉ��͐���
	static const int One;			// 1
	static const int Two;			// 2
	static const int Three;			// 3
	static const int Four;			// 4
	static const int Five;			// 5
	static const int Six;			// 6
	static const int Seven;			// 7
	static const int Eight;			// 8
	static const int Nine;			// 9
	static const int Zero;			// 0


	// ������
	static void initialize();
	// �X�V
	static void update();
	// �L�[��������Ă��邩
	static bool state(int key);
	// �L�[�������ꂽ��
	static bool trigger(int key);
	// �L�[�������ꂽ��
	static bool release(int key);
	// �I������
	static void finalize();

private:
	// ����̃L�[���͏��
	static int current_state_;
	// �O��̃L�[���͏��
	static int previous_state_;
};

#endif // !KEYBOARD_H_