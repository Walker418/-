#include "Mouse.h"

// マウス入力クラス

// コンストラクタ
Mouse::Mouse() :
	current_state_{ 0 }, previous_state_{ 0 }
{
	SetUseDirectInputFlag(TRUE);	// これを実行しないと、マウス5ボタンまでしか取得できない
}

// ボタンが押されているか
bool Mouse::State(UINT button)
{
	return (current_state_ & button) != 0;
}

// ボタンが押されたか
bool Mouse::Trigger(UINT button)
{
	return (current_state_ & ~previous_state_ & button) != 0;
}

// ボタンが離されたか
bool Mouse::Release(UINT button)
{
	return (~current_state_ & previous_state_ & button) != 0;
}

// マウスカーソルの位置を取得
Vector2 Mouse::cursor_point()
{
	int x = 0, y = 0;
	GetMousePoint(&x, &y);
	return Vector2(float(x), float(y));
}

// マウスカーソルの位置を設定
void Mouse::cursor_point(const Vector2& point)
{
	SetMousePoint(int(point.x), int(point.y));
}

// マウスカーソルの表示
void Mouse::show_cursor()
{
	SetMouseDispFlag(TRUE);
}

// マウスカーソルの非表示
void Mouse::hide_cursor()
{
	SetMouseDispFlag(FALSE);
}

// 更新
void Mouse::update()
{
	previous_state_ = current_state_;
	current_state_ = GetMouseInput();
}