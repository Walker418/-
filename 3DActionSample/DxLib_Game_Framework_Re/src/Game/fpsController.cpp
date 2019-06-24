#include "fpsController.h"
#include <DxLib.h>

// fps����N���X

// �X�V
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

// �`��
void fpsController::draw()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", fps_);
}

// �ҋ@
void fpsController::wait()
{
	int took_time = GetNowCount() - start_time_;
	int wait_time = count_ * 1000 / FPS - took_time;
	if (wait_time > 0)	Sleep(wait_time);
}