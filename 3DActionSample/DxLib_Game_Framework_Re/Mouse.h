#ifndef MOUSE_H_
#define MOUSE_H_

#include <DxLib.h>
#include <array>
#include "Vector2.h"

// マウスのボタン
namespace MouseButton
{
	const UINT Left = MOUSE_INPUT_LEFT;			// 左ボタン
	const UINT Right = MOUSE_INPUT_RIGHT;		// 右ボタン
	const UINT Middle = MOUSE_INPUT_MIDDLE;		// 中央ボタン
	const UINT Button4 = MOUSE_INPUT_4;			// ボタン4
	const UINT Button5 = MOUSE_INPUT_5;			// ボタン5
	const UINT Button6 = MOUSE_INPUT_6;			// ボタン6
	const UINT Button7 = MOUSE_INPUT_7;			// ボタン7
	const UINT Button8 = MOUSE_INPUT_8;			// ボタン8
}

// マウス入力クラス
class Mouse
{
private:
	// コンストラクタ
	Mouse();
	// デストラクタ
	~Mouse() = default;

public:
	// マウスクラスの取得
	static Mouse &getInstance()
	{
		static Mouse m;
		return m;
	}

	// ボタンが押されているか
	bool State(UINT button);
	// ボタンが押されたか
	bool Trigger(UINT button);
	// ボタンが離されたか
	bool Release(UINT button);

	// マウスカーソルの位置を取得
	Vector2 cursor_point();
	// マウスカーソルの位置を設定
	void cursor_point(const Vector2& point);
	// マウスカーソルの表示
	void show_cursor();
	// マウスカーソルの非表示
	void hide_cursor();

	// 更新
	void update();

	// コピー禁止
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;
	// 移動禁止
	Mouse(Mouse&&) = delete;
	Mouse& operator = (Mouse&&) = delete;

private:
	// 入力状態保存用変数
	int current_state_;		// 現フレーム
	int previous_state_;	// 前フレーム
};

#endif // !MOUSE_H_