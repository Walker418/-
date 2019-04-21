#include "CountdownTimer.h"
#include <algorithm>

// クラス：カウントダウンタイマー（DeltaTime式）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

// コンストラクタ（時間はフレーム数で入力）
CountdownTimer::CountdownTimer(float time)
{
	time_set_ = time;
}

// 更新
void CountdownTimer::update(float delta_time)
{
	// 設定された時間の間、現在時間を減算
	time_current_ = std::max(time_current_ - delta_time, 0.0f);
}

// リセット
void CountdownTimer::reset()
{
	time_current_ = time_set_;
}

// 強制終了
void CountdownTimer::shut()
{
	time_current_ = 0.0f;
}

// タイムアウトか
bool CountdownTimer::is_time_out() const
{
	return time_current_ <= 0.0f;
}