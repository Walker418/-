#include "Keyboard.h"
#include <DxLib.h>

// クラス：キーボード入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// キーボードのキーの定義
const int Keyboard::Esc{ KEY_INPUT_ESCAPE };
const int Keyboard::Enter{ KEY_INPUT_RETURN };
const int Keyboard::Space{ KEY_INPUT_SPACE };
const int Keyboard::LeftCtrl{ KEY_INPUT_LCONTROL };
const int Keyboard::RightCtrl{ KEY_INPUT_RCONTROL };
const int Keyboard::LeftShift{ KEY_INPUT_LSHIFT };
const int Keyboard::RightShift{ KEY_INPUT_RSHIFT };

const int Keyboard::Up{ KEY_INPUT_UP };
const int Keyboard::Down{ KEY_INPUT_DOWN };
const int Keyboard::Left{ KEY_INPUT_LEFT };
const int Keyboard::Right{ KEY_INPUT_RIGHT };

const int Keyboard::Num0{ KEY_INPUT_NUMPAD0 };
const int Keyboard::Num1{ KEY_INPUT_NUMPAD1 };
const int Keyboard::Num2{ KEY_INPUT_NUMPAD2 };
const int Keyboard::Num3{ KEY_INPUT_NUMPAD3 };
const int Keyboard::Num4{ KEY_INPUT_NUMPAD4 };
const int Keyboard::Num5{ KEY_INPUT_NUMPAD5 };
const int Keyboard::Num6{ KEY_INPUT_NUMPAD6 };
const int Keyboard::Num7{ KEY_INPUT_NUMPAD7 };
const int Keyboard::Num8{ KEY_INPUT_NUMPAD8 };
const int Keyboard::Num9{ KEY_INPUT_NUMPAD9 };

const int Keyboard::A{ KEY_INPUT_A };
const int Keyboard::B{ KEY_INPUT_B };
const int Keyboard::C{ KEY_INPUT_C };
const int Keyboard::D{ KEY_INPUT_D };
const int Keyboard::E{ KEY_INPUT_E };
const int Keyboard::F{ KEY_INPUT_F };
const int Keyboard::G{ KEY_INPUT_G };
const int Keyboard::H{ KEY_INPUT_H };
const int Keyboard::I{ KEY_INPUT_I };
const int Keyboard::J{ KEY_INPUT_J };
const int Keyboard::K{ KEY_INPUT_K };
const int Keyboard::L{ KEY_INPUT_L };
const int Keyboard::M{ KEY_INPUT_M };
const int Keyboard::N{ KEY_INPUT_N };
const int Keyboard::O{ KEY_INPUT_O };
const int Keyboard::P{ KEY_INPUT_P };
const int Keyboard::Q{ KEY_INPUT_Q };
const int Keyboard::R{ KEY_INPUT_R };
const int Keyboard::S{ KEY_INPUT_S };
const int Keyboard::T{ KEY_INPUT_T };
const int Keyboard::U{ KEY_INPUT_U };
const int Keyboard::V{ KEY_INPUT_V };
const int Keyboard::W{ KEY_INPUT_W };
const int Keyboard::X{ KEY_INPUT_X };
const int Keyboard::Y{ KEY_INPUT_Y };
const int Keyboard::Z{ KEY_INPUT_Z };

const int Keyboard::One{ KEY_INPUT_1 };
const int Keyboard::Two{ KEY_INPUT_2 };
const int Keyboard::Three{ KEY_INPUT_3 };
const int Keyboard::Four{ KEY_INPUT_4 };
const int Keyboard::Five{ KEY_INPUT_5 };
const int Keyboard::Six{ KEY_INPUT_6 };
const int Keyboard::Seven{ KEY_INPUT_7 };
const int Keyboard::Eight{ KEY_INPUT_8 };
const int Keyboard::Nine{ KEY_INPUT_9 };
const int Keyboard::Zero{ KEY_INPUT_0 };

// 今回の入力状態
int Keyboard::current_state_{ 0 };
// 前回の入力状態
int Keyboard::previous_state_{ 0 };

// 初期化
void Keyboard::initialize()
{
	current_state_ = 0;
	previous_state_ = 0;
}

// 更新
void Keyboard::update()
{
	
}

