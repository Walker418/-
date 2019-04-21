#ifndef COUNTDOWN_TIMER_H_
#define COUNTDOWN_TIMER_H_

// クラス：カウントダウンタイマー（DeltaTime式）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class CountdownTimer
{
public:
	// コンストラクタ（時間はフレーム数で入力）
	CountdownTimer(float time);
	// 更新
	void update(float delta_time);
	// リセット
	void reset();
	// タイムアウトか
	bool is_time_out() const;

private:
	float time_current_{ 0.0f };	// 現在の時間
	float time_set_{ 0.0f };		// 設定時間
};

#endif // !COUNTDOWN_TIMER_H_