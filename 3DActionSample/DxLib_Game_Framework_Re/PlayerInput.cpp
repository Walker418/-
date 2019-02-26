#include "PlayerInput.h"
#include "GamePad.h"
#include "Keyboard.h"

// クラス：プレイヤー入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// 攻撃入力をしたか
bool PlayerInput::attack()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::B) || Keyboard::getInstance().KeyTriggerDown(Keycode::Space));
}

// ガード入力をしたか
bool PlayerInput::guard()
{
	return (GamePad::getInstance().ButtonStateDown(PadButton::R) || Keyboard::getInstance().KeyStateDown(Keycode::L_Control));
}

// ガード入力を終了したか
bool PlayerInput::guard_end()
{
	return (GamePad::getInstance().ButtonStateUp(PadButton::R) && Keyboard::getInstance().KeyStateUp(Keycode::L_Control));
}

// 回避入力をしたか
bool PlayerInput::skip()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::A) || Keyboard::getInstance().KeyTriggerDown(Keycode::L_Shift));
}

// 前移動
bool PlayerInput::move_forward()
{
	return (GamePad::getInstance().GetCurrent_L_Stick().y > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::W));
}

// 後移動
bool PlayerInput::move_backward()
{
	return (GamePad::getInstance().GetCurrent_L_Stick().y < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::S));
}

// 左移動
bool PlayerInput::move_left()
{
	return (GamePad::getInstance().GetCurrent_L_Stick().x < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::A));
}

// 右移動
bool PlayerInput::move_right()
{
	return (GamePad::getInstance().GetCurrent_L_Stick().x > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::D));
}

// カメラ左回転
bool PlayerInput::camera_turn_left()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().x < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Left));
}

// カメラ右回転
bool PlayerInput::camera_turn_right()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().x > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Right));
}

// カメラ上回転
bool PlayerInput::camera_turn_up()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().y > 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Up));
}

// カメラ下回転
bool PlayerInput::camera_turn_down()
{
	return (GamePad::getInstance().GetCurrent_R_Stick().y < 0.0f || Keyboard::getInstance().KeyStateDown(Keycode::Down));
}

// ゲーム終了
bool PlayerInput::game_end()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::Back) || Keyboard::getInstance().KeyTriggerDown(Keycode::Escape));
}