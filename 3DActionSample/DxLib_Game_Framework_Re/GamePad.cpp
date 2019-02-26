#include "GamePad.h"
#include <string>

// ゲームパッド入力クラス

// コンストラクタ
GamePad::GamePad() :
	current_L_stick_{ Vector2::Zero }, current_R_stick_{ Vector2::Zero }
{
	// 入力状態をクリア
	m_inputs_.clear();
	m_onbuttons_.clear();
	m_offbuttons_.clear();
	m_pads_.clear();
	m_onPOVbuttons_.clear();

	// 入力判定を初期化
	m_inputs_.push_back(PAD_INPUT_DOWN);
	m_inputs_.push_back(PAD_INPUT_UP);
	m_inputs_.push_back(PAD_INPUT_LEFT);
	m_inputs_.push_back(PAD_INPUT_RIGHT);
	m_inputs_.push_back(PAD_INPUT_1);
	m_inputs_.push_back(PAD_INPUT_2);
	m_inputs_.push_back(PAD_INPUT_3);
	m_inputs_.push_back(PAD_INPUT_4);
	m_inputs_.push_back(PAD_INPUT_5);
	m_inputs_.push_back(PAD_INPUT_6);
	m_inputs_.push_back(PAD_INPUT_7);
	m_inputs_.push_back(PAD_INPUT_8);
	m_inputs_.push_back(PAD_INPUT_9);
	m_inputs_.push_back(PAD_INPUT_10);
	m_inputs_.push_back(PAD_INPUT_11);
	m_inputs_.push_back(PAD_INPUT_12);

	// パッドの接続情報を初期化
	m_pads_.push_back(DX_INPUT_PAD1);
	m_pads_.push_back(DX_INPUT_PAD2);
	m_pads_.push_back(DX_INPUT_PAD3);
	m_pads_.push_back(DX_INPUT_PAD4);

	// 入力状態を初期化
	for (auto& j : m_pads_)
	{
		for (auto& i : m_inputs_)
		{
			m_onbuttons_[j][i] = 0;
			m_offbuttons_[j][i] = 0;
		}
		m_onPOVbuttons_[j] = 0;
	}
}

// 特定のボタンが押されたか
bool GamePad::ButtonTriggerDown(UINT button, int pad)
{
	Exception(pad, button);

	return m_onbuttons_[pad][button] == 1;
}

// 特定のボタンが離されたか
bool GamePad::ButtonTriggerUp(UINT button, int pad)
{
	Exception(pad, button);

	return m_offbuttons_[pad][button] == 1;
}

// 特定のボタンが押されているか
bool GamePad::ButtonStateDown(UINT button, int pad)
{
	Exception(pad, button);

	return m_onbuttons_[pad][button] > 0;
}

// 特定のボタンが離されているか
bool GamePad::ButtonStateUp(UINT button, int pad)
{
	Exception(pad, button);

	return m_offbuttons_[pad][button] > 0;
}

// いずれかのボタンが押されたか
bool GamePad::AnyButtonTriggerDown(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_onbuttons_[pad][button] == 1)
		{
			return true;
		}
	}

	return false;
}

// いずれかのボタンが離されたか
bool GamePad::AnyButtonTriggerUp(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_offbuttons_[pad][button] == 1)
		{
			return true;
		}
	}

	return false;
}

// いずれかのボタンが押されているか
bool GamePad::AnyButtonStateDown(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_onbuttons_[pad][button] > 0)
		{
			return true;
		}
	}

	return false;
}

// いずれかのボタンが離されているか
bool GamePad::AnyButtonStateUp(int pad)
{
	for (auto& button : m_inputs_)
	{
		Exception(pad, button);

		if (m_offbuttons_[pad][button] > 0)
		{
			return true;
		}
	}

	return false;
}

// POVの入力された瞬間の方向を取得
int GamePad::POVTriggerDown(int pad)
{
	// 入力がない場合、-1を返す
	if (m_onPOVbuttons_[pad] != 1)
		return -1;

	// 入力されてる方向を取得
	return POVStateDown(pad);
}

// POVの入力されてる方向を取得
int GamePad::POVStateDown(int pad)
{
	return GetJoypadPOVState(pad, 0) / 100;
}

// Lスティックの入力具合を取得
Vector2 GamePad::L_Stick(int pad)
{
	int x = 0, y = 0;
	GetJoypadAnalogInput(&x, &y, pad);

	return Vector2((float)x / 1000.0f, (float)-y / 1000.0f);
}

// 現フレームのLスティックの入力を取得
Vector2 GamePad::GetCurrent_L_Stick()
{
	return current_L_stick_;
}

// 前フレームのLスティックの入力を取得
Vector2 GamePad::GetPrevious_L_Stick()
{
	return previous_L_stick_;
}

// Rスティックの入力具合を取得
Vector2 GamePad::R_Stick(int pad)
{
	int x = 0, y = 0;
	GetJoypadAnalogInputRight(&x, &y, pad);
	return Vector2((float)x / 1000.0f, (float)-y / 1000.0f);
}

// 現フレームのRスティックの入力を取得
Vector2 GamePad::GetCurrent_R_Stick()
{
	return current_R_stick_;
}

// 前フレームのRスティックの入力を取得
Vector2 GamePad::GetPrevious_R_Stick()
{
	return previous_R_stick_;
}

// ゲームパッドの振動を開始させる
void GamePad::VibrationStart(int power, int time, int pad)
{
	StartJoypadVibration(pad, power, time);
}

// ゲームパッドの振動を停止させる
void GamePad::VibrationStop(int pad)
{
	StopJoypadVibration(pad);
}

// 更新
void GamePad::update()
{
	// 各ボタンの状態を更新
	for (auto& pad : m_pads_)
	{
		for (auto& button : m_inputs_)
		{
			if ((GetJoypadInputState(pad) &button) == 0)
			{
				m_onbuttons_[pad][button] = 0;
				m_offbuttons_[pad][button]++;
			}
			else
			{
				m_offbuttons_[pad][button] = 0;
				m_onbuttons_[pad][button]++;
			}
		}
		if (GetJoypadPOVState(pad, 0) == -1)
		{
			m_onPOVbuttons_[pad] = 0;
		}
		else
		{
			m_onPOVbuttons_[pad]++;
		}
	}

	// Lスティックの状態を更新
	previous_L_stick_ = current_L_stick_;
	int x = 0, y = 0;
	GetJoypadAnalogInput(&x, &y, 1);
	current_L_stick_ = Vector2((float)x / 1000.0f, (float)-y / 1000.0f);

	// Rスティックの状態を更新
	previous_R_stick_ = current_R_stick_;
	GetJoypadAnalogInputRight(&x, &y, 1);
	current_R_stick_ = Vector2((float)x / 1000.0f, (float)-y / 1000.0f);
}

void GamePad::Exception(int pad, int button)
{
	if (m_onbuttons_.find(pad) == m_onbuttons_.end())
		throw std::string("残念ながら、指定したパッド番号は使用できません");

	if (m_onbuttons_[pad].find(button) == m_onbuttons_[pad].end())
		throw std::string("残念ながら、指定したボタン番号は使用できません");
}