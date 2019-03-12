#ifndef GAME_PAD_H_
#define GAME_PAD_H_

#include <DxLib.h>
#include "../Math/Vector2.h"
#include <unordered_map>
#include <vector>
#include <array>

// 接続しているゲームパッドの番号
namespace PadNo
{
	const UINT Pad1 = DX_INPUT_PAD1;
	const UINT Pad2 = DX_INPUT_PAD2;
	const UINT Pad3 = DX_INPUT_PAD3;
	const UINT Pad4 = DX_INPUT_PAD4;
}

// ゲームパッドのボタン
// p.s.十字キーと書いてあるが実際はスティックのことらしい
namespace PadButton
{
	const UINT Left = PAD_INPUT_LEFT;		// 左ボタン
	const UINT Right = PAD_INPUT_RIGHT;		// 右ボタン
	const UINT Up = PAD_INPUT_UP;			// 上ボタン
	const UINT Down = PAD_INPUT_DOWN;		// 下ボタン
	const UINT A = PAD_INPUT_1;				// Aボタン
	const UINT B = PAD_INPUT_2;				// Bボタン
	const UINT X = PAD_INPUT_3;				// Xボタン
	const UINT Y = PAD_INPUT_4;				// Yボタン
	const UINT L = PAD_INPUT_5;				// Lボタン
	const UINT R = PAD_INPUT_6;				// Rボタン
	const UINT Back = PAD_INPUT_7;			// Backボタン
	const UINT Start = PAD_INPUT_8;			// Startボタン
	const UINT NUM9 = PAD_INPUT_9;
	const UINT NUM10 = PAD_INPUT_10;
	const UINT NUM11 = PAD_INPUT_11;
	const UINT NUM12 = PAD_INPUT_12;
}

// ゲームパッド入力クラス
class GamePad
{
private:
	// コンストラクタ
	GamePad();
	// デストラクタ
	~GamePad() = default;

public:
	// ゲームパッドクラスの取得
	static GamePad &getInstance()
	{
		static GamePad g;
		return g;
	}

	// 特定のボタンが押されたか
	bool ButtonTriggerDown(UINT button, int pad = PadNo::Pad1);
	// 特定のボタンが離されたか
	bool ButtonTriggerUp(UINT button, int pad = PadNo::Pad1);
	// 特定のボタンが押されているか
	bool ButtonStateDown(UINT button, int pad = PadNo::Pad1);
	// 特定のボタンが離されているか
	bool ButtonStateUp(UINT button, int pad = PadNo::Pad1);

	// いずれかのボタンが押されたか
	bool AnyButtonTriggerDown(int pad = PadNo::Pad1);
	// いずれかのボタンが離されたか
	bool AnyButtonTriggerUp(int pad = PadNo::Pad1);
	// いずれかのボタンが押されているか
	bool AnyButtonStateDown(int pad = PadNo::Pad1);
	// いずれかのボタンが離されているか
	bool AnyButtonStateUp(int pad = PadNo::Pad1);
	
	// POV（十字キー）の入力された瞬間の方向を取得
	int POVTriggerDown(int pad = PadNo::Pad1);
	// POVの入力されてる方向を取得
	int POVStateDown(int pad = PadNo::Pad1);

	// Lスティックの入力具合を取得
	// (x = -1.0 ~ 1.0, y = -1.0 ~ 1.0)
	Vector2 L_Stick(int pad = PadNo::Pad1);
	// 現フレームのLスティックの入力を取得
	Vector2 GetCurrent_L_Stick();
	// 前フレームのLスティックの入力を取得
	Vector2 GetPrevious_L_Stick();
	// Rスティックの入力具合を取得
	// (x = -1.0 ~ 1.0, y = -1.0 ~ 1.0)
	Vector2 R_Stick(int pad = PadNo::Pad1);
	// 現フレームのRスティックの入力を取得
	Vector2 GetCurrent_R_Stick();
	// 前フレームのRスティックの入力を取得
	Vector2 GetPrevious_R_Stick();

	// ゲームパッドの振動を開始させる
	void VibrationStart(int power, int time, int pad = PadNo::Pad1);
	// ゲームパッドの振動を停止させる
	void VibrationStop(int pad = PadNo::Pad1);

	// 更新
	void update();

	// コピー禁止
	GamePad(const GamePad&) = delete;
	GamePad& operator = (const GamePad&) = delete;
	// 移動禁止
	GamePad(GamePad&&) = delete;
	GamePad& operator = (GamePad&&) = delete;

private:
	void Exception(int pad, int button);

private:
	// ボタンの状態保存用マップ
	std::unordered_map<int, std::unordered_map<int, int>> m_onbuttons_;			// 押されている
	std::unordered_map<int, std::unordered_map<int, int>> m_offbuttons_;		// 押されていない
	// 十字キーの状態
	std::unordered_map<int, int> m_onPOVbuttons_;
	// ゲームパッドの番号
	std::vector<int> m_pads_;
	// ボタンの入力判定
	std::vector<int> m_inputs_;

	// 現フレームのLスティック入力
	Vector2 current_L_stick_;
	// 前フレームのLスティック入力
	Vector2 previous_L_stick_;
	// 現フレームのRスティック入力
	Vector2 current_R_stick_;
	// 前フレームのRスティック入力
	Vector2 previous_R_stick_;
};

#endif // !GAME_PAD_H_