#include "GamePad.h"
#include <DxLib.h>

// クラス：ゲームパッド入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// ゲームパッドのボタンの定義
const int GamePad::Left{ PAD_INPUT_LEFT };		// 左
const int GamePad::Right{ PAD_INPUT_RIGHT };	// 右
const int GamePad::Up{ PAD_INPUT_UP };			// 上
const int GamePad::Down{ PAD_INPUT_DOWN };		// 下
const int GamePad::A{ PAD_INPUT_1 };			// A
const int GamePad::B{ PAD_INPUT_2 };			// B
const int GamePad::X{ PAD_INPUT_3 };			// X
const int GamePad::Y{ PAD_INPUT_4 };			// Y
const int GamePad::L{ PAD_INPUT_5 };			// L
const int GamePad::R{ PAD_INPUT_6 };			// R
const int GamePad::Back{ PAD_INPUT_9 };			// Back
const int GamePad::Start{ PAD_INPUT_10 };		// Start

// 今回の入力状態
int GamePad::current_state_{ 0 };
// 前回の入力状態
int GamePad::previous_state_{ 0 };

// 左アナログのx軸入力状態
int GamePad::analogL_X_{ 0 };
// 左アナログのy軸入力状態
int GamePad::analogL_Y_{ 0 };

// 右アナログのx軸入力状態
int GamePad::analogR_X_{ 0 };
// 右アナログのy軸入力状態
int GamePad::analogR_Y_{ 0 };

// 初期化
void GamePad::initialize()
{
	current_state_ = 0;
	previous_state_ = 0;

	analogL_X_ = 0;
	analogL_Y_ = 0;

	analogR_X_ = 0;
	analogR_Y_ = 0;
}

// 更新
void GamePad::update()
{
	// ボタン入力状況を更新
	previous_state_ = current_state_;
	current_state_ = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 左アナログスティック入力状況を更新
	GetJoypadAnalogInput(&analogL_X_, &analogL_Y_, DX_INPUT_KEY_PAD1);

	// 右アナログスティック入力状況を更新
	GetJoypadAnalogInput(&analogR_X_, &analogR_Y_, DX_INPUT_KEY_PAD1);
}

// ボタンが押されているか
bool GamePad::state(int button)
{
	return (current_state_ &button) != 0;
}

// ボタンが押されたか
bool GamePad::trigger(int button)
{
	return (current_state_ & ~previous_state_ &button) != 0;
}

// ボタンが離されたか
bool GamePad::release(int button)
{
	return (~current_state_ & previous_state_ &button) != 0;
}

// 左アナログのx軸入力状態の取得
int GamePad::get_analogL_X()
{
	return analogL_X_;
}

// 左アナログのy軸入力状態の取得
int GamePad::get_analogL_Y()
{
	return analogL_Y_;
}

// 右アナログのx軸入力状態の取得
int GamePad::get_analogR_X()
{
	return analogR_X_;
}

// 右アナログのy軸入力状態の取得
int GamePad::get_analogR_Y()
{
	return analogR_Y_;
}

// 終了処理
void GamePad::finalize()
{
	// 初期化と同じ処理
	initialize();
}