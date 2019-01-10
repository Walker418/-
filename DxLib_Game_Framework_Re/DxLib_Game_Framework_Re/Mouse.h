#ifndef MOUSE_H_
#define MOUSE_H_

#include "Vector2.h"

// クラス：マウス入力
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）
class Mouse
{
public:
	// マウスのボタンの定義
	static const int Left;
	static const int Right;
	static const int Middle;

	// 初期化
	static void initialize();
	// 更新
	static void update();
	// マウスカーソルの位置の取得
	static Vector2 cursor_point();
	// マウスカーソルの位置の設定
	static void cursor_point(const Vector2& point);
	// マウスカーソルの表示
	static void show_cursor();
	// マウスカーソルの非表示
	static void hide_cursor();
	// ボタンが押されているか
	static bool state(int button);
	// ボタンが押されたか
	static bool trigger(int button);
	// ボタンが離されたか
	static bool release(int button);
	// 終了処理
	static void finalize();

private:
	// 今回の入力状態
	static int current_state_;
	// 前回の入力状態
	static int previous_state_;
};

#endif // !MOUSE_H_