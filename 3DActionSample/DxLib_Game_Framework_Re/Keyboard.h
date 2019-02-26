#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <DxLib.h>
#include <array>

// キーボードのキー
namespace Keycode
{
	const UINT Back = KEY_INPUT_BACK;				// バックスペースキー
	const UINT Tab = KEY_INPUT_TAB;					// タブキー
	const UINT Return = KEY_INPUT_RETURN;			// エンターキー
	const UINT L_Shift = KEY_INPUT_LSHIFT;			// 左シフトキー
	const UINT R_Shift = KEY_INPUT_RSHIFT;			// 右シフトキー
	const UINT L_Control = KEY_INPUT_LCONTROL;		// 左コントロールキー
	const UINT R_Control = KEY_INPUT_RCONTROL;		// 右コントロールキー
	const UINT Escape = KEY_INPUT_ESCAPE;			// エスケープキー
	const UINT Space = KEY_INPUT_SPACE;				// スペースキー
	const UINT PageUp = KEY_INPUT_PGUP;				// ページアップキー
	const UINT PageDown = KEY_INPUT_PGDN;			// ページダウンキー
	const UINT End = KEY_INPUT_END;					// エンドキー
	const UINT Home = KEY_INPUT_HOME;				// ホームキー
	const UINT Left = KEY_INPUT_LEFT;				// 左キー
	const UINT Up = KEY_INPUT_UP;					// 上キー
	const UINT Right = KEY_INPUT_RIGHT;				// 右キー
	const UINT Down = KEY_INPUT_DOWN;				// 下キー
	const UINT Insert = KEY_INPUT_INSERT;			// インサートキー
	const UINT Delete = KEY_INPUT_DELETE;			// デリートキー

	const UINT Minus = KEY_INPUT_MINUS;				// -キー
	const UINT Yen = KEY_INPUT_YEN;					// \キー
	const UINT PrevTrack = KEY_INPUT_PREVTRACK;		// ^キー
	const UINT Period = KEY_INPUT_PERIOD;			// .キー
	const UINT Slash = KEY_INPUT_SLASH;				// /キー
	const UINT L_Alt = KEY_INPUT_LALT;				// 左Altキー
	const UINT R_Alt = KEY_INPUT_RALT;				// 右Altキー
	const UINT Scroll = KEY_INPUT_SCROLL;			// ScrollLockキー
	const UINT SemiColon = KEY_INPUT_SEMICOLON;		// ;キー
	const UINT Colon = KEY_INPUT_COLON;				// :キー
	const UINT L_Bracket = KEY_INPUT_LBRACKET;		// [キー
	const UINT R_Bracket = KEY_INPUT_RBRACKET;		// ]キー
	const UINT At = KEY_INPUT_AT;					// @キー
	const UINT BackSlash = KEY_INPUT_BACKSLASH;		// \キー
	const UINT Comma = KEY_INPUT_COMMA;				// ,キー
	const UINT CapsLock = KEY_INPUT_CAPSLOCK;		// CapsLockキー
	const UINT Pause = KEY_INPUT_PAUSE;				// PauseBreakキー

	const UINT NumPad_0 = KEY_INPUT_NUMPAD0;			// テンキー 0
	const UINT NumPad_1 = KEY_INPUT_NUMPAD1;			// テンキー 1
	const UINT NumPad_2 = KEY_INPUT_NUMPAD2;			// テンキー 2
	const UINT NumPad_3 = KEY_INPUT_NUMPAD3;			// テンキー 3
	const UINT NumPad_4 = KEY_INPUT_NUMPAD4;			// テンキー 4
	const UINT NumPad_5 = KEY_INPUT_NUMPAD5;			// テンキー 5
	const UINT NumPad_6 = KEY_INPUT_NUMPAD6;			// テンキー 6
	const UINT NumPad_7 = KEY_INPUT_NUMPAD7;			// テンキー 7
	const UINT NumPad_8 = KEY_INPUT_NUMPAD8;			// テンキー 8
	const UINT NumPad_9 = KEY_INPUT_NUMPAD9;			// テンキー 9
	const UINT Multiply = KEY_INPUT_MULTIPLY;			// テンキー *
	const UINT Add = KEY_INPUT_ADD;						// テンキー +
	const UINT Subtract = KEY_INPUT_SUBTRACT;			// テンキー -
	const UINT Decimal = KEY_INPUT_DECIMAL;				// テンキー .
	const UINT Divide = KEY_INPUT_DIVIDE;				// テンキー /
	const UINT NumPad_Enter = KEY_INPUT_NUMPADENTER;	// テンキー Enter

	const UINT F1 = KEY_INPUT_F1;			// F1キー
	const UINT F2 = KEY_INPUT_F2;			// F2キー
	const UINT F3 = KEY_INPUT_F3;			// F3キー
	const UINT F4 = KEY_INPUT_F4;			// F4キー
	const UINT F5 = KEY_INPUT_F5;			// F5キー
	const UINT F6 = KEY_INPUT_F6;			// F6キー
	const UINT F7 = KEY_INPUT_F7;			// F7キー
	const UINT F8 = KEY_INPUT_F8;			// F8キー
	const UINT F9 = KEY_INPUT_F9;			// F9キー
	const UINT F10 = KEY_INPUT_F10;			// F10キー
	const UINT F11 = KEY_INPUT_F11;			// F11キー
	const UINT F12 = KEY_INPUT_F12;			// F12キー

	const UINT A = KEY_INPUT_A;				// Aキー
	const UINT B = KEY_INPUT_B;				// Bキー
	const UINT C = KEY_INPUT_C;				// Cキー
	const UINT D = KEY_INPUT_D;				// Dキー
	const UINT E = KEY_INPUT_E;				// Eキー
	const UINT F = KEY_INPUT_F;				// Fキー
	const UINT G = KEY_INPUT_G;				// Gキー
	const UINT H = KEY_INPUT_H;				// Hキー
	const UINT I = KEY_INPUT_I;				// Iキー
	const UINT J = KEY_INPUT_J;				// Jキー
	const UINT K = KEY_INPUT_K;				// Kキー
	const UINT L = KEY_INPUT_L;				// Lキー
	const UINT M = KEY_INPUT_M;				// Mキー
	const UINT N = KEY_INPUT_N;				// Nキー
	const UINT O = KEY_INPUT_O;				// Oキー
	const UINT P = KEY_INPUT_P;				// Pキー
	const UINT Q = KEY_INPUT_Q;				// Qキー
	const UINT R = KEY_INPUT_R;				// Rキー
	const UINT S = KEY_INPUT_S;				// Sキー
	const UINT T = KEY_INPUT_T;				// Tキー
	const UINT U = KEY_INPUT_U;				// Uキー
	const UINT V = KEY_INPUT_V;				// Vキー
	const UINT W = KEY_INPUT_W;				// Wキー
	const UINT X = KEY_INPUT_X;				// Xキー
	const UINT Y = KEY_INPUT_Y;				// Yキー
	const UINT Z = KEY_INPUT_Z;				// Zキー

	const UINT Zero = KEY_INPUT_0;			// 0キー
	const UINT One = KEY_INPUT_1;			// 1キー
	const UINT Two = KEY_INPUT_2;			// 2キー
	const UINT Three = KEY_INPUT_3;			// 3キー
	const UINT Four = KEY_INPUT_4;			// 4キー
	const UINT Five = KEY_INPUT_5;			// 5キー
	const UINT Six = KEY_INPUT_6;			// 6キー
	const UINT Seven = KEY_INPUT_7;			// 7キー
	const UINT Eight = KEY_INPUT_8;			// 8キー
	const UINT Nine = KEY_INPUT_9;			// 9キー
}

// キーボード入力クラス
class Keyboard
{
private:
	// コンストラクタ
	Keyboard();
	// デストラクタ
	~Keyboard() = default;

public:
	// キーボードクラスの取得
	static Keyboard &getInstance()
	{
		static Keyboard k;
		return k;
	}

	// 特定のキーが押されたか
	bool KeyTriggerDown(UINT keycode);
	// 特定のキーが離されたか
	bool KeyTriggerUp(UINT keycode);
	// 特定のキーが押されているか
	bool KeyStateDown(UINT keycode);
	// 特定のキーが離されているか
	bool KeyStateUp(UINT keycode);

	// いずれかのキーが押されたか
	bool AnyKeyTriggerDown(UINT keycode);
	// いずれかのキーが離されたか
	bool AnyKeyTriggerUp(UINT keycode);
	// いずれかのキーが押されているか
	bool AnyKeyStateDown(UINT keycode);
	// いずれかのキーが離されているか
	bool AnyKeyStateUp(UINT keycode);

	// 更新
	void update();

	// コピー禁止
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator = (const Keyboard&) = delete;
	// 移動禁止
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator = (Keyboard&&) = delete;

private:
	// キーの状態を更新
	void update_key(char key, int num);

private:
	// キーの状態取得用配列
	char m_keycode_[256];
	// キーの状態保存用配列
	std::array<int, 256> m_onkey_;		// 押されている
	std::array<int, 256> m_offkey_;		// 押されていない
};

#endif // !KEYBOARD_H_