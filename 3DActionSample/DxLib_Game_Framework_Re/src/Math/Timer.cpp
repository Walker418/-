#include "Timer.h"
#include <algorithm>

// クラス：タイマー（DeltaTime式）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ（制限時間を設定する）
Timer::Timer(float limit_time)
{
	time_limit_ = limit_time;
}

// 更新
void Timer::update(float delta_time)
{
	// 制限時間を設定されていない場合、ずっとDeltaTimeを加算
	// 制限時間を設定されている場合、制限時間に達するまでDeltaTimeを加算
	(time_limit_ <= 0.0f) ? time_current_ += delta_time : time_current_ = std::min(time_current_ + delta_time, time_limit_);
}

// 現在の時間を取得
float Timer::get_time() const
{
	return time_current_;
}

// リセット
void Timer::reset()
{
	time_current_ = 0.0f;
}

// タイムアウトか
bool Timer::is_time_out() const
{
	if (time_limit_ <= 0.0f) return false;	// 制限時間を設定されていない場合、falseを返す

	return time_current_ >= time_limit_;
}