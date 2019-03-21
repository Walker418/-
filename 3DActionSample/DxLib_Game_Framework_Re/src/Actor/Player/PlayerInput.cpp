#include "PlayerInput.h"
#include "../../Input/GamePad.h"
#include "../../Input/Keyboard.h"

// クラス：プレイヤー入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// ゲームスタート入力をしたか
bool PlayerInput::game_start()
{
	// ゲームパッドのスタートボタンが押されると、Trueを返す
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::Start)) return true;
	// キーボードのスペースキーが押されると、Trueを返す
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::Space)) return true;

	return false;
}

// 攻撃入力をしたか
bool PlayerInput::attack()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::Y) || Keyboard::getInstance().KeyTriggerDown(Keycode::Space));
}

// ジャンプ攻撃入力をしたか
bool PlayerInput::jump_attack()
{
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::Y) && GamePad::getInstance().ButtonTriggerDown(PadButton::B))
		return true;
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::C))
		return true;

	return false;
}

// ガード入力をしたか
bool PlayerInput::guard()
{
	return (GamePad::getInstance().ButtonStateDown(PadButton::R) || Keyboard::getInstance().KeyStateDown(Keycode::Z));
}

// ガード入力を終了したか
bool PlayerInput::guard_end()
{
	return (GamePad::getInstance().ButtonStateUp(PadButton::R) && Keyboard::getInstance().KeyStateUp(Keycode::Z));
}

// 回避入力をしたか
bool PlayerInput::evasion()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::A) || Keyboard::getInstance().KeyTriggerDown(Keycode::X));
}

// 前移動
bool PlayerInput::move_forward()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::W);
}

// 後移動
bool PlayerInput::move_backward()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::S);
}

// 左移動
bool PlayerInput::move_left()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::A);
}

// 右移動
bool PlayerInput::move_right()
{
	return Keyboard::getInstance().KeyStateDown(Keycode::D);
}

// キーボードで移動しているか
bool PlayerInput::keyboard_move()
{
	// いずれかの方向入力があれば、Trueを返す
	return (move_forward() || move_backward() || move_left() || move_right());
}

// 左スティックによる移動
Vector2 PlayerInput::L_stick_move()
{
	// キーボードで移動している場合は反応しない（Vector2::Zeroを返す）
	if (keyboard_move())	return Vector2::Zero;

	return (GamePad::getInstance().L_Stick());
}

// 左スティックで移動しているか
bool PlayerInput::gamepad_move()
{
	// 左スティックの入力は0でなければ、Trueを返す
	return L_stick_move() != Vector2::Zero;
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