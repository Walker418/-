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
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::B))
		return true;

	return false;
}

// ガード入力をしたか
bool PlayerInput::guard()
{
	return (GamePad::getInstance().ButtonStateDown(PadButton::R) || Keyboard::getInstance().KeyStateDown(Keycode::M));
}

// ガード入力を終了したか
bool PlayerInput::guard_end()
{
	return (GamePad::getInstance().ButtonStateUp(PadButton::R) && Keyboard::getInstance().KeyStateUp(Keycode::M));
}

// 回避入力をしたか
bool PlayerInput::evasion()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::A) || Keyboard::getInstance().KeyTriggerDown(Keycode::N));
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

// 移動方向の入力
Vector2 PlayerInput::move_input()
{
	Vector2 input = Vector2::Zero;	// 入力情報を返す変数を宣言しておく

	// キーボードの入力を取得
	input.y = (move_forward()) ? 1.0f : (move_backward()) ? -1.0f : 0.0f;	// 前後
	input.x = (move_left()) ? -1.0f : (move_right()) ? 1.0f : 0.0f;			// 左右

	// キーボードの入力がなければ、ゲームパッドの入力を取得
	if (!keyboard_move()) input = GamePad::getInstance().L_Stick();

	// 正規化した方向入力を返す
	return input.Normalize();
}

// 左スティックで移動しているか
bool PlayerInput::gamepad_move()
{
	// 左スティックの入力は0でなければ、Trueを返す
	return move_input() != Vector2::Zero;
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

// カメラリセット
bool PlayerInput::camera_reset()
{
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::L))		return true;
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::R_Shift))	return true;

	return false;
}

// 雑魚戦スキップ
bool PlayerInput::skip_phase1()
{
	return (Keyboard::getInstance().KeyTriggerDown(Keycode::T));
}

// ポーズ
bool PlayerInput::pause()
{
	// ゲームパッドのスタートボタンが押されると、Trueを返す
	if (GamePad::getInstance().ButtonTriggerDown(PadButton::Start)) return true;
	// キーボードのYが押されると、Trueを返す
	if (Keyboard::getInstance().KeyTriggerDown(Keycode::Y)) return true;

	return false;
}

// ゲーム終了
bool PlayerInput::game_end()
{
	return (GamePad::getInstance().ButtonTriggerDown(PadButton::Back) || Keyboard::getInstance().KeyTriggerDown(Keycode::Escape));
}