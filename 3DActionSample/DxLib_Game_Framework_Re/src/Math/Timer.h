#ifndef TIMER_H_
#define TIMER_H_

// クラス：タイマー（DeltaTime式）
// 製作者：何 兆祺（"Jacky" Ho Siu Ki）

class Timer
{
public:
	// デフォルトコンストラクタ（制限時間を設定しない）
	Timer() = default;
	// コンストラクタ（制限時間を設定する、制限時間はフレーム数で入力）
	Timer(float limit_time);
	// 更新
	void update(float delta_time);
	// 現在の時間を取得
	float get_time() const;
	// リセット
	void reset();
	// タイムアウトか
	bool is_time_out() const;

private:
	float time_current_{ 0.0f };	// 現在の時間
	float time_limit_{ 0.0f };		// 制限時間
};

#endif // !TIMER_H_