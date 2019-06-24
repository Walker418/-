#include "fpsController.h"
#include <DxLib.h>

// fps制御クラス

// 更新
bool fpsController::update()
{
	if (count_ == 0)	start_time_ = GetNowCount();
	if (count_ >= N)
	{
		int t = GetNowCount();
		fps_ = 1000.0f / ((t - start_time_) / (float)N);
		count_ = 0;
		start_time_ = t;
	}

	++count_;

	return true;
}

// 描画
void fpsController::draw()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", fps_);
}

// 待機
void fpsController::wait()
{
	int took_time = GetNowCount() - start_time_;
	int wait_time = count_ * 1000 / FPS - took_time;
	if (wait_time > 0)	Sleep(wait_time);
}