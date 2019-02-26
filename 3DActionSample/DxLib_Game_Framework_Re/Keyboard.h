#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <DxLib.h>
#include <array>

// �L�[�{�[�h�̃L�[
namespace Keycode
{
	const UINT Back = KEY_INPUT_BACK;				// �o�b�N�X�y�[�X�L�[
	const UINT Tab = KEY_INPUT_TAB;					// �^�u�L�[
	const UINT Return = KEY_INPUT_RETURN;			// �G���^�[�L�[
	const UINT L_Shift = KEY_INPUT_LSHIFT;			// ���V�t�g�L�[
	const UINT R_Shift = KEY_INPUT_RSHIFT;			// �E�V�t�g�L�[
	const UINT L_Control = KEY_INPUT_LCONTROL;		// ���R���g���[���L�[
	const UINT R_Control = KEY_INPUT_RCONTROL;		// �E�R���g���[���L�[
	const UINT Escape = KEY_INPUT_ESCAPE;			// �G�X�P�[�v�L�[
	const UINT Space = KEY_INPUT_SPACE;				// �X�y�[�X�L�[
	const UINT PageUp = KEY_INPUT_PGUP;				// �y�[�W�A�b�v�L�[
	const UINT PageDown = KEY_INPUT_PGDN;			// �y�[�W�_�E���L�[
	const UINT End = KEY_INPUT_END;					// �G���h�L�[
	const UINT Home = KEY_INPUT_HOME;				// �z�[���L�[
	const UINT Left = KEY_INPUT_LEFT;				// ���L�[
	const UINT Up = KEY_INPUT_UP;					// ��L�[
	const UINT Right = KEY_INPUT_RIGHT;				// �E�L�[
	const UINT Down = KEY_INPUT_DOWN;				// ���L�[
	const UINT Insert = KEY_INPUT_INSERT;			// �C���T�[�g�L�[
	const UINT Delete = KEY_INPUT_DELETE;			// �f���[�g�L�[

	const UINT Minus = KEY_INPUT_MINUS;				// -�L�[
	const UINT Yen = KEY_INPUT_YEN;					// \�L�[
	const UINT PrevTrack = KEY_INPUT_PREVTRACK;		// ^�L�[
	const UINT Period = KEY_INPUT_PERIOD;			// .�L�[
	const UINT Slash = KEY_INPUT_SLASH;				// /�L�[
	const UINT L_Alt = KEY_INPUT_LALT;				// ��Alt�L�[
	const UINT R_Alt = KEY_INPUT_RALT;				// �EAlt�L�[
	const UINT Scroll = KEY_INPUT_SCROLL;			// ScrollLock�L�[
	const UINT SemiColon = KEY_INPUT_SEMICOLON;		// ;�L�[
	const UINT Colon = KEY_INPUT_COLON;				// :�L�[
	const UINT L_Bracket = KEY_INPUT_LBRACKET;		// [�L�[
	const UINT R_Bracket = KEY_INPUT_RBRACKET;		// ]�L�[
	const UINT At = KEY_INPUT_AT;					// @�L�[
	const UINT BackSlash = KEY_INPUT_BACKSLASH;		// \�L�[
	const UINT Comma = KEY_INPUT_COMMA;				// ,�L�[
	const UINT CapsLock = KEY_INPUT_CAPSLOCK;		// CapsLock�L�[
	const UINT Pause = KEY_INPUT_PAUSE;				// PauseBreak�L�[

	const UINT NumPad_0 = KEY_INPUT_NUMPAD0;			// �e���L�[ 0
	const UINT NumPad_1 = KEY_INPUT_NUMPAD1;			// �e���L�[ 1
	const UINT NumPad_2 = KEY_INPUT_NUMPAD2;			// �e���L�[ 2
	const UINT NumPad_3 = KEY_INPUT_NUMPAD3;			// �e���L�[ 3
	const UINT NumPad_4 = KEY_INPUT_NUMPAD4;			// �e���L�[ 4
	const UINT NumPad_5 = KEY_INPUT_NUMPAD5;			// �e���L�[ 5
	const UINT NumPad_6 = KEY_INPUT_NUMPAD6;			// �e���L�[ 6
	const UINT NumPad_7 = KEY_INPUT_NUMPAD7;			// �e���L�[ 7
	const UINT NumPad_8 = KEY_INPUT_NUMPAD8;			// �e���L�[ 8
	const UINT NumPad_9 = KEY_INPUT_NUMPAD9;			// �e���L�[ 9
	const UINT Multiply = KEY_INPUT_MULTIPLY;			// �e���L�[ *
	const UINT Add = KEY_INPUT_ADD;						// �e���L�[ +
	const UINT Subtract = KEY_INPUT_SUBTRACT;			// �e���L�[ -
	const UINT Decimal = KEY_INPUT_DECIMAL;				// �e���L�[ .
	const UINT Divide = KEY_INPUT_DIVIDE;				// �e���L�[ /
	const UINT NumPad_Enter = KEY_INPUT_NUMPADENTER;	// �e���L�[ Enter

	const UINT F1 = KEY_INPUT_F1;			// F1�L�[
	const UINT F2 = KEY_INPUT_F2;			// F2�L�[
	const UINT F3 = KEY_INPUT_F3;			// F3�L�[
	const UINT F4 = KEY_INPUT_F4;			// F4�L�[
	const UINT F5 = KEY_INPUT_F5;			// F5�L�[
	const UINT F6 = KEY_INPUT_F6;			// F6�L�[
	const UINT F7 = KEY_INPUT_F7;			// F7�L�[
	const UINT F8 = KEY_INPUT_F8;			// F8�L�[
	const UINT F9 = KEY_INPUT_F9;			// F9�L�[
	const UINT F10 = KEY_INPUT_F10;			// F10�L�[
	const UINT F11 = KEY_INPUT_F11;			// F11�L�[
	const UINT F12 = KEY_INPUT_F12;			// F12�L�[

	const UINT A = KEY_INPUT_A;				// A�L�[
	const UINT B = KEY_INPUT_B;				// B�L�[
	const UINT C = KEY_INPUT_C;				// C�L�[
	const UINT D = KEY_INPUT_D;				// D�L�[
	const UINT E = KEY_INPUT_E;				// E�L�[
	const UINT F = KEY_INPUT_F;				// F�L�[
	const UINT G = KEY_INPUT_G;				// G�L�[
	const UINT H = KEY_INPUT_H;				// H�L�[
	const UINT I = KEY_INPUT_I;				// I�L�[
	const UINT J = KEY_INPUT_J;				// J�L�[
	const UINT K = KEY_INPUT_K;				// K�L�[
	const UINT L = KEY_INPUT_L;				// L�L�[
	const UINT M = KEY_INPUT_M;				// M�L�[
	const UINT N = KEY_INPUT_N;				// N�L�[
	const UINT O = KEY_INPUT_O;				// O�L�[
	const UINT P = KEY_INPUT_P;				// P�L�[
	const UINT Q = KEY_INPUT_Q;				// Q�L�[
	const UINT R = KEY_INPUT_R;				// R�L�[
	const UINT S = KEY_INPUT_S;				// S�L�[
	const UINT T = KEY_INPUT_T;				// T�L�[
	const UINT U = KEY_INPUT_U;				// U�L�[
	const UINT V = KEY_INPUT_V;				// V�L�[
	const UINT W = KEY_INPUT_W;				// W�L�[
	const UINT X = KEY_INPUT_X;				// X�L�[
	const UINT Y = KEY_INPUT_Y;				// Y�L�[
	const UINT Z = KEY_INPUT_Z;				// Z�L�[

	const UINT Zero = KEY_INPUT_0;			// 0�L�[
	const UINT One = KEY_INPUT_1;			// 1�L�[
	const UINT Two = KEY_INPUT_2;			// 2�L�[
	const UINT Three = KEY_INPUT_3;			// 3�L�[
	const UINT Four = KEY_INPUT_4;			// 4�L�[
	const UINT Five = KEY_INPUT_5;			// 5�L�[
	const UINT Six = KEY_INPUT_6;			// 6�L�[
	const UINT Seven = KEY_INPUT_7;			// 7�L�[
	const UINT Eight = KEY_INPUT_8;			// 8�L�[
	const UINT Nine = KEY_INPUT_9;			// 9�L�[
}

// �L�[�{�[�h���̓N���X
class Keyboard
{
private:
	// �R���X�g���N�^
	Keyboard();
	// �f�X�g���N�^
	~Keyboard() = default;

public:
	// �L�[�{�[�h�N���X�̎擾
	static Keyboard &getInstance()
	{
		static Keyboard k;
		return k;
	}

	// ����̃L�[�������ꂽ��
	bool KeyTriggerDown(UINT keycode);
	// ����̃L�[�������ꂽ��
	bool KeyTriggerUp(UINT keycode);
	// ����̃L�[��������Ă��邩
	bool KeyStateDown(UINT keycode);
	// ����̃L�[��������Ă��邩
	bool KeyStateUp(UINT keycode);

	// �����ꂩ�̃L�[�������ꂽ��
	bool AnyKeyTriggerDown(UINT keycode);
	// �����ꂩ�̃L�[�������ꂽ��
	bool AnyKeyTriggerUp(UINT keycode);
	// �����ꂩ�̃L�[��������Ă��邩
	bool AnyKeyStateDown(UINT keycode);
	// �����ꂩ�̃L�[��������Ă��邩
	bool AnyKeyStateUp(UINT keycode);

	// �X�V
	void update();

	// �R�s�[�֎~
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator = (const Keyboard&) = delete;
	// �ړ��֎~
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator = (Keyboard&&) = delete;

private:
	// �L�[�̏�Ԃ��X�V
	void update_key(char key, int num);

private:
	// �L�[�̏�Ԏ擾�p�z��
	char m_keycode_[256];
	// �L�[�̏�ԕۑ��p�z��
	std::array<int, 256> m_onkey_;		// ������Ă���
	std::array<int, 256> m_offkey_;		// ������Ă��Ȃ�
};

#endif // !KEYBOARD_H_