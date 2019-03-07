#ifndef PLAYER_INPUT_H_
#define PLAYER_INPUT_H_

#include "Vector2.h"

// クラス：プレイヤー入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class PlayerInput
{
public:
	// 攻撃入力をしたか
	static bool attack();
	// ガード入力をしたか
	static bool guard();
	// ガード入力を終了したか
	static bool guard_end();
	// 回避入力をしたか
	static bool evasion();

	// 前移動
	static bool move_forward();
	// 後移動
	static bool move_backward();
	// 左移動
	static bool move_left();
	// 右移動
	static bool move_right();
	// キーボードで移動しているか
	static bool keyboard_move();

	// 左スティックによる移動
	static Vector2 L_stick_move();
	// 左スティックで移動しているか
	static bool gamepad_move();

	// カメラ左回転
	static bool camera_turn_left();
	// カメラ右回転
	static bool camera_turn_right();
	// カメラ上回転
	static bool camera_turn_up();
	// カメラ下回転
	static bool camera_turn_down();

	// ゲーム終了
	static bool game_end();
};

#endif // !PLAYER_INPUT_H_